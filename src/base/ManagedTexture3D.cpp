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

#include <Carna/base/ManagedTexture3D.h>
#include <Carna/base/Texture3D.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ManagedTexture3D
// ----------------------------------------------------------------------------------

ManagedTexture3D::ManagedTexture3D( Texture3D& manager )
    : GeometryFeature::VideoResourceAcquisition( manager )
    , manager( manager )
{
    if( manager.videoResourceAcquisitionsCount() == 1 )
    {
        manager.textureObject.reset( new Texture3DObject
            ( manager.size
            , manager.internalFormat
            , manager.pixelFormat
            , manager.bufferType
            , manager.bufferPtr ) );
    }
}


ManagedTexture3D::~ManagedTexture3D()
{
    if( manager.videoResourceAcquisitionsCount() == 1 )
    {
        manager.textureObject.reset();
    }
}


unsigned int ManagedTexture3D::id() const
{
    return manager.textureObject->id;
}


void ManagedTexture3D::bind( unsigned int unit ) const
{
    manager.textureObject->bind( unit );
}



}  // namespace Carna :: base

}  // namespace Carna
