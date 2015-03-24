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
#include <Carna/base/glError.h>
#include <Carna/base/Texture3DObject.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/text.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// createGLTextureObject
// ----------------------------------------------------------------------------------

static unsigned int createGLTextureObject()
{
    unsigned int id;
    glGenTextures( 1, &id );
    return id;
}



// ----------------------------------------------------------------------------------
// Texture3DObject
// ----------------------------------------------------------------------------------

Texture3DObject::Texture3DObject
        ( const math::Vector3ui& size
        , int internalFormat
        , int pixelFormat
        , int bufferType
        , const void* bufferPtr )
    : id( createGLTextureObject() )
    , size( size )
    , internalFormat( internalFormat )
    , pixelFormat( pixelFormat )
{
    try
    {
        CARNA_ASSERT_EX( id != 0, "Texture3DObject acquisition failed." );
        CARNA_ASSERT_EX
            ( size.x() % 2 == 0 && size.y() % 2 == 0 && size.z() % 2 == 0
            , "Texture3DObject only supports even sizes." );
        
        /* Upload texture data.
         */
        bind( SETUP_UNIT );
        glTexImage3D( GL_TEXTURE_3D, 0, internalFormat
                    , size.x(), size.y(), size.z()
                    , 0, pixelFormat, bufferType, bufferPtr );

        const unsigned int err = glGetError();
        if( err != GL_NO_ERROR )
        {
            std::string err_msg;
            switch( err )
            {
                case GL_INVALID_ENUM:
                    err_msg = "invalid enumerator";
                    break;

                case GL_INVALID_VALUE:
                    err_msg = "invalid value";
                    break;

                case GL_INVALID_OPERATION:
                    err_msg = "invalid operation";
                    break;

                case GL_OUT_OF_MEMORY:
                    err_msg = "out of memory";
                    break;

                default:
                    err_msg = base::text::lexical_cast< std::string >( err );
            }

            std::stringstream ss;
            ss << "OpenGL 3D texture object acquisition failed: "
                << err_msg;

            CARNA_FAIL( ss.str() );
        }
    }
    catch( ... )
    {
        cleanup();
        throw;
    }
}


Texture3DObject::~Texture3DObject()
{
    cleanup();
}


void Texture3DObject::cleanup()
{
    glDeleteTextures( 1, &id );
}


void Texture3DObject::bind( unsigned int unit ) const
{
    glActiveTexture( GL_TEXTURE0 + unit );
    glBindTexture( GL_TEXTURE_3D, id );
}



}  // namespace Carna :: base

}  // namespace Carna
