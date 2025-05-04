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

#include <LibCarna/base/glew.h>
#include <LibCarna/base/glError.h>
#include <LibCarna/base/Sampler.h>
#include <LibCarna/base/Texture.h>
#include <LibCarna/base/CarnaException.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// validateWrapMode
// ----------------------------------------------------------------------------------

static inline void validateWrapMode( unsigned int wrapMode )
{
    LIBCARNA_ASSERT_EX
        (  wrapMode == Sampler::WRAP_MODE_CLAMP
        || wrapMode == Sampler::WRAP_MODE_REPEAT
        || wrapMode == Sampler::WRAP_MODE_MIRRORED_REPEAT
        , "Unknown wrap mode." );
}



// ----------------------------------------------------------------------------------
// validateTextureFilter
// ----------------------------------------------------------------------------------

static inline void validateTextureFilter( unsigned int filter )
{
    LIBCARNA_ASSERT_EX
        (  filter == Sampler::FILTER_NEAREST
        || filter == Sampler::FILTER_LINEAR
        , "Unknown texture filter." );
}



// ----------------------------------------------------------------------------------
// Sampler
// ----------------------------------------------------------------------------------

const unsigned int Sampler::WRAP_MODE_CLAMP           = GL_CLAMP_TO_EDGE;
const unsigned int Sampler::WRAP_MODE_REPEAT          = GL_REPEAT;
const unsigned int Sampler::WRAP_MODE_MIRRORED_REPEAT = GL_MIRRORED_REPEAT;

const unsigned int Sampler::FILTER_NEAREST = GL_NEAREST;
const unsigned int Sampler::FILTER_LINEAR  = GL_LINEAR;


static unsigned int createGLSampler()
{
    unsigned int id;
    glGenSamplers( 1, &id );
    return id;
}


Sampler::Sampler
        ( unsigned int wrapModeS
        , unsigned int wrapModeT
        , unsigned int wrapModeR
        , unsigned int minFilter
        , unsigned int magFilter )
    : id( createGLSampler() )
{
    bind( Texture< 0 >::SETUP_UNIT );
    
    setWrapModeS( wrapModeS );
    setWrapModeT( wrapModeT );
    setWrapModeR( wrapModeR );
    
    setMinFilter( minFilter );
    setMagFilter( magFilter );
}


Sampler::~Sampler()
{
    glDeleteSamplers( 1, &id );
}


void Sampler::bind( int unit ) const
{
    glBindSampler( unit, id );
}


void Sampler::setWrapModeS( unsigned int wrapMode )
{
    validateWrapMode( wrapMode );
    glSamplerParameteri( id, GL_TEXTURE_WRAP_S, wrapMode );
}


void Sampler::setWrapModeT( unsigned int wrapMode )
{
    validateWrapMode( wrapMode );
    glSamplerParameteri( id, GL_TEXTURE_WRAP_T, wrapMode );
}


void Sampler::setWrapModeR( unsigned int wrapMode )
{
    validateWrapMode( wrapMode );
    glSamplerParameteri( id, GL_TEXTURE_WRAP_R, wrapMode );
}


void Sampler::setMinFilter( unsigned int minFilter )
{
    validateTextureFilter( minFilter );
    glSamplerParameteri( id, GL_TEXTURE_MIN_FILTER, minFilter );
}


void Sampler::setMagFilter( unsigned int magFilter )
{
    validateTextureFilter( magFilter );
    glSamplerParameteri( id, GL_TEXTURE_MAG_FILTER, magFilter );
}



}  // namespace Carna :: base

}  // namespace Carna
