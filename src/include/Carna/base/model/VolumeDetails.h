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

#ifndef VOLUMEDETAILS_H_6014714286
#define VOLUMEDETAILS_H_6014714286

#include <QThread>
#include <vector>
#include <Carna/base/noncopyable.h>
#include <Carna/base/model/UInt16Volume.h>
#include <Carna/base/model/SceneFactory.h>
#include <Carna/base/CarnaException.h>

class DicomSeries;  // from dicom-interface

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// UInt16VolumeBaseWorker
// ----------------------------------------------------------------------------------

class UInt16VolumeBaseWorker : public QObject
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

    /** \brief	Emitted when job has finished.
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
// UInt16VolumeMaskingWorker
// ----------------------------------------------------------------------------------

class UInt16VolumeMaskingWorker : public UInt16VolumeBaseWorker
{

    Q_OBJECT

public:

    UInt16VolumeMaskingWorker( const Volume& baseVolume
                             , const SceneFactory::FactorizedScalarField& mask
                             , DestinationBuffer& dst );


public slots:

    virtual void run() override;


private:

    const Volume& baseVolume;
    const SceneFactory::FactorizedScalarField& mask;

}; // UInt16VolumeMaskingWorker



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // VOLUMEDETAILS_H_6014714286
