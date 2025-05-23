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

#ifndef PARALLAXSTAGE_H_6014714286
#define PARALLAXSTAGE_H_6014714286

#include <LibCarna/presets/CompositionStage.hpp>

/** \file
  * \brief
  * Defines \ref LibCarna::presets::ParallaxStage.
  */

namespace LibCarna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// ParallaxStage
// ----------------------------------------------------------------------------------

/** \brief
  * Produces stereoscopic renderings.
  *
  * \section ParallaxStageExample Example
  *
  * The instantiation of the `%ParallaxStage` takes a parameter that decides how
  * the composition of the two images should be done:
  *
  * \snippet IntegrationTests/ParallaxStageIntegrationTest.cpp parallax_instantiation
  *
  * We also enlarge the eye distance for demonstration purposes. For same reason,
  * lets also add a few other rendering stages:
  *
  * \snippet IntegrationTests/ParallaxStageIntegrationTest.cpp parallax_instantiation_others
  *
  * The concept of geometry types is explained \ref GeometryTypes "here".
  *
  * The last step is to setup the scene, e.g.:
  *
  * \snippet IntegrationTests/ParallaxStageIntegrationTest.cpp parallax_scene_setup
  *
  * The concept of materials, meshes and other geometry feature is explained
  * \ref GeometryFeatures "here".
  *
  * \image html ParallaxStageIntegrationTest/aside.png "exemplary rendering in aside-mode from code above"
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA ParallaxStage : public CompositionStage
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Holds the default double distance that the camera is offset to left/right.
      */
    const static float DEFAULT_EYE_DISTANCE;

    /** \brief
      * Sets the \ref setEyeDistance "eye distance" to \ref DEFAULT_EYE_DISTANCE.
      */
    explicit ParallaxStage( CompositionMode compositionMode );

    /** \brief
      * Deletes.
      */
    virtual ~ParallaxStage();
    
    /** \brief
      * Sets the double of the distance that the camera is offset to left/right.
      */
    void setEyeDistance( float );
    
    /** \brief
      * Tells the double of the distance that the camera is offset to left/right.
      */
    float eyeDistance() const;
        
protected:

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp
        , bool isFirstInvocation
        , bool isFirstSource ) override;

}; // ParallaxStage



}  // namespace LibCarna :: presets

}  // namespace LibCarna

#endif // PARALLAXSTAGE_H_6014714286
