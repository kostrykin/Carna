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

#include <LibCarna/base/Viewport.h>
#include <LibCarna/base/FrameRenderer.h>
#include <LibCarna/base/glew.h>
#include <LibCarna/base/math.h>
#include <stack>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Viewport :: Details
// ----------------------------------------------------------------------------------

struct Viewport::Details
{
    Details( unsigned int rootWidth, unsigned int rootHeight );
    Details( const Viewport* parent );
    const Viewport* const parent;
    const unsigned int rootWidth;
    const unsigned int rootHeight;

    unsigned int left;
    unsigned int top;
    unsigned int width;
    unsigned int height;
    
    void refresh() const;
};


Viewport::Details::Details( unsigned int rootWidth, unsigned int rootHeight )
    : parent( nullptr )
    , rootWidth( rootWidth )
    , rootHeight( rootHeight )
{
}


Viewport::Details::Details( const Viewport* parent )
    : parent( parent )
    , rootWidth( 0 )
    , rootHeight( 0 )
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


Viewport::Viewport( unsigned int parentWidth, unsigned int parentHeight, bool fitSquare )
    : pimpl( new Details( parentWidth, parentHeight ) )
{
    if( fitSquare )
    {
        const unsigned int s = std::min( parentWidth, parentHeight );
        pimpl->width = pimpl->height = s;
        pimpl->top   = parentHeight / 2 - s / 2;
        pimpl->left  = parentWidth  / 2 - s / 2;
    }
    else
    {
        pimpl->top = pimpl->left = 0;
        pimpl->width  = parentWidth;
        pimpl->height = parentHeight;
    }
}


Viewport::Viewport( const Framebuffer& fb )
    : pimpl( new Details( fb.width(), fb.height() ) )
{
    pimpl->top = pimpl->left = 0;
    pimpl->width  = fb.width();
    pimpl->height = fb.height();
}


Viewport::Viewport( const Viewport& parent, unsigned int left, unsigned int top, unsigned int width, unsigned int height )
    : pimpl( new Details( &parent ) )
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
    LIBCARNA_ASSERT( isActive() );
    viewports.pop();
    if( !viewports.empty() )
    {
        viewports.top()->pimpl->refresh();
    }
}
    
    
void Viewport::setWidth( unsigned int width )
{
    LIBCARNA_ASSERT( !isActive() );
    pimpl->width = width;
}


void Viewport::setHeight( unsigned int height )
{
    LIBCARNA_ASSERT( !isActive() );
    pimpl->height = height;
}


void Viewport::setMarginLeft( unsigned int marginLeft )
{
    LIBCARNA_ASSERT( !isActive() );
    pimpl->left = marginLeft;
}


void Viewport::setMarginTop( unsigned int marginTop )
{
    LIBCARNA_ASSERT( !isActive() );
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
        return pimpl->rootWidth;
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
        return pimpl->rootHeight;
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



}  // namespace LibCarna :: base

}  // namespace LibCarna
