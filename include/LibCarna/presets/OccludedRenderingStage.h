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

#ifndef OCCLUDEDRENDERINGSTAGE_H_6014714286
#define OCCLUDEDRENDERINGSTAGE_H_6014714286

#include <LibCarna/base/RenderStage.h>

/** \file   OccludedRenderingStage.h
  * \brief  Defines \ref Carna::presets::OccludedRenderingStage.
  */

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// OccludedRenderingStage
// ----------------------------------------------------------------------------------

/** \brief
  * Renders geometry that is actually depth-occluded. The occluded geometry is
  * rendered with reduced opacity on-top of the geometry that it is occluded by.
  *
  * \note
  * In the \ref RenderingProcess "rendering process" this stage should be inserted
  * \em before the geometry that is to be rendered with reduced opacity when occluded
  * and \em after the geometry that the occluded geometry should be rendered on-top
  * of.
  *
  * Note that every geometry type that is to be rendered like described above when
  * occluded must be \ref enableStage "enabled" explicitly.
  *
  * The following exemplary setup of the rendering process draws opaque meshes, that
  * are occluded by cutting planes, on-top of them with reduced opacity:
  *
  * -# \ref CuttingPlanesStage
  * -# \ref OccludedRenderingStage with stage 3 enabled
  * -# \ref OpaqueRenderingStage
  *
  * \author Leonid Kostrykin
  * \date   21.2.15 - 6.3.15
  */
class LIBCARNA OccludedRenderingStage : public base::RenderStage
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

    struct VideoResources;
    std::unique_ptr< VideoResources > vr;

public:

    /** \brief
      * Holds the default translucency of the occluding geometry.
      */
    const static float DEFAULT_OCCLUSION_TRANSLUCENCY;

    /** \brief
      * Sets \ref setOcclusionTranslucency "translucency of occluding geometry" to
      * \ref DEFAULT_OCCLUSION_TRANSLUCENCY.
      */
    OccludedRenderingStage();

    /** \brief
      * Deletes.
      */
    virtual ~OccludedRenderingStage();

    /** \brief
      * Disables all stages.
      */
    void disableAllStages();

    /** \brief
      * Enables \a rs in \f$\mathcal O\left(\log n\right)\f$.
      */
    void enableStage( const base::RenderStage& rs );
    
    /** \brief
      * Disables \a rs in \f$\mathcal O\left(\log n\right)\f$.
      */
    void disableStage( const base::RenderStage& rs );
    
    /** \brief
      * Tells whether \a rs is enabled in \f$\mathcal O\left(\log n\right)\f$.
      */
    bool isStageEnabled( const base::RenderStage& rs ) const;

    /** \brief
      * Sets the translucency of the occluding geometry.
      *
      * For `translucency == 1` it looks as if the occluding geometry was completely
      * translucent.
      */
    void setOcclusionTranslucency( float translucency );
    
    /** \brief
      * Tells the translucency of the occluding geometry.
      *
      * For `translucency == 1` it looks as if the occluding geometry was completely
      * translucent.
      */
    float occlusionTranslucency() const;

    virtual void reshape( base::FrameRenderer& fr, unsigned int width, unsigned int height ) override;
    
    virtual void prepareFrame( base::Node& root ) override;

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;

}; // OccludedRenderingStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // OCCLUDEDRENDERINGSTAGE_H_6014714286
