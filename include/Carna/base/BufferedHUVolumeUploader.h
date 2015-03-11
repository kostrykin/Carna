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

#ifndef BUFFEREDHUVOLUMEUPLOADER_H_6014714286
#define BUFFEREDHUVOLUMEUPLOADER_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/UInt16HUVolume.h>
#include <Carna/base/CarnaException.h>

/** \file   BufferedHUVolumeUploader.h
  * \brief  Defines \ref Carna::base::uploadBufferedHUVolume.
  * \todo	Rename this file.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BufferedHUVolumeUploader
// ----------------------------------------------------------------------------------

void CARNA_LIB uploadBufferedHUVolume( const UInt16HUVolume& src, Texture3D& dst );



}  // namespace Carna :: base

}  // namespace Carna

#endif // BUFFEREDHUVOLUMEUPLOADER_H_6014714286
