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
#include <Carna/base/Texture3D.h>

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
        managed.textureObject.reset( new Texture3D
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


unsigned int ManagedTexture3DInterface::id() const
{
    return managed.textureObject->id;
}


void ManagedTexture3DInterface::bind( unsigned int unit ) const
{
    managed.textureObject->bind( unit );
}



}  // namespace Carna :: base

}  // namespace Carna
