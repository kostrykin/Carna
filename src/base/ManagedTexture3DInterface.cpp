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

#include <Carna/base/ManagedTexture3DInterface.h>
#include <Carna/base/ManagedTexture3D.h>
#include <Carna/base/Texture.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ManagedTexture3DInterface
// ----------------------------------------------------------------------------------

ManagedTexture3DInterface::ManagedTexture3DInterface( ManagedTexture3D& managed )
    : GeometryFeature::ManagedInterface( managed )
    , managed( managed )
{
    if( managed.videoResourceAcquisitionsCount() == 1 )
    {
        managed.textureObject.reset( new Texture< 3 >
            ( managed.size
            , managed.internalFormat
            , managed.pixelFormat
            , managed.bufferType
            , managed.bufferPtr ) );
    }
}


ManagedTexture3DInterface::~ManagedTexture3DInterface()
{
    if( managed.videoResourceAcquisitionsCount() == 1 )
    {
        managed.textureObject.reset();
    }
}


const Texture< 3 >& ManagedTexture3DInterface::get() const
{
    return *managed.textureObject;
}



}  // namespace Carna :: base

}  // namespace Carna
