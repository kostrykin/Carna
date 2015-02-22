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

#include <Carna/base/view/glew.h>
#include <Carna/base/view/BufferedHUVolumeUploader.h>
#include <Carna/base/view/Texture3D.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// BufferedHUVolumeUploader
// ----------------------------------------------------------------------------------

void uploadUInt16HUVolume( const model::UInt16HUVolume& src, Texture3D& dst )
{
    dst.upload( GL_INTENSITY16, src.size, GL_RED, GL_UNSIGNED_SHORT, &src.buffer().front() );
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
