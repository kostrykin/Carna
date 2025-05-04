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

#ifndef COMPOSITIONSTAGE_H_6014714286
#define COMPOSITIONSTAGE_H_6014714286

#include <LibCarna/base/RenderStage.h>

/** \file   CompositionStage.h
  * \brief  Defines \ref Carna::presets::CompositionStage.
  */

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// CompositionStage
// ----------------------------------------------------------------------------------

/** \brief
  * Defines \ref base::RenderStage "rendering stage" that combines two other
  * renderings either by putting them next to each other or through row-wise
  * interleaving.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 31.3.15
  */
class LIBCARNA CompositionStage : public base::RenderStage
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Defines how two renderings are to be combined.
      */
    enum CompositionMode
    {
        /** Combines renderings through row-wise interleaving.
          * \image html ParallaxStageIntegrationTest/interleaved.png
          */
        interleave,

        /** Combines renderings by putting them next to each other.
          * \image html ParallaxStageIntegrationTest/aside.png
          */
        aside
    };

    /** \brief
      * Instantiates.
      */
    CompositionStage( CompositionMode compositionMode );

    /** \brief
      * Deletes.
      */
    virtual ~CompositionStage();
    
    /** \brief
      * Sets how the two renderings obtained through
      * \ref renderPass(const base::math::Matrix4f&,base::RenderTask&,const base::Viewport&,bool,bool)
      * shall be combined.
      */
    void setCompositionMode( CompositionMode compositionMode );
    
    /** \brief
      * Tells how the two renderings obtained through
      * \ref renderPass(const base::math::Matrix4f&,base::RenderTask&,const base::Viewport&,bool,bool)
      * are combined.
      */
    CompositionMode compositionMode() const;
    
    /** \brief
      * Swaps the two renderings obtained through
      * \ref renderPass(const base::math::Matrix4f&,base::RenderTask&,const base::Viewport&,bool,bool)
      * when combining.
      */
    void setCompositionSwap( bool );
    
    /** \brief
      * Tells whether the two renderings obtained through
      * \ref renderPass(const base::math::Matrix4f&,base::RenderTask&,const base::Viewport&,bool,bool)
      * are swapped when combined.
      */
    bool isCompositionSwapped() const;

    virtual void reshape( base::FrameRenderer& fr, unsigned int width, unsigned int height ) override;
    
    virtual void prepareFrame( base::Node& root ) override;

    /** \brief
      * Invokes
      * \ref renderPass(const base::math::Matrix4f&,base::RenderTask&,const base::Viewport&,bool,bool)
      * twice and combines the results \ref setCompositionMode "like specified".
      */
    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;
        
protected:

    /** \brief
      * Renders one of the two renderings that are to be combined per invocation.
      *
      * \param viewTransform transforms \ref ViewSpace "world space to view space".
      *
      * \param rt references the \ref RenderingProcess "rendering task".
      *
      * \param vp references the \ref FrameCoordinates "viewport".
      *
      * \param isFirstInvocation indicates whether this is the first rendering
      *     invocation of the current \ref renderPass "rendering pass".
      *
      * \param isFirstSource indicates whether this rendering should be done using
      *     the "first" method/source/algorithm.
      *
      * Note that `isFirstInvocation == isFirstSource` is true when
      * \ref setCompositionSwap "swapping" was disabled and
      * `isFirstInvocation != isFirstSource` is true if it was enabled.
      */
    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp
        , bool isFirstInvocation
        , bool isFirstSource ) = 0;

}; // CompositionStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // COMPOSITIONSTAGE_H_6014714286
