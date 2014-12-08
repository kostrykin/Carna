/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/VolumeRenderings/MIP/MaximumIntensityProjection.h>
#include <Carna/VolumeRenderings/MIP/MaximumIntensityProjectionDetails.h>
#include <Carna/VolumeRenderings/MIP/MaximumIntensityProjectionControllerUI.h>
#include <Carna/VolumeRenderings/MIP/Channel.h>
#include <Carna/VolumeRenderings/VolumeRenderer.h>
#include <Carna/base/view/glew.h>
#include <Carna/base/view/ShaderProgram.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/view/RenderTexture.h>
#include <Carna/base/view/FramebufferObject.h>
#include <Carna/base/Visualization.h>
#include <Carna/base/VisualizationEnvironment.h>
#include <Carna/base/CarnaException.h>
#include <map>
#include <algorithm>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// MIP :: MaximumIntensityProjection
// ----------------------------------------------------------------------------------

const std::string MaximumIntensityProjection::NAME = "Maximum Intensity Projection";


MaximumIntensityProjection::MaximumIntensityProjection()
    : VolumeRendererMode( NAME )
    , pimpl( new Details() )
{
}


MaximumIntensityProjection::~MaximumIntensityProjection()
{
}


QWidget* MaximumIntensityProjection::createControllerUI()
{
    return new MaximumIntensityProjectionControllerUI( *this );
}


void MaximumIntensityProjection::prepareForRenderer( VolumeRenderer& renderer )
{
    VolumeRendererMode::prepareForRenderer( renderer );

    pimpl->fbo.reset( new base::view::FramebufferObject
        ( renderer.shortestTargetSide()
        , renderer.shortestTargetSide() ) );

    this->addChannel( new Channel
        ( base::IntSpan( -1024, 3071 )
        , QColor( 255, 255, 255, 0 )
        , QColor( 255, 255, 255, 255 ) ) );
}


void MaximumIntensityProjection::start()
{
    glEnable( GL_BLEND );
}


void MaximumIntensityProjection::resizeBuffers( int w, int h )
{
    const unsigned int s = std::min( w, h );

    for( auto slot_itr  = pimpl->channelSlots.begin(); slot_itr != pimpl->channelSlots.end(); ++slot_itr )
    {
        slot_itr->texture->resize( s, s );
    }

    pimpl->fbo->resize( s, s );
}


void MaximumIntensityProjection::renderColor( RayMarching& volume_rendering )
{
    glClear( GL_COLOR_BUFFER_BIT );

 // rebuild max intensity textures

    glBlendEquation( GL_MAX );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    {

     // bind shader

        base::view::ShaderProgram::Binding shader( volume_rendering.renderer.shader( "mip" ) );
        shader.putUniform1i( "intensityTexture", volume_rendering.volumeTextureSampler );
        shader.putUniform1i( "frontFacesTexture", volume_rendering.frontFacesTextureSampler );
        shader.putUniform1i( "backFacesTexture", volume_rendering.backFacesTextureSampler );
        shader.putUniform1i( "sample_count", volume_rendering.samples );

     // render textures

        for( unsigned int i = 0; i < pimpl->channelSlots.size(); ++i )
        {
            const Channel& ch = *pimpl->channelSlots[ i ].channel;
            base::view::RenderTexture& rt = *pimpl->channelSlots[ i ].texture;

         // bind & setup framebuffer object

            base::view::FramebufferObject::MinimalBinding fbo_minimal_binding( *pimpl->fbo );

            fbo_minimal_binding.setColorComponent( rt );

            base::view::FramebufferObject::Binding fbo_binding( *pimpl->fbo );

            glClear( GL_COLOR_BUFFER_BIT );
            
            shader.putUniform1i( "min_huv", ch.getHuvRange().first );
            shader.putUniform1i( "max_huv", ch.getHuvRange().last );

         // draw

            volume_rendering.renderer.paintRectangle();
        }

    }

 // compose result

    glBlendEquation( GL_FUNC_ADD );

    for( unsigned int i = 0; i < pimpl->channelSlots.size(); ++i )
    {
        const Channel& ch = *pimpl->channelSlots[ i ].channel;
        base::view::RenderTexture& rt = *pimpl->channelSlots[ i ].texture;

        ch.getFunction().makeCurrent();

     // bind max intensity texture

        glBindTexture( GL_TEXTURE_2D, rt.id );

     // bind shader

        base::view::ShaderProgram::Binding shader( volume_rendering.renderer.shader( "mip-colorizer" ) );
        shader.putUniform1i( "maxIntensityTexture", 0 );
        shader.putUniform1f( "min_color_r", ch.getMinColor().  redF() );
        shader.putUniform1f( "min_color_g", ch.getMinColor().greenF() );
        shader.putUniform1f( "min_color_b", ch.getMinColor(). blueF() );
        shader.putUniform1f( "min_color_a", ch.getMinColor().alphaF() * ch.getOpacity() );
        shader.putUniform1f( "max_color_r", ch.getMaxColor().  redF() );
        shader.putUniform1f( "max_color_g", ch.getMaxColor().greenF() );
        shader.putUniform1f( "max_color_b", ch.getMaxColor(). blueF() );
        shader.putUniform1f( "max_color_a", ch.getMaxColor().alphaF() * ch.getOpacity() );

     // paint max intensity texture

        renderer().paintRectangle();
    }

    // paint objects

    this->paintObjects3D();
    glColor4f( 1, 1, 1, 1 );
}


void MaximumIntensityProjection::renderDepth( RayMarching& )
{
}


void MaximumIntensityProjection::addChannel( Channel* channel )
{
    CARNA_ASSERT( this->isInitialized() );
    CARNA_ASSERT( pimpl->findSlotByChannel( *channel ) == pimpl->channelSlots.end() );

    Details::ChannelSlot slot( *this, channel );

    pimpl->channelSlots.push_back( std::move( slot ) );

    this->invalidate();

    connect( channel, SIGNAL( changed() ), &renderer(), SLOT( invalidate() ) );
}


void MaximumIntensityProjection::removeChannel( Channel& channel )
{
    CARNA_ASSERT( this->isInitialized() );

    auto slot_itr = pimpl->findSlotByChannel( channel );

    CARNA_ASSERT( slot_itr != pimpl->channelSlots.end() );

    renderer().module().environment().activateContext();

    pimpl->channelSlots.erase( slot_itr );
    
    this->invalidate();
}


void MaximumIntensityProjection::ascendChannel( const Channel& channel )
{
    CARNA_ASSERT( this->isInitialized() );

    auto slot_itr = pimpl->findSlotByChannel( channel );

    CARNA_ASSERT( slot_itr != pimpl->channelSlots.end() );
    CARNA_ASSERT( ( slot_itr + 1 ) != pimpl->channelSlots.end() );

    slot_itr->swap( *( slot_itr + 1 ) );

    renderer().invalidate();
}


#define VISIT_CHANNELS_IMPL \
    for( auto slot_itr = pimpl->channelSlots.begin(); slot_itr != pimpl->channelSlots.end(); ++slot_itr ) \
    { \
        if( !visit( *slot_itr->channel ) ) \
        { \
            break; \
        } \
    }


void MaximumIntensityProjection::visitChannels( ChannelVisitor& visit )
{
    VISIT_CHANNELS_IMPL
}


void MaximumIntensityProjection::visitChannels( ConstChannelVisitor& visit ) const
{
    VISIT_CHANNELS_IMPL
}


#undef VISIT_CHANNELS_IMPL



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
