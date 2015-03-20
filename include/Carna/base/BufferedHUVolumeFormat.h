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

/** \file   BufferedHUVolumeFormat.h
  * \brief  Defines \ref Carna::base::BufferedHUVolumeFormat.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BufferedHUVolumeFormat
// ----------------------------------------------------------------------------------

/** \brief
  * Maps \ref BufferedHUVolume class template instances to \ref Texture3D formats.
  */
template< typename BufferedHUVolumeType >
struct BufferedHUVolumeFormat
{
    static_assert( sizeof( BufferedHUVolumeType ) == -1, "Unknown BufferedHUVolumeType." );
};


/** \brief
  * Defines \ref Texture3D format for \ref UInt16HUVolume.
  */
template< >
struct CARNA_LIB BufferedHUVolumeFormat< UInt16HUVolume >
{
    const static unsigned int INTERNAL_FORMAT;
    const static unsigned int BUFFER_TYPE;
};



}  // namespace Carna :: base

}  // namespace Carna

#endif // BUFFEREDHUVOLUMEUPLOADER_H_6014714286
