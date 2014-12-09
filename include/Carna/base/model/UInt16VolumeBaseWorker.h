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

#ifndef UINT16VOLUMEBASEWORKER_H_6014714286
#define UINT16VOLUMEBASEWORKER_H_6014714286

#include <QThread>
#include <vector>
#include <Carna/base/noncopyable.h>
#include <Carna/base/model/UInt16Volume.h>
#include <Carna/base/model/SceneFactory.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// UInt16VolumeBaseWorker
// ----------------------------------------------------------------------------------

class CARNA_EXPORT UInt16VolumeBaseWorker : public QObject
{

    Q_OBJECT

    NON_COPYABLE

public:

    typedef std::vector< UInt16Volume::VoxelType > DestinationBuffer;


    struct Setup
    {

        const Vector3ui firstVoxel;
        const Vector3ui lastVoxel;

        Setup( const Vector3ui& firstVoxel, const Vector3ui& lastVoxel )
            : firstVoxel( firstVoxel )
            , lastVoxel( lastVoxel )
        {
        }

    }; // Setup


    explicit UInt16VolumeBaseWorker( DestinationBuffer& dst );


    /** \brief  Returns whether current job has been aborted.
      */
    inline bool isCanceled() const
    {
        return canceled;
    }

    int getWorkAmount() const
    {
        CARNA_ASSERT( setup.get() != nullptr );

        return setup->lastVoxel.z - setup->firstVoxel.z;
    }

    const Setup& getSetup() const
    {
        return *setup;
    }

    unsigned int array_element_index( const Vector3ui& size, unsigned int x, unsigned int y, unsigned int z ) const;


public slots:

    /** \brief  Performs the current job.
      */
    virtual void run() = 0;

    /** \brief  Aborts the current job.
      */
    inline void cancel()
    {
        canceled = true;
    }

    void setSetup( const Carna::base::model::UInt16VolumeBaseWorker::Setup* setup )
    {
        this->setup.reset( setup );
    }


signals:

    /** \brief  Emitted when job finishes a z-slice.
      *
      * \param  amount  is the number of recently finished slices.
      */
    void progressed( int amount );

    /** \brief  Emitted when job has finished.
      */
    void finished();


private:

    /** \brief  Holds whether the current job has been aborted.
      */
    bool canceled;

    std::unique_ptr< const Setup > setup;


protected:

    DestinationBuffer& dst;

}; // UInt16VolumeBaseWorker



// ----------------------------------------------------------------------------------
// configureWorkers
// ----------------------------------------------------------------------------------

/** \relates UInt16VolumeBaseWorker
  */
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



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // UINT16VOLUMEBASEWORKER_H_6014714286
