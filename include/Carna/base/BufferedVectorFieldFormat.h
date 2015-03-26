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

#include <Carna/Carna.h>

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
  * Defines \ref Texture format for \ref HUVolumeUInt16.
  */
template< >
struct CARNA_LIB BufferedVectorFieldFormat< HUVolumeUInt16 >
{
    const static unsigned int INTERNAL_FORMAT;
    const static unsigned int PIXEL_FORMAT;
    const static unsigned int BUFFER_TYPE;
};


/** \brief
  * Defines \ref Texture format for \ref NormalMap3DInt8.
  */
template< >
struct CARNA_LIB BufferedVectorFieldFormat< NormalMap3DInt8 >
{
    const static unsigned int INTERNAL_FORMAT;
    const static unsigned int PIXEL_FORMAT;
    const static unsigned int BUFFER_TYPE;
};



}  // namespace Carna :: base

}  // namespace Carna

#endif // BUFFEREDVECTORFIELDFORMAT_H_6014714286
