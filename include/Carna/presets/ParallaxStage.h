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

#ifndef PARALLAXSTAGE_H_6014714286
#define PARALLAXSTAGE_H_6014714286

#include <Carna/presets/CompositionStage.h>

/** \file   ParallaxStage.h
  * \brief  Defines \ref Carna::presets::ParallaxStage.
  */

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// ParallaxStage
// ----------------------------------------------------------------------------------

class CARNA_LIB ParallaxStage : public CompositionStage
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    const static float DEFAULT_EYE_DISTANCE;

    ParallaxStage( CompositionMode compositionMode );

    virtual ~ParallaxStage();
    
    void setEyeDistance( float );
    
    float eyeDistance() const;
        
protected:

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp
        , bool isFirstInvocation
        , bool isFirstSource ) override;

}; // ParallaxStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // PARALLAXSTAGE_H_6014714286
