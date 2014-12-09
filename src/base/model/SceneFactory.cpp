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

#include <Carna/base/model/Scene.h>
#include <Carna/base/model/SceneFactory.h>
#include <Carna/base/model/Volume.h>
#include <Carna/base/model/UInt16VolumeMaskingWorker.h>
#include <Carna/base/qt/CarnaProgressDialog.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Parallelization.h>
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

    const unsigned int num_workers = getWorkerCount();
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


unsigned int SceneFactory::getWorkerCount()
{
    switch( parallelizationSettings.threads )
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



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna
