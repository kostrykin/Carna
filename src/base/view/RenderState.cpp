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
#include <Carna/base/view/RenderState.h>
#include <Carna/base/view/BlendFunction.h>
#include <Carna/base/view/GLContext.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// RenderState :: Details
// ----------------------------------------------------------------------------------

struct RenderState::Details
{

    Details( const RenderState* parent );

    const RenderState* const parent;
    GLContext* glc;

    bool     depthTest;
    bool     depthWrite;
    int      depthTestFunction;
    bool     blend;
    int      blendFunctionSourceFactor;
    int      blendFunctionDestinationFactor;
    int      blendEquation;
    CullFace cullFace;
    bool     frontFaceCCW;

}; // RenderState :: Details


RenderState::Details::Details( const RenderState* parent )
    : parent( parent )
    , glc( nullptr )
{
}



// ----------------------------------------------------------------------------------
// RenderState
// ----------------------------------------------------------------------------------

RenderState::RenderState()
    : pimpl( new Details( nullptr ) )
{
}


RenderState::RenderState( GLContext& glc )
    : pimpl( new Details( glc.renderStates.top() ) )
{
    const RenderState& parent = *glc.renderStates.top();
    glc.renderStates.push( this );
    pimpl->glc = &glc;

    pimpl->depthTest                      = parent.pimpl->depthTest;
    pimpl->depthWrite                     = parent.pimpl->depthWrite;
    pimpl->depthTestFunction              = parent.pimpl->depthTestFunction;
    pimpl->blend                          = parent.pimpl->blend;
    pimpl->blendFunctionSourceFactor      = parent.pimpl->blendFunctionSourceFactor;
    pimpl->blendFunctionDestinationFactor = parent.pimpl->blendFunctionDestinationFactor;
    pimpl->blendEquation                  = parent.pimpl->blendEquation;
    pimpl->cullFace                       = parent.pimpl->cullFace;
    pimpl->frontFaceCCW                   = parent.pimpl->frontFaceCCW;
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
    }
    if( pimpl->glc != nullptr )
    {
        pimpl->glc->renderStates.pop();
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
}


void RenderState::setDepthTest( bool dt )
{
    if( dt != pimpl->depthTest )
    {
        pimpl->depthTest = dt;
        commitDepthTest();
    }
}


void RenderState::setDepthWrite( bool dw )
{
    if( dw != pimpl->depthWrite )
    {
        pimpl->depthWrite = dw;
        commitDepthWrite();
    }
}


void RenderState::setDepthTestFunction( int dtf )
{
    if( dtf != pimpl->depthTestFunction )
    {
        pimpl->depthTestFunction = dtf;
        commitDepthTestFunction();
    }
}


void RenderState::setBlend( bool b )
{
    if( b != pimpl->blend )
    {
        pimpl->blend = b;
        commitBlend();
    }
}


void RenderState::setBlendFunction( const BlendFunction& bf )
{
    if( bf.sourceFactor != pimpl->blendFunctionSourceFactor || bf.destinationFactor != pimpl->blendFunctionDestinationFactor )
    {
        pimpl->blendFunctionSourceFactor = bf.sourceFactor;
        pimpl->blendFunctionDestinationFactor = bf.destinationFactor;
        commitBlendFunction();
    }
}


void RenderState::setBlendEquation( int be )
{
    if( be != pimpl->blendEquation )
    {
        pimpl->blendEquation = be;
        commitBlendEquation();
    }
}


void RenderState::setCullFace( CullFace cf )
{
    if( cf != pimpl->cullFace )
    {
        pimpl->cullFace = cf;
        commitCullFace();
    }
}


void RenderState::setFrontFace( bool ccw )
{
    if( ccw != pimpl->frontFaceCCW )
    {
        pimpl->frontFaceCCW = ccw;
        commitFrontFace();
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



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
