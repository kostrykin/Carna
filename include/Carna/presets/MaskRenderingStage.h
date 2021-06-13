/*
 *  Copyright (C) 2021 Leonid Kostrykin
 *
 */

#ifndef MASKRENDERINGSTAGE_H_6014714286
#define MASKRENDERINGSTAGE_H_6014714286

#include <Carna/presets/VolumeRenderingStage.h>
#include <Carna/Carna.h>
#include <memory>

/** \file   MaskRenderingStage.h
  * \brief  Defines \ref Carna::presets::MaskRenderingStage.
  */

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// MaskRenderingStage
// ----------------------------------------------------------------------------------

/** \brief
  * Renders 3D masks.
  *
  * The `%MaskRenderingStage` constructor takes a geometry type parameter:
  *
  * \snippet IntegrationTests/MaskRenderingStageIntegrationTest.cpp mask_rendering_setup
  *
  * The concept of geometry types is explained \ref GeometryTypes "here".
  *
  * \note
  * In the \ref RenderingProcess "rendering process" this stage will usually be
  * inserted as the last stage.
  *
  * A mask can be produced, for example, by threshold a 3D intensity image:
  *
  * \snippet IntegrationTests/MaskRenderingStageIntegrationTest.cpp create_mask
  *
  * The mask can then be attached to, for example, an already existing
  * \ref base::Geometry "geometry node":
  *
  * \snippet IntegrationTests/MaskRenderingStageIntegrationTest.cpp attach_mask
  *
  * This will render an image similar to this:
  *
  * \image html MaskRenderingStageTest.png "exemplary rendering from code above"
  *
  * \author Leonid Kostrykin
  * \date   June 2021
  * \since  \ref v_3_3_0
  */
class CARNA_LIB MaskRenderingStage : public VolumeRenderingStage
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    const static base::Color  DEFAULT_COLOR;      ///< Holds the default rendering color.
    const static unsigned int ROLE_DEFAULT_MASK;  ///< Holds the default value of \ref maskRole.

    /** \brief
      * Holds the \ref GeometryFeatures "role" that mask volume data is expected to
      * take when attached to \ref base::Geometry nodes.
      */
    const unsigned int maskRole;
    
    /** \brief
      * Instantiates.
      *
      * The created stage will render such \ref base::Geometry scene graph nodes,
      * whose \ref GeometryTypes "geometry types" equal \a geometryType. The mask
      * data will be obtained from \a maskRole.
      */
    explicit MaskRenderingStage( unsigned int geometryType, unsigned int maskRole = ROLE_DEFAULT_MASK );

    /** \brief
      * Deletes.
      */
    virtual ~MaskRenderingStage();
    
    MaskRenderingStage* clone() const override;

    virtual void reshape( base::FrameRenderer& fr, unsigned int width, unsigned int height ) override;

    /** \brief
      * Performs the mask rendering.
      */
    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;

    /** \brief
      * Tells the rendering color.
      */
    const base::Color& color() const;
    
    /** \brief
      * Sets the rendering \a color.
      */
    void setColor( const base::Color& color );

    /** \brief
      * Tells whether borders are rendered instead of regions.
      */
    bool renderBorders() const;

    /** \brief
      * Sets whether to render \a borders instead of regions.
      */
    void setRenderBorders( bool borders );

protected:

    virtual unsigned int loadVideoResources() override;

    virtual void createVolumeSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler ) override;

    /** \brief
      * Acquires the `mask_rendering` shader from the \ref base::ShaderManager.
      */
    virtual const base::ShaderProgram& acquireShader() override;

    /** \brief
      * Maps \ref maskRole to `mask`.
      */
    virtual const std::string& uniformName( unsigned int role ) const override;

    /** \brief
      * Does nothing.
      */
    virtual void configureShader() override;

    /** \brief
      * Does nothing.
      */
    virtual void configureShader( const base::Renderable& ) override;

}; // DRRStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // MASKRENDERINGSTAGE_H_6014714286
