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

#ifndef SAMPLER_H_6014714286
#define SAMPLER_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>

/** \file   Sampler.h
  * \brief  Defines \ref Carna::base::view::Sampler.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Sampler
// ----------------------------------------------------------------------------------

class CARNA_LIB Sampler
{

    NON_COPYABLE

public:

    const static unsigned int WRAP_MODE_CLAMP;
    const static unsigned int WRAP_MODE_REPEAT;
    const static unsigned int WRAP_MODE_MIRRORED_REPEAT;

    const static unsigned int FILTER_NEAREST;
    const static unsigned int FILTER_LINEAR;

    Sampler();

    ~Sampler();

    const unsigned int id;

    void bind( int unit ) const;

    void setWrapModeS( unsigned int wrap_mode );
    void setWrapModeT( unsigned int wrap_mode );
    void setWrapModeR( unsigned int wrap_mode );

    void setMinFilter( unsigned int min_filter );
    void setMagFilter( unsigned int mag_filter );

}; // Sampler



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // SAMPLER_H_6014714286
