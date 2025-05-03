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

#include <LibCarna/base/glew.h>
#include <LibCarna/base/RenderState.h>
#include <LibCarna/base/BlendFunction.h>
#include <LibCarna/base/GLContext.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// RenderState :: Details
// ----------------------------------------------------------------------------------

struct RenderState::Details
{

    Details( const RenderState* parent, GLContext* );

    const RenderState* const parent;
    GLContext* glc;

    static bool isCurrent( RenderState* self );
    static void assertCurrent( RenderState* self );

    bool     depthTest;
    bool     depthWrite;
    int      depthTestFunction;
    bool     blend;
    int      blendFunctionSourceFactor;
    int      blendFunctionDestinationFactor;
    int      blendEquation;
    CullFace cullFace;
    bool     frontFaceCCW;
    float    pointSize;

}; // RenderState :: Details


RenderState::Details::Details( const RenderState* parent, GLContext* glc )
    : parent( parent )
    , glc( glc )
{
}


bool RenderState::Details::isCurrent( RenderState* self )
{
    LIBCARNA_ASSERT( self->pimpl->glc != nullptr );
    if( self->pimpl->glc->isCurrent() )
    {
        return &self->pimpl->glc->currentRenderState() == self;
    }
    else
    {
        return false;
    }
}


void RenderState::Details::assertCurrent( RenderState* self )
{
    LIBCARNA_ASSERT_EX( isCurrent( self ), "RenderState instance is locked, because it is not the current one." );
}



// ----------------------------------------------------------------------------------
// RenderState
// ----------------------------------------------------------------------------------

RenderState::RenderState( GLContext& glc )
    : pimpl( new Details( nullptr, &glc ) )
{
    pimpl->glc->pushRenderState( *this );
}


RenderState::RenderState()
    : pimpl( new Details( &GLContext::current().currentRenderState(), &GLContext::current() ) )
{
    const RenderState& parent = *pimpl->parent;
    pimpl->glc->pushRenderState( *this );
    
    pimpl->depthTest                      = parent.pimpl->depthTest;
    pimpl->depthWrite                     = parent.pimpl->depthWrite;
    pimpl->depthTestFunction              = parent.pimpl->depthTestFunction;
    pimpl->blend                          = parent.pimpl->blend;
    pimpl->blendFunctionSourceFactor      = parent.pimpl->blendFunctionSourceFactor;
    pimpl->blendFunctionDestinationFactor = parent.pimpl->blendFunctionDestinationFactor;
    pimpl->blendEquation                  = parent.pimpl->blendEquation;
    pimpl->cullFace                       = parent.pimpl->cullFace;
    pimpl->frontFaceCCW                   = parent.pimpl->frontFaceCCW;
    pimpl->pointSize                      = parent.pimpl->pointSize;
}


RenderState::~RenderState()
{
    if( pimpl->parent != nullptr )
    {
        const RenderState::Details& parent = *pimpl->parent->pimpl;

        setDepthTest( parent.depthTest );
        setDepthWrite( parent.depthWrite );
        setDepthTestFunction( parent.depthTestFunction );
        setBlend( parent.blend );
        setBlendFunction( BlendFunction( parent.blendFunctionSourceFactor, parent.blendFunctionDestinationFactor ) );
        setBlendEquation( parent.blendEquation );
        setCullFace( parent.cullFace );
        setFrontFace( parent.frontFaceCCW );
        setPointSize( parent.pointSize );

        pimpl->glc->popRenderState();
    }
}


void RenderState::commit() const
{
    commitDepthTest();
    commitDepthWrite();
    commitDepthTestFunction();
    commitBlend();
    commitBlendFunction();
    commitBlendEquation();
    commitCullFace();
    commitFrontFace();
    commitPointSize();
}


void RenderState::setDepthTest( bool dt )
{
    Details::assertCurrent( this );
    if( dt != pimpl->depthTest )
    {
        pimpl->depthTest = dt;
        commitDepthTest();
    }
}


void RenderState::setDepthWrite( bool dw )
{
    Details::assertCurrent( this );
    if( dw != pimpl->depthWrite )
    {
        pimpl->depthWrite = dw;
        commitDepthWrite();
    }
}


void RenderState::setDepthTestFunction( int dtf )
{
    Details::assertCurrent( this );
    if( dtf != pimpl->depthTestFunction )
    {
        pimpl->depthTestFunction = dtf;
        commitDepthTestFunction();
    }
}


void RenderState::setBlend( bool b )
{
    Details::assertCurrent( this );
    if( b != pimpl->blend )
    {
        pimpl->blend = b;
        commitBlend();
    }
}


void RenderState::setBlendFunction( const BlendFunction& bf )
{
    Details::assertCurrent( this );
    if( bf.sourceFactor != pimpl->blendFunctionSourceFactor || bf.destinationFactor != pimpl->blendFunctionDestinationFactor )
    {
        pimpl->blendFunctionSourceFactor = bf.sourceFactor;
        pimpl->blendFunctionDestinationFactor = bf.destinationFactor;
        commitBlendFunction();
    }
}


void RenderState::setBlendEquation( int be )
{
    Details::assertCurrent( this );
    if( be != pimpl->blendEquation )
    {
        pimpl->blendEquation = be;
        commitBlendEquation();
    }
}


void RenderState::setCullFace( CullFace cf )
{
    Details::assertCurrent( this );
    if( cf != pimpl->cullFace )
    {
        pimpl->cullFace = cf;
        commitCullFace();
    }
}


void RenderState::setFrontFace( bool ccw )
{
    Details::assertCurrent( this );
    if( ccw != pimpl->frontFaceCCW )
    {
        pimpl->frontFaceCCW = ccw;
        commitFrontFace();
    }
}


void RenderState::setPointSize( float pointSize )
{
    Details::assertCurrent( this );
    if( !( pointSize < 0 && pimpl->pointSize < 0 ) && !base::math::isEqual( pointSize, pimpl->pointSize ) )
    {
        pimpl->pointSize = pointSize;
        commitPointSize();
    }
}


void RenderState::commitDepthTest() const
{
    if( pimpl->depthTest )
    {
        glEnable( GL_DEPTH_TEST );
    }
    else
    {
        glDisable( GL_DEPTH_TEST );
    }
}


void RenderState::commitDepthWrite() const
{
    glDepthMask( pimpl->depthWrite ? GL_TRUE : GL_FALSE );
}


void RenderState::commitDepthTestFunction() const
{
    glDepthFunc( pimpl->depthTestFunction );
}


void RenderState::commitBlend() const
{
    if( pimpl->blend )
    {
        glEnable( GL_BLEND );
    }
    else
    {
        glDisable( GL_BLEND );
    }
}


void RenderState::commitBlendFunction() const
{
    glBlendFunc( pimpl->blendFunctionSourceFactor, pimpl->blendFunctionDestinationFactor );
}


void RenderState::commitBlendEquation() const
{
    glBlendEquation( pimpl->blendEquation );
}


void RenderState::commitCullFace() const
{
    switch( pimpl->cullFace )
    {

    case cullNone:
        glDisable( GL_CULL_FACE );
        break;

    case cullFront:
        glEnable( GL_CULL_FACE );
        glCullFace( GL_FRONT );
        break;

    case cullBack:
        glEnable( GL_CULL_FACE );
        glCullFace( GL_BACK );
        break;

    }
}


void RenderState::commitFrontFace() const
{
    glFrontFace( pimpl->frontFaceCCW ? GL_CCW : GL_CW );
}


void RenderState::commitPointSize() const
{
    if( pimpl->pointSize < 0 )
    {
        glEnable( GL_PROGRAM_POINT_SIZE );
    }
    else
    {
        glDisable( GL_PROGRAM_POINT_SIZE );
        glPointSize( pimpl->pointSize );
    }
}



}  // namespace Carna :: base

}  // namespace Carna
