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

#ifndef VIEWPORT_H_6014714286
#define VIEWPORT_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>

/** \file   Viewport.h
  * \brief  Defines \ref Carna::base::view::Viewport.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Viewport
// ----------------------------------------------------------------------------------

class CARNA_LIB Viewport
{

    NON_COPYABLE

    const Viewport* parent;

public:

    Viewport( const FrameRenderer& fr, bool fitSquare );
    
    Viewport( const Viewport& parent, unsigned int left, unsigned int top, unsigned int width, unsigned int height );

    Viewport( const Viewport& parent, float width, float height, float left, float top );

    unsigned int left;

    unsigned int top;

    unsigned int width;

    unsigned int height;

    void makeActive() const;

    void done() const;

}; // Viewport



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // VIEWPORT_H_6014714286
