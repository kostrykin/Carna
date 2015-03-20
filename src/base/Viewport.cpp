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

#include <Carna/base/Viewport.h>
#include <Carna/base/FrameRenderer.h>
#include <Carna/base/glew.h>
#include <stack>

namespace Carna
{

namespace base
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
// Viewport :: Details
// ----------------------------------------------------------------------------------

struct Viewport::Details
{
    Details( const Viewport* parent, const FrameRenderer* fr );
    const Viewport* const parent;
    const FrameRenderer* const fr;

    unsigned int left;
    unsigned int top;
    unsigned int width;
    unsigned int height;
    
    void refresh() const;
};


Viewport::Details::Details( const Viewport* parent, const FrameRenderer* fr )
    : parent( parent )
    , fr( fr )
{
}


void Viewport::Details::refresh() const
{
    glViewport( left, top, width, height );
}



// ----------------------------------------------------------------------------------
// Viewport
// ----------------------------------------------------------------------------------

typedef std::stack< const Viewport* > Viewports;
static Viewports viewports = Viewports();


Viewport::Viewport( const FrameRenderer& fr, bool fitSquare )
    : pimpl( new Details( nullptr, &fr ) )
{
    if( fitSquare )
    {
        const unsigned int s = std::min( fr.width(), fr.height() );
        pimpl->width = pimpl->height = s;
        pimpl->top  = fr.height() / 2 - s / 2;
        pimpl->left = fr.width () / 2 - s / 2;
    }
    else
    {
        pimpl->left = pimpl->top = 0;
        pimpl->width = fr.width();
        pimpl->height = fr.height();
    }
}


Viewport::Viewport( const Viewport& parent, unsigned int left, unsigned int top, unsigned int width, unsigned int height )
    : pimpl( new Details( &parent, nullptr ) )
{
    pimpl->left   = left;
    pimpl->top    = top;
    pimpl->width  = width;
    pimpl->height = height;
}


Viewport::~Viewport()
{
    if( isActive() )
    {
        done();
    }
}


bool Viewport::isActive() const
{
    return !viewports.empty() && viewports.top() == this;
}

 
void Viewport::makeActive() const
{
    if( !isActive() )
    {
        viewports.push( this );
        pimpl->refresh();
    }
}


void Viewport::done() const
{
    CARNA_ASSERT( isActive() );
    viewports.pop();
    if( !viewports.empty() )
    {
        viewports.top()->pimpl->refresh();
    }
}
    
    
void Viewport::setWidth( unsigned int width )
{
    CARNA_ASSERT( !isActive() );
    pimpl->width = width;
}


void Viewport::setHeight( unsigned int height )
{
    CARNA_ASSERT( !isActive() );
    pimpl->height = height;
}


void Viewport::setMarginLeft( unsigned int marginLeft )
{
    CARNA_ASSERT( !isActive() );
    pimpl->left = marginLeft;
}


void Viewport::setMarginTop( unsigned int marginTop )
{
    CARNA_ASSERT( !isActive() );
    pimpl->top = marginTop;
}


unsigned int Viewport::width() const
{
    return pimpl->width;
}


unsigned int Viewport::height() const
{
    return pimpl->height;
}


unsigned int Viewport::parentWidth() const
{
    if( pimpl->parent != nullptr )
    {
        return pimpl->parent->width();
    }
    else
    {
        return pimpl->fr->width();
    }
}


unsigned int Viewport::parentHeight() const
{
    if( pimpl->parent != nullptr )
    {
        return pimpl->parent->height();
    }
    else
    {
        return pimpl->fr->height();
    }
}


unsigned int Viewport::marginLeft() const
{
    return pimpl->left;
}


unsigned int Viewport::marginTop() const
{
    return pimpl->top;
}


unsigned int Viewport::marginRight() const
{
    return parentWidth() - marginLeft() - width();
}


unsigned int Viewport::marginBottom() const
{
    return parentHeight() - marginTop() - height();
}



}  // namespace Carna :: base

}  // namespace Carna
