/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#ifndef UINT16HUVOLUME_H_6014714286
#define UINT16HUVOLUME_H_6014714286

/** \file   UInt16HUVolume.h
  *
  * \brief  Defines \ref Carna::base::UInt16HUVolume.
  *
  * \author Leonid Kostrykin
  * \date   25.7.11
  */

#include <Carna/base/BufferedHUVolume.h>
#include <Carna/base/HUVolumeGrid.h>
#include <Carna/base/HUVolumeSegment.h>
#include <cstdint>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// UInt16HUVolume
// ----------------------------------------------------------------------------------

/** \brief  Defines 16bit unsigned integer volume data.
  */
typedef BufferedHUVolume< uint16_t > UInt16HUVolume;

typedef HUVolumeGrid< UInt16HUVolume > UInt16HUVolumeGrid;

typedef HUVolumeSegment< UInt16HUVolumeGrid, UInt16HUVolume > UInt16HUVolumeSegment;



}  // namespace Carna :: base

}  // namespace Carna

#endif // UINT16HUVOLUME_H_6014714286
