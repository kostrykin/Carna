/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/base/view/glew.h>
#include <Carna/base/view/glError.h>
#include <Carna/base/view/SceneProviderDetails.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/model/BufferedMask.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Text.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// uploadMaskTexture
// ----------------------------------------------------------------------------------

void uploadMaskTexture
    ( const Vector3ui& size
    , unsigned int internalFormat
    , unsigned int type
    , const void* buffer
    , float scale )
{
    REPORT_GL_ERROR;

    glPixelTransferf( GL_RED_SCALE, scale );

    glTexImage3D( GL_TEXTURE_3D, 0, internalFormat
                , size.x, size.y, size.z
                , 0, GL_RED, type, buffer );

    glPixelTransferf( GL_RED_SCALE, 1.f );

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
                err_msg = base::Text::lexical_cast< std::string >( err );
        }

        std::stringstream ss;
        ss << "3D texture acquisition failed: "
           << err_msg;

        CARNA_FAIL( ss.str() );
    }
}



// ----------------------------------------------------------------------------------
// SceneProvider :: Details
// ----------------------------------------------------------------------------------

SceneProvider::Details::Details( SceneProvider& self )
    : self( self )
    , volumeTextureReferenceCounter( 0 )
    , maskTextureID( 0 )
    , maskTextureReferenceCounter( 0 )
    , maskTextureInvalidated( false )
{
}


SceneProvider::Details::~Details()
{
    CARNA_ASSERT_EX
        ( volumeTextureReferenceCounter == 0
        , "SceneProvider::Details has been destroyed having volume texture still acquired" );
}


unsigned int SceneProvider::Details::acquireVolumeTexture()
{
    CARNA_ASSERT( ( volumeTexture.get() != nullptr ) == ( volumeTextureReferenceCounter > 0 ) );

    if( volumeTexture.get() == nullptr )
    {
        volumeTexture.reset( new model::Volume::Texture( self.scene.volume() ) );
    }

    ++volumeTextureReferenceCounter;

    return volumeTexture->textureID;
}


void SceneProvider::Details::releaseVolumeTexture()
{
    CARNA_ASSERT( volumeTexture.get() != nullptr && volumeTextureReferenceCounter > 0 );

    CARNA_ASSERT_EX( volumeTextureReferenceCounter > 0, "volume texture reference counter already zero" );

    if( --volumeTextureReferenceCounter == 0 )
    {
        volumeTexture.reset();
    }
}


unsigned int SceneProvider::Details::acquireMaskTexture()
{
    CARNA_ASSERT( ( maskTextureID == 0 && maskTextureReferenceCounter == 0 ) || ( maskTextureID > 0 && maskTextureReferenceCounter > 0 ) );

    ++maskTextureReferenceCounter;

    if( maskTextureID == 0 )
    {
        glGenTextures( 1, &maskTextureID );

        CARNA_ASSERT( maskTextureID > 0 );

        glBindTexture( GL_TEXTURE_3D, maskTextureID );

        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

        updateMaskTexture();

        connect( &self.scene, SIGNAL( volumeMaskExchanged() ), this, SLOT( invalidateMaskTexture() ) );
    }

    return maskTextureID;
}


void SceneProvider::Details::releaseMaskTexture()
{
    CARNA_ASSERT( maskTextureID > 0 && maskTextureReferenceCounter > 0 );

    if( --maskTextureReferenceCounter == 0 )
    {
        glDeleteTextures( 1, &maskTextureID );
        maskTextureID = 0;
    }
}


void SceneProvider::Details::updateMaskTexture()
{
    maskTextureInvalidated = false;

    int previouslyBoundTextureID;
    glGetIntegerv( GL_TEXTURE_BINDING_3D, &previouslyBoundTextureID );
    glBindTexture( GL_TEXTURE_3D, maskTextureID );

    const static float binary_scale = 255.f;
    const static unsigned int binary_internal_format = GL_INTENSITY4;
    const static unsigned int binary_buffer_type = GL_UNSIGNED_BYTE;
    if( self.scene.hasVolumeMask() )
    {
        unsigned int bufferType, internalFormat;
        Vector3ui size;
        const void* buffer;
        float scale;

        if( self.scene.volumeMask().isBinary() )
        {
            internalFormat = binary_internal_format;
            bufferType = binary_buffer_type;
            buffer = &self.scene.volumeMask().binary().buffer.front();
            size = self.scene.volumeMask().binary().size;
            scale = binary_scale;
        }
        else
        if( self.scene.volumeMask().isFactorial() )
        {
            internalFormat = GL_R16F;
            bufferType = GL_FLOAT;
            buffer = &self.scene.volumeMask().factorial().buffer.front();
            size = self.scene.volumeMask().factorial().size;
            scale = 1.f;
        }
        else
        {
            CARNA_FAIL( "unknown mask bufferType" );
        }

        uploadMaskTexture( size, internalFormat, bufferType, buffer, scale );
    }
    else
    {
        const uint8_t constant_value = 0;
        uploadMaskTexture
            ( Vector3ui( 1, 1, 1 )
            , binary_internal_format
            , binary_buffer_type
            , &constant_value
            , binary_scale );
    }

    glBindTexture( GL_TEXTURE_3D, previouslyBoundTextureID );
}


void SceneProvider::Details::invalidateMaskTexture()
{
    maskTextureInvalidated = true;
}


bool SceneProvider::Details::isMaskTextureInvalidated() const
{
    return maskTextureInvalidated;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
