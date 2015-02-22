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

#ifndef BUFFEREDHUVOLUMETEXTURE_H_6014714286
#define BUFFEREDHUVOLUMETEXTURE_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/model/UInt16HUVolume.h>
#include <Carna/base/CarnaException.h>

/** \file   BufferedHUVolumeTexture.h
  * \brief  Defines \ref Carna::base::view::BufferedHUVolumeTexture.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// BufferedHUVolumeUploader
// ----------------------------------------------------------------------------------

template< typename BufferedHUVolume >
struct BufferedHUVolumeUploader
{

    static void upload( const BufferedHUVolume& src, Texture3D& dst )
    {
        CARNA_FAIL( "Upload routine not implemented for this buffer type." );
    }

}; // BufferedHUVolumeUploader


void uploadUInt16HUVolume( const model::UInt16HUVolume& src, Texture3D& dst );


template< >
struct CARNA_LIB BufferedHUVolumeUploader< model::UInt16HUVolume >
{

    static void upload( const model::UInt16HUVolume& src, Texture3D& dst )
    {
        uploadUInt16HUVolume( src, dst );
    }

}; // BufferedHUVolumeUploader< UInt16HUVolume >



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // BUFFEREDHUVOLUMETEXTURE_H_6014714286
