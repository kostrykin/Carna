/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include <LibCarna/base/glew.hpp>
#include <LibCarna/base/BufferedVectorFieldFormat.hpp>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BufferedVectorFieldFormat
// ----------------------------------------------------------------------------------

const unsigned int BufferedVectorFieldFormat< IntensityVolumeUInt16 >::INTERNAL_FORMAT = GL_INTENSITY16;
const unsigned int BufferedVectorFieldFormat< IntensityVolumeUInt16 >::PIXEL_FORMAT    = GL_RED;
const unsigned int BufferedVectorFieldFormat< IntensityVolumeUInt16 >::BUFFER_TYPE     = GL_UNSIGNED_SHORT;

const unsigned int BufferedVectorFieldFormat< IntensityVolumeUInt8 >::INTERNAL_FORMAT = GL_INTENSITY8;
const unsigned int BufferedVectorFieldFormat< IntensityVolumeUInt8 >::PIXEL_FORMAT    = GL_RED;
const unsigned int BufferedVectorFieldFormat< IntensityVolumeUInt8 >::BUFFER_TYPE     = GL_UNSIGNED_BYTE;

const unsigned int BufferedVectorFieldFormat< HUVolumeUInt16 >::INTERNAL_FORMAT = GL_INTENSITY16;
const unsigned int BufferedVectorFieldFormat< HUVolumeUInt16 >::PIXEL_FORMAT    = GL_RED;
const unsigned int BufferedVectorFieldFormat< HUVolumeUInt16 >::BUFFER_TYPE     = GL_UNSIGNED_SHORT;

const unsigned int BufferedVectorFieldFormat< NormalMap3DInt8 >::INTERNAL_FORMAT = GL_RGBA8_SNORM;
const unsigned int BufferedVectorFieldFormat< NormalMap3DInt8 >::PIXEL_FORMAT    = GL_RGBA;
const unsigned int BufferedVectorFieldFormat< NormalMap3DInt8 >::BUFFER_TYPE     = GL_BYTE;



}  // namespace LibCarna :: base

}  // namespace LibCarna
