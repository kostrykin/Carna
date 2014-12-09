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

#include <Carna/base/model/VolumeDetails.h>
#include <Carna/base/Transformation.h>
#include <QApplication>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// UInt16VolumeBaseWorker
// ----------------------------------------------------------------------------------

UInt16VolumeBaseWorker::UInt16VolumeBaseWorker( DestinationBuffer& dst )
    : canceled( false )
    , dst( dst )
{
}


std::size_t UInt16VolumeBaseWorker::array_element_index( const Vector3ui& size, unsigned int x, unsigned int y, unsigned int z ) const
{
    const unsigned int index = x + y * size.x + z * size.x * size.y;
    CARNA_ASSERT( index < size.x * size.y * size.z );
    return index;
}



// ----------------------------------------------------------------------------------
// UInt16VolumeMaskingWorker
// ----------------------------------------------------------------------------------

UInt16VolumeMaskingWorker::UInt16VolumeMaskingWorker( const Volume& baseVolume
                                                    , const SceneFactory::FactorizedScalarField& mask
                                                    , DestinationBuffer& dst )
    : UInt16VolumeBaseWorker( dst )
    , baseVolume( baseVolume )
    , mask( mask )
{
}


void UInt16VolumeMaskingWorker::run()
{
    unsigned int progress = 0;

    const unsigned int progressEmitationThreshold = ( getSetup().lastVoxel.z - getSetup().firstVoxel.z ) / 25; //< ugly patch \o/

    for( unsigned int z = getSetup().firstVoxel.z; z <= getSetup().lastVoxel.z; ++z )
    {
        if( this->isCanceled() )
        {
            emit finished();

            return;
        }

        for( unsigned int x = getSetup().firstVoxel.x; x <= getSetup().lastVoxel.x; ++x )
        for( unsigned int y = getSetup().firstVoxel.y; y <= getSetup().lastVoxel.y; ++y )
        {
            const unsigned int position = array_element_index( baseVolume.size, x, y, z );
            const short huv0 = baseVolume( x, y, z );
            const short huv1 = static_cast< short >( ( huv0 + 1024 ) * mask( x, y, z ) ) - 1024;
            dst[ position ] = static_cast< UInt16Volume::VoxelType >( ( huv1 + 1024 ) << 4 );
        }

        ++progress;
        if( progress >= progressEmitationThreshold )
        {
            emit progressed( progress );
            progress = 0;
        }

        QApplication::processEvents();
    }

    emit finished();
}



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna
