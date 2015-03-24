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

#include <Carna/base/glew.h>
#include <Carna/base/BufferedHUVolumeFormat.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BufferedHUVolumeFormat
// ----------------------------------------------------------------------------------

const unsigned int BufferedHUVolumeFormat< UInt16HUVolume >::INTERNAL_FORMAT = GL_INTENSITY16;
const unsigned int BufferedHUVolumeFormat< UInt16HUVolume >::BUFFER_TYPE     = GL_UNSIGNED_SHORT;



}  // namespace Carna :: base

}  // namespace Carna
