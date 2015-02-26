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
#include <Carna/base/view/glError.h>
#include <Carna/base/view/Sampler.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace base
{

namespace view
{



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


Sampler::Sampler()
    : id( createGLSampler() )
{
}


Sampler::~Sampler()
{
    glDeleteSamplers( 1, &id );
}


void Sampler::bind( int unit ) const
{
    glBindSampler( unit, id );
}


void Sampler::setWrapModeS( unsigned int wrap_mode )
{
    glSamplerParameteri( id, GL_TEXTURE_WRAP_S, wrap_mode );
}


void Sampler::setWrapModeT( unsigned int wrap_mode )
{
    glSamplerParameteri( id, GL_TEXTURE_WRAP_T, wrap_mode );
}


void Sampler::setWrapModeR( unsigned int wrap_mode )
{
    glSamplerParameteri( id, GL_TEXTURE_WRAP_R, wrap_mode );
}


void Sampler::setMinFilter( unsigned int min_filter )
{
    glSamplerParameteri( id, GL_TEXTURE_MIN_FILTER, min_filter );
}


void Sampler::setMagFilter( unsigned int mag_filter )
{
    glSamplerParameteri( id, GL_TEXTURE_MAG_FILTER, mag_filter );
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
