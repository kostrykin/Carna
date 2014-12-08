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

#include <Carna/base/view/glew.h>
#include <Carna/VolumeRenderings/DRR/XRay.h>
#include <Carna/VolumeRenderings/DRR/XRayControllerUI.h>
#include <Carna/VolumeRenderings/DRR/XRayDetails.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/model/Volume.h>
#include <Carna/VolumeRenderings/VolumeRenderer.h>

namespace Carna
{

namespace VolumeRenderings
{

namespace DRR
{



// ----------------------------------------------------------------------------------
// XRay
// ----------------------------------------------------------------------------------

const std::string XRay::NAME = "X-Ray";


XRay::XRay( const base::model::Scene& model )
    : VolumeRendererSinglePassMode( "xray", NAME )
    , pimpl( new Details( model ) )
{
    pimpl->mu_water = 2.0;
    pimpl->brightness = 0.6;
    pimpl->lower_threshold = model.recommendedVoidThreshold();
    pimpl->upper_threshold = 400;
    pimpl->upper_multiplier = 1.5;
    pimpl->window_level = 0.5;
    pimpl->window_width = 1.0;
    pimpl->mask_contrast = 2.;

    connect( &model, SIGNAL( volumeMaskExchanged() ), this, SLOT( invalidate() ) );
}


XRay::~XRay()
{
}


QWidget* XRay::createControllerUI()
{
    return new XRayControllerUI( *this );
}


void XRay::configureShader
    ( RayMarching& pass
    , base::view::ShaderProgram::Binding& shader )
{
    shader.putUniform1i( "maskTexture", pass.maskTextureSampler );

 // ----------------------------------------------------------------------------------

    shader.putUniform1f( "mu_water", static_cast< float >( pimpl->mu_water ) );
    shader.putUniform1f( "brightness", static_cast< float >( pimpl->brightness ) );
    shader.putUniform1f( "lower_threshold", ( pimpl->lower_threshold + 1024 ) / 4096.f );
    shader.putUniform1f( "upper_threshold", ( pimpl->upper_threshold + 1024 ) / 4096.f );
    shader.putUniform1f( "upper_multiplier", static_cast< float >( pimpl->upper_multiplier ) );
    shader.putUniform1f( "window_level", static_cast< float >( pimpl->window_level ) );
    shader.putUniform1f( "window_width", static_cast< float >( pimpl->window_width ) );

 // ----------------------------------------------------------------------------------

    base::Vector proportions( pimpl->model.volume().size.x * pimpl->model.spacingX()
                            , pimpl->model.volume().size.y * pimpl->model.spacingY()
                            , pimpl->model.volume().size.z * pimpl->model.spacingZ() );
    const double proportions_factor = std::max( proportions.x(), std::max( proportions.y(), proportions.z() ) );
    proportions /= proportions_factor;
    
 // ----------------------------------------------------------------------------------

    const base::model::Scene& model = renderer().provider.scene;
    if( !model.hasVolumeMask() )
    {
        shader.putUniform1f( "mask_bias", 1.f );
    }
    else
    {
        shader.putUniform1f( "mask_bias", model.volumeMask().isBinary() ? 1.f : 0.f );
    }

    shader.putUniform1f( "mask_contrast", static_cast< float >( pimpl->mask_contrast ) );
    
 // ----------------------------------------------------------------------------------

    shader.putUniform3f( "proportions", proportions );

 // ----------------------------------------------------------------------------------

    glDisable( GL_DEPTH_TEST );
}


void XRay::setWaterAttenuation( double mu_water )
{
    pimpl->mu_water = mu_water;

    emit waterAttenuationChanged( mu_water );
    this->invalidate();
}


void XRay::setBrightness( double brightness )
{
    pimpl->brightness = brightness;

    emit brightnessChanged( brightness );
    this->invalidate();
}


void XRay::setLowerThreshold( int lower_threshold )
{
    pimpl->lower_threshold = lower_threshold;

    emit lowerThresholdChanged( lower_threshold );
    this->invalidate();
}


void XRay::setUpperThreshold( int upper_threshold )
{
    pimpl->upper_threshold = upper_threshold;

    emit upperThresholdChanged( upper_threshold );
    this->invalidate();
}


void XRay::setUpperMultiplier( double upper_multiplier )
{
    pimpl->upper_multiplier = upper_multiplier;

    emit upperMultiplierChanged( upper_multiplier );
    this->invalidate();
}


void XRay::setWindowingLevel( double window_level )
{
    pimpl->window_level = window_level;

    emit windowingLevelChanged( window_level );
    this->invalidate();
}


void XRay::setWindowingWidth( double window_width )
{
    pimpl->window_width = window_width;

    emit windowingWidthChanged( window_width );
    this->invalidate();
}


void XRay::setMaskContrast( double mask_contrast )
{
    pimpl->mask_contrast = mask_contrast;

    emit maskContrastChanged( mask_contrast );
    this->invalidate();
}


double XRay::waterAttenuation() const
{
    return pimpl->mu_water;
}


double XRay::brightness() const
{
    return pimpl->brightness;
}


int XRay::lowerThreshold() const
{
    return pimpl->lower_threshold;
}


int XRay::upperThreshold() const
{
    return pimpl->upper_threshold;
}


double XRay::upperMultiplier() const
{
    return pimpl->upper_multiplier;
}


double XRay::windowingLevel() const
{
    return pimpl->window_level;
}


double XRay::windowingWidth() const
{
    return pimpl->window_width;
}


double XRay::maskContrast() const
{
    return pimpl->mask_contrast;
}



}  // namespace Carna :: VolumeRenderings :: DRR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
