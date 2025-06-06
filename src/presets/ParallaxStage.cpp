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

#include <LibCarna/base/glew.hpp>
#include <LibCarna/presets/ParallaxStage.hpp>
#include <LibCarna/base/math.hpp>

namespace LibCarna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// ParallaxStage :: Details
// ----------------------------------------------------------------------------------

struct ParallaxStage::Details
{
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    float eyeDistance;
    base::math::Matrix4f vtLeft;
    base::math::Matrix4f vtRight;
};



// ----------------------------------------------------------------------------------
// ParallaxStage
// ----------------------------------------------------------------------------------

const float ParallaxStage::DEFAULT_EYE_DISTANCE = 60;


ParallaxStage::ParallaxStage( CompositionStage::CompositionMode compositionMode )
    : CompositionStage( compositionMode )
    , pimpl( new Details() )
{
    setEyeDistance( DEFAULT_EYE_DISTANCE );
}


ParallaxStage::~ParallaxStage()
{
}


void ParallaxStage::setEyeDistance( float eyeDistance )
{
    pimpl->eyeDistance = eyeDistance;
    pimpl->vtLeft  = base::math::translation4f( -eyeDistance / 2, 0, 0 );
    pimpl->vtRight = base::math::translation4f( +eyeDistance / 2, 0, 0 );
}


float ParallaxStage::eyeDistance() const
{
    return pimpl->eyeDistance;
}


void ParallaxStage::renderPass
    ( const base::math::Matrix4f& viewTransform
    , base::RenderTask& rt
    , const base::Viewport& vp
    , bool isFirstInvocation
    , bool isFirstSource )
{
    const base::math::Matrix4f& vtShift = isFirstSource ? pimpl->vtLeft : pimpl->vtRight;
    const base::math::Matrix4f vtNew = viewTransform * vtShift;
    base::RenderTask rtFork( rt );
    if( isFirstInvocation )
    {
        rtFork.overrideViewTransform( vtNew );
        rtFork.render( vp );
    }
    else
    {
        rtFork.overrideViewTransform( vtNew );
        rtFork.render( vp );
        rt.finish();
    }
}



}  // namespace LibCarna :: presets

}  // namespace LibCarna
