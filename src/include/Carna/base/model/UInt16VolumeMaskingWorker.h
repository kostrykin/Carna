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

#ifndef UINT16VOLUMEMASKINGWORKER_H_6014714286
#define UINT16VOLUMEMASKINGWORKER_H_6014714286

#include <Carna/base/model/UInt16VolumeBaseWorker.h>

namespace Carna
{

namespace base
{

namespace model
{



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

#endif // UINT16VOLUMEMASKINGWORKER_H_6014714286
