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

#include <Carna/base/view/Viewport.h>
#include <Carna/base/view/FrameRenderer.h>
#include <Carna/base/view/glew.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// mix
// ----------------------------------------------------------------------------------

template< typename T >
T mix( const T& a, const T& b, float t )
{
    return static_cast< T >( a * ( 1 - t ) + b * t + 0.5f );
}



// ----------------------------------------------------------------------------------
// Viewport
// ----------------------------------------------------------------------------------

Viewport::Viewport( const Viewport& parent, unsigned int left, unsigned int top, unsigned int width, unsigned int height )
    : parent( &parent )
    , isDone( false )
    , left( left )
    , top( top )
    , width( width )
    , height( height )
{
}


Viewport::Viewport( const FrameRenderer& fr, bool fitSquare )
    : parent( nullptr )
    , isDone( false )
{
    if( fitSquare )
    {
        const unsigned int s = std::min( fr.width(), fr.height() );
        width = height = s;
        top = fr.height() / 2 - s / 2;
        left = fr.width() / 2 - s / 2;
    }
    else
    {
        left = top = 0;
        width = fr.width();
        height = fr.height();
    }
}


Viewport::Viewport( const Viewport& parent, float width, float height, float left, float top )
    : parent( &parent )
    , isDone( false )
    , width( static_cast< unsigned int >( parent.width * width + 0.5f ) )
    , height( static_cast< unsigned int >( parent.height * height + 0.5f ) )
{
    const unsigned int maxLeft = static_cast< unsigned >( static_cast< int >( parent.width  ) - static_cast< int >( this->width  ) );
    const unsigned int maxTop  = static_cast< unsigned >( static_cast< int >( parent.height ) - static_cast< int >( this->height ) );

    this->left = mix( 0u, maxLeft, left );
    this->top  = mix( 0u, maxTop , top );
}


Viewport::~Viewport()
{
    if( !isDone )
    {
        done();
    }
}

 
void Viewport::makeActive() const
{
    isDone = false;
    glViewport( left, top, width, height );
}


void Viewport::done() const
{
    isDone = true;
    if( parent != nullptr )
    {
        parent->makeActive();
    }
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
