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

#include <Carna/base/model/UInt16VolumeBaseWorker.h>
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



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna
