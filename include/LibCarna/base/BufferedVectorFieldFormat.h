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

#ifndef BUFFEREDVECTORFIELDFORMAT_H_6014714286
#define BUFFEREDVECTORFIELDFORMAT_H_6014714286

#include <LibCarna/LibCarna.h>

/** \file   BufferedVectorFieldFormat.h
  * \brief  Defines \ref Carna::base::BufferedVectorFieldFormat.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BufferedVectorFieldFormat
// ----------------------------------------------------------------------------------

/** \brief
  * Maps \ref math::VectorField implementations to \ref Texture formats.
  */
template< typename BufferedVectorFieldType >
struct BufferedVectorFieldFormat
{
    static_assert( sizeof( BufferedVectorFieldType ) == -1, "Unknown BufferedVectorFieldType." );
};


/** \brief
  * Defines \ref Texture format for \ref IntensityVolumeUInt16.
  * 
  * \since \ref v_3_2_0
  */
template< >
struct LIBCARNA BufferedVectorFieldFormat< IntensityVolumeUInt16 >
{
    const static unsigned int INTERNAL_FORMAT; ///< \copydoc ManagedTexture3D::internalFormat
    const static unsigned int PIXEL_FORMAT;    ///< \copydoc ManagedTexture3D::pixelFormat
    const static unsigned int BUFFER_TYPE;     ///< \copydoc ManagedTexture3D::bufferType
};


/** \brief
  * Defines \ref Texture format for \ref IntensityVolumeUInt8.
  * 
  * \since \ref v_3_2_0
  */
template< >
struct LIBCARNA BufferedVectorFieldFormat< IntensityVolumeUInt8 >
{
    const static unsigned int INTERNAL_FORMAT; ///< \copydoc ManagedTexture3D::internalFormat
    const static unsigned int PIXEL_FORMAT;    ///< \copydoc ManagedTexture3D::pixelFormat
    const static unsigned int BUFFER_TYPE;     ///< \copydoc ManagedTexture3D::bufferType
};


/** \brief
  * Defines \ref Texture format for \ref HUVolumeUInt16.
  */
template< >
struct LIBCARNA BufferedVectorFieldFormat< HUVolumeUInt16 >
{
    const static unsigned int INTERNAL_FORMAT; ///< \copydoc ManagedTexture3D::internalFormat
    const static unsigned int PIXEL_FORMAT;    ///< \copydoc ManagedTexture3D::pixelFormat
    const static unsigned int BUFFER_TYPE;     ///< \copydoc ManagedTexture3D::bufferType
};


/** \brief
  * Defines \ref Texture format for \ref NormalMap3DInt8.
  */
template< >
struct LIBCARNA BufferedVectorFieldFormat< NormalMap3DInt8 >
{
    const static unsigned int INTERNAL_FORMAT; ///< \copydoc ManagedTexture3D::internalFormat
    const static unsigned int PIXEL_FORMAT;    ///< \copydoc ManagedTexture3D::pixelFormat
    const static unsigned int BUFFER_TYPE;     ///< \copydoc ManagedTexture3D::bufferType
};



}  // namespace Carna :: base

}  // namespace Carna

#endif // BUFFEREDVECTORFIELDFORMAT_H_6014714286
