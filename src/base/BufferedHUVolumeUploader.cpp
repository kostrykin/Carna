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
#include <Carna/base/BufferedHUVolumeUploader.h>
#include <Carna/base/Texture3D.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// uploadBufferedHUVolume
// ----------------------------------------------------------------------------------

void uploadBufferedHUVolume( const UInt16HUVolume& src, Texture3D& dst )
{
    dst.upload( GL_INTENSITY16, src.size, GL_RED, GL_UNSIGNED_SHORT, &src.buffer().front() );
}



}  // namespace Carna :: base

}  // namespace Carna
