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

#include <Carna/base/glew.h>
#include <Carna/presets/CompositionStage.h>
#include <Carna/base/FrameRenderer.h>

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// CompositionStage :: Details
// ----------------------------------------------------------------------------------

struct CompositionStage::Details
{
    Details();
    CompositionMode compositionMode;
    base::GLContext* glc;
    bool swap;
    
    void renderInterleaved
        ( CompositionStage* self
        , const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp );
    
    void renderAside
        ( CompositionStage* self
        , const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp );
};


CompositionStage::Details::Details()
    : glc( nullptr )
    , swap( false )
{
}


void CompositionStage::Details::renderInterleaved
    ( CompositionStage* self
    , const base::math::Matrix4f& viewTransform
    , base::RenderTask& rt
    , const base::Viewport& vp )
{
    CARNA_FAIL( "Interleaved composition mode not implemented yet!" );
}


void CompositionStage::Details::renderAside
    ( CompositionStage* self
    , const base::math::Matrix4f& vt
    , base::RenderTask& rt
    , const base::Viewport& vp )
{
    base::Viewport vp2( vp, vp.marginLeft() / 2, vp.marginTop() / 2, vp.width() / 2, vp.height() );
    self->renderPass( vt, rt, vp2, true, true ^ swap );
    vp2.setMarginLeft( vp2.marginLeft() + vp.parentWidth() / 2 );
    self->renderPass( vt, rt, vp2, false, false ^ swap );
}



// ----------------------------------------------------------------------------------
// CompositionStage
// ----------------------------------------------------------------------------------

CompositionStage::CompositionStage( CompositionMode compositionMode )
    : pimpl( new Details() )
{
    pimpl->compositionMode = compositionMode;
}


CompositionStage::~CompositionStage()
{
}


bool CompositionStage::isCompositionSwapped() const
{
    return pimpl->swap;
}


void CompositionStage::setCompositionSwap( bool swap )
{
    pimpl->swap = swap;
}


CompositionStage::CompositionMode CompositionStage::compositionMode() const
{
    return pimpl->compositionMode;
}


void CompositionStage::setCompositionMode( CompositionMode compositionMode )
{
    pimpl->compositionMode = compositionMode;
}


void CompositionStage::reshape( const base::FrameRenderer& fr, unsigned int width, unsigned int height )
{
    pimpl->glc = &fr.glContext();
}


bool CompositionStage::isInitialized() const
{
    return pimpl->glc != nullptr;
}


void CompositionStage::renderPass
    ( const base::math::Matrix4f& viewTransform
    , base::RenderTask& rt
    , const base::Viewport& vp )
{
    switch( pimpl->compositionMode )
    {
    
    case interleave:
        pimpl->renderInterleaved( this, viewTransform, rt, vp );
        break;
    
    case aside:
        pimpl->renderAside( this, viewTransform, rt, vp );
        break;
        
    default:
        CARNA_FAIL( "Unknown composition mode!" );
    
    }
}


void CompositionStage::prepareFrame( base::Node& root )
{
}



}  // namespace Carna :: presets

}  // namespace Carna
