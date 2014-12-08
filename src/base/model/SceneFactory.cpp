/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

//#include <Carna/base/view/glew.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/model/SceneFactory.h>
#include <Carna/base/model/Volume.h>
#include <Carna/base/model/VolumeDetails.h>
#include <Carna/base/model/SeriesLoadingWorker.h>
#include <Carna/base/model/SeriesLoadingRequest.h>
#include <Carna/base/model/SeriesElement.h>
#include <Carna/base/model/Series.h>
#include <Carna/base/qt/CarnaProgressDialog.h>
#include <Carna/base/qt/DicomOpeningDialog.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Parallelization.h>
#include <dicom-interface/dicomImage.h>
#include <dicom-interface/dicomSeries.h>
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <limits>
#include <sstream>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// getWorkerCount
// ----------------------------------------------------------------------------------

unsigned int getWorkerCount( const ParallelizationSettings& wishedSettings )
{
    switch( wishedSettings.threads )
    {

        case 0:
        case 1:
            return 1;

        case 2:
        case 3:
            return 2;

        case 4:
        default:
            return 4;

    }
}



// ----------------------------------------------------------------------------------
// configureWorkers
// ----------------------------------------------------------------------------------

template< typename ConcreteParallelization >
static void configureWorkers( ConcreteParallelization& job, const Vector3ui& size )
{
    qDebug() << "*** configuring" << job.countWorkers() << "workers...";
    switch( job.countWorkers() )
    {

        case 1:
        {
            job.workerByIndex( 0 ).setSetup( new UInt16VolumeBaseWorker::Setup
                ( Vector3ui( 0, 0, 0 )
                , Vector3ui( size.x - 1, size.y - 1, size.z - 1 ) ) );

            break;
        }

        case 2:
        {
            job.workerByIndex( 0 ).setSetup( new UInt16VolumeBaseWorker::Setup
                ( Vector3ui( 0, 0, 0 )
                , Vector3ui( size.x - 1, size.y - 1, size.z / 2 - 1 ) ) );

            job.workerByIndex( 1 ).setSetup( new UInt16VolumeBaseWorker::Setup
                ( Vector3ui( 0, 0, size.z / 2 )
                , Vector3ui( size.x - 1, size.y - 1, size.z - 1 ) ) );

            break;
        }

        case 4:
        {
            job.workerByIndex( 0 ).setSetup( new UInt16VolumeBaseWorker::Setup
                ( Vector3ui( 0, 0, 0 )
                , Vector3ui( size.x - 1, size.y / 2 - 1, size.z / 2 - 1 ) ) );

            job.workerByIndex( 1 ).setSetup( new UInt16VolumeBaseWorker::Setup
                ( Vector3ui( 0, 0, size.z / 2 )
                , Vector3ui( size.x - 1, size.y / 2 - 1, size.z - 1 ) ) );

            job.workerByIndex( 2 ).setSetup( new UInt16VolumeBaseWorker::Setup
                ( Vector3ui( 0, size.y / 2, 0 )
                , Vector3ui( size.x - 1, size.y - 1, size.z / 2 - 1 ) ) );

            job.workerByIndex( 3 ).setSetup( new UInt16VolumeBaseWorker::Setup
                ( Vector3ui( 0, size.y / 2, size.z / 2 )
                , Vector3ui( size.x - 1, size.y - 1, size.z - 1 ) ) );

            break;
        }

        default:
        {
            CARNA_FAIL( "unsupported workers count (" << job.countWorkers() << ")" );
        }

    }
}



// ----------------------------------------------------------------------------------
// SceneFactory
// ----------------------------------------------------------------------------------

SceneFactory::SceneFactory( QWidget* dialogParent
                          , const ParallelizationSettings& parallelizationSettings
                          , QObject* parent )
    : QObject( parent )
    , dialogParent( dialogParent )
    , parallelizationSettings( parallelizationSettings )
    , max_allowed_side_length( view::SceneProvider::max3DTextureSize() / 3 )
{
}


void SceneFactory::setMaximumAllowedSideLength( unsigned int max_allowed_side_length )
{
    this->max_allowed_side_length = max_allowed_side_length;
}


unsigned int SceneFactory::maximumAllowedSideLength() const
{
    return max_allowed_side_length;
}


int SceneFactory::computeVoidThreshold( const Volume& volume )
{
    CARNA_ASSERT( volume.size.x >= 0 && volume.size.y >= 0 && volume.size.z >= 0 );

    double sum = 0;
    for( unsigned int side = 0; side < 2; ++side )
    {
        const unsigned int z = side * ( volume.size.z - 1 );

        for( unsigned int x = 0; x < unsigned( volume.size.x ); ++x )
            for( unsigned int y = 0; y < unsigned( volume.size.y ); ++y )
            {
                sum += volume( x, y, z );
            }

            const unsigned int y = side * ( volume.size.y - 1 );

            for( unsigned int x = 0; x < unsigned( volume.size.x ); ++x )
                for( unsigned int z = 0; z < unsigned( volume.size.z ); ++z )
                {
                    sum += volume( x, y, z );
                }

                const unsigned int x = side * ( volume.size.x - 1 );

                for( unsigned int z = 0; z < unsigned( volume.size.z ); ++z )
                    for( unsigned int y = 0; y < unsigned( volume.size.y ); ++y )
                    {
                        sum += volume( x, y, z );
                    }
    }

    sum /= 2 *
        ( volume.size.x * volume.size.y
        + volume.size.x * volume.size.z
        + volume.size.z * volume.size.y );

    return static_cast< int >( sum ) / 100;
}


Scene* SceneFactory::createFromVolumeMasking( const Scene& baseModel, const BinarizedScalarField& mask )
{
    class MaskAdapter : public FactorizedScalarField
    {

    public:

        const BinarizedScalarField& mask;

        MaskAdapter( const BinarizedScalarField& mask )
            : mask( mask )
        {
        }

        virtual float operator()( unsigned int x
                                , unsigned int y
                                , unsigned int z ) const override
        {
            return mask( x, y, z ) ? 1.f : 0.f;
        }

        virtual float operator()( const Vector3ui& at ) const override
        {
            return mask( at ) ? 1.f : 0.f;
        }

    }; // MaskAdapter

    return createFromVolumeMasking( baseModel, MaskAdapter( mask ) );
}


Scene* SceneFactory::createFromVolumeMasking( const Scene& baseModel, const FactorizedScalarField& mask )
{
    UInt16Volume::BufferType& data = *( new UInt16Volume::BufferType
        ( baseModel.volume().size.x
        * baseModel.volume().size.y
        * baseModel.volume().size.z ) );

 // parallel execution

    const unsigned int num_workers = getWorkerCount( parallelizationSettings );
    Parallelization< UInt16VolumeMaskingWorker > parallel_job;

    for( unsigned int i = 0; i < num_workers; ++i )
    {
        parallel_job.putWorker( new UInt16VolumeMaskingWorker( baseModel.volume(), mask, data ) );
    }

    configureWorkers( parallel_job, baseModel.volume().size );

    parallel_job.run( "Masking...", parallelizationSettings.priority, dialogParent );

 // -----------------------------------------------------------------

 // create volume

    UInt16Volume* const volume = new UInt16Volume( baseModel.volume().size, new Composition< UInt16Volume::BufferType >( &data ) );

 // create model

    Scene* const model = new Scene( new Composition< Volume >( volume )
                                  , baseModel.spacingX()
                                  , baseModel.spacingY()
                                  , baseModel.spacingZ() );

 // -----------------------------------------------------------------

 // sum cube faces' intensity

    model->setRecommendedVoidThreshold( computeVoidThreshold( model->volume() ) );

 // -----------------------------------------------------------------

 // fetch recommended windowing settings

    model->setRecommendedWindowingLevel( baseModel.recommendedWindowingLevel() );
    model->setRecommendedWindowingWidth( baseModel.recommendedWindowingWidth() );

 // finish

    return model;
}


Scene* SceneFactory::createFromUserInput()
{
    qt::DicomOpeningDialog openingDialog( dialogParent );

    if( openingDialog.exec() == QDialog::Accepted )
    {
        return createFromRequest( openingDialog.getRequest() );
    }
    else
    {
        return nullptr;
    }
}


Scene* SceneFactory::createFromRequest( const SeriesLoadingRequest& request )
{
    DicomSeries ds;

    ds.setSpacingZ( 0 );

    QStringList sFilenameList;
    for( auto element_itr  = request.getSeries().getElements().begin();
              element_itr != request.getSeries().getElements().end();
            ++element_itr )
    {
        const SeriesElement& element = **element_itr;
        sFilenameList.push_back( QString::fromStdString( element.fileName ) );
    }
    
 // -----------------------------------------------------------------

 // create worker threads

    const unsigned int numWorkers = std::min( unsigned( sFilenameList.size() ), unsigned( parallelizationSettings.threads ) );
    const unsigned int filesPerWorker = sFilenameList.size() / numWorkers;
    Parallelization< SeriesLoadingWorker > parallel_job;
    for( unsigned int i = 0; i < numWorkers; ++i )
    {
        const unsigned int first = i * filesPerWorker;
        const unsigned int count = ( i + 1 == numWorkers ? sFilenameList.size() - first : filesPerWorker );
        parallel_job.putWorker( new SeriesLoadingWorker( sFilenameList, first, count ) );
    }

 // run workers

    if( !parallel_job.run( "Opening...", parallelizationSettings.priority, dialogParent ) )
    {
        return nullptr; // canceled by user
    }

 // fetch results

    for( unsigned int i = 0; i < numWorkers; ++i )
    {
        SeriesLoadingWorker& worker = parallel_job.workerByIndex( i );

        const SeriesLoadingWorker::DicomImageVector& images = worker.getResult();
        for( auto it = images.begin(); it != images.end(); ++it )
        {
            DicomImage* const dicomImage = *it;

            if( ds.size() == 0 )
            {
                ds.setWidth( dicomImage->getWidth() );
                ds.setHeight( dicomImage->getHeight() );
                ds.setSpacingXY( dicomImage->getSpacingXY() );
            }
            else
            if( ds.size() == 1 )
            {
                ds.setSpacingZ( dicomImage->getPositionZ() - ds.getImage( 0 )->getPositionZ() );
                if( ds.spacingZ() == 0 )
                {
                    QMessageBox::warning( dialogParent, "DICOM Data", "Z-Position of DICOM images is not set. This might cause serious problems with most views." );
                    ds.setSpacingZ( 1.f );
                }
            }
            if( ds.width() != dicomImage->getWidth()
             || ds.height() != dicomImage->getHeight() )
            {
                ds.deleteAll();

                throw std::runtime_error( "Inconsistent image size." );
            }
            ds.addImage( dicomImage );
        }
    }

 // make spacing positive

    ds.setSpacingXY( std::abs( ds.spacingXY() ) );
    ds.setSpacingZ ( request.getSpacingZ() );
    
 // -----------------------------------------------------------------

 // compute required volume size

    const Vector3ui full_size( ds.width(), ds.height(), ds.size() );
    const unsigned int longest_side = std::max( full_size.x, std::max( full_size.y, full_size.z ) );

    Vector3ui used_size = full_size;
    if( longest_side > max_allowed_side_length )
    {
        used_size.x = ( used_size.x * max_allowed_side_length ) / longest_side;
        used_size.y = ( used_size.y * max_allowed_side_length ) / longest_side;
        used_size.z = ( used_size.z * max_allowed_side_length ) / longest_side;

        const double axis_scaling_factor = static_cast< double >( longest_side ) / max_allowed_side_length;

        ds.setSpacingXY( ds.spacingXY() * axis_scaling_factor );
        ds.setSpacingZ( ds.spacingZ() * axis_scaling_factor );
    }

 // resize if necessary and create scene object

    Scene* const model = createFromDicomSeries( ds, used_size );
    
    ds.deleteAll();

    return model;
}


Scene* SceneFactory::createFromDicomSeries( const DicomSeries& dicomSeries
                                          , const Vector3ui& size )
{
    UInt16Volume::BufferType& data = *( new UInt16Volume::BufferType( size.x * size.y * size.z ) );

    DicomSeries& ds = const_cast< DicomSeries& >( dicomSeries );
    const Vector3ui src_size( ds.width(), ds.height(), ds.images()->size() );

    CARNA_ASSERT_EX
        ( size.x >= 2 && size.y >= 2 && size.z >= 2
        , "DICOM data set with less than 2 slices or pixels per image axis not supported" );

    CARNA_ASSERT_EX
        ( size.x <= src_size.x && size.y <= src_size.y && size.z <= src_size.z
        , "destination image can't be larger than source data" );

    const bool down_sampling = size.x < src_size.x
                            || size.y < src_size.y 
                            || size.z < src_size.z;

    const double resolution_reduction = 1 - ( 1. * size.x * size.y * size.z ) / ( src_size.x * src_size.y * src_size.z );

 // parallel execution

    const unsigned int num_workers = getWorkerCount( parallelizationSettings );
    Parallelization< UInt16VolumeComputationWorker > parallel_job;

    for( unsigned int i = 0; i < num_workers; ++i )
    {
        parallel_job.putWorker( new UInt16VolumeComputationWorker( ds, size, data ) );
    }

    configureWorkers( parallel_job, size );

    std::stringstream ss_hint_resample;
    ss_hint_resample << "Resampling volume...\nReducing resolution by " << ( 100 * resolution_reduction ) << " %";
    const std::string hint = down_sampling ? ss_hint_resample.str() : "Sampling volume...";
    parallel_job.run( hint, parallelizationSettings.priority, dialogParent );
    
 // -----------------------------------------------------------------

 // create volume

    UInt16Volume* const volume = new UInt16Volume( size, new Composition< UInt16Volume::BufferType >( &data ) );

 // create model

    Scene* const model = new Scene( new Composition< Volume >( volume )
                                  , ds.spacingXY()
                                  , ds.spacingXY()
                                  , ds.spacingZ () );
    
 // -----------------------------------------------------------------

 // sum cube faces' intensity

    model->setRecommendedVoidThreshold( computeVoidThreshold( model->volume() ) );
    
 // -----------------------------------------------------------------

 // fetch recommended windowing settings

    model->setRecommendedWindowingLevel( ds.getImage( 0 )->getRecommendedWindowLevel() );
    model->setRecommendedWindowingWidth( ds.getImage( 0 )->getRecommendedWindowWidth() );

 // finish

    return model;
}



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna
