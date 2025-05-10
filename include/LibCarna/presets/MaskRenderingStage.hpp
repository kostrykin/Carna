/*
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#ifndef MASKRENDERINGSTAGE_H_6014714286
#define MASKRENDERINGSTAGE_H_6014714286

#include <LibCarna/presets/VolumeRenderingStage.hpp>
#include <LibCarna/LibCarna.hpp>
#include <memory>

/** \file
  * \brief
  * Defines \ref LibCarna::presets::MaskRenderingStage.
  */

namespace LibCarna
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
  * \snippet IntegrationTests/DRRMaskRenderingStageIntegrationTest.cpp mask_rendering_setup
  *
  * The concept of geometry types is explained \ref GeometryTypes "here".
  *
  * \note
  * In the \ref RenderingProcess "rendering process" this stage will usually be
  * inserted as the last stage.
  *
  * A mask can be produced, for example, by threshold a 3D intensity image:
  *
  * \snippet IntegrationTests/DRRMaskRenderingStageIntegrationTest.cpp create_mask
  *
  * The mask can then be attached to, for example, an already existing
  * \ref base::Geometry "geometry node":
  *
  * \snippet IntegrationTests/DRRMaskRenderingStageIntegrationTest.cpp attach_mask
  *
  * This will render an image similar to this:
  *
  * \image html MaskRenderingStageTest.png "exemplary rendering from code above"
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA MaskRenderingStage : public VolumeRenderingStage
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    const static base::Color  DEFAULT_COLOR;      ///< Holds the default rendering color.
    const static unsigned int DEFAULT_ROLE_MASK;  ///< Holds the default value of \ref maskRole.
    const static bool         DEFAULT_FILLING;    ///< Holds the default value of \ref filling.

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
    explicit MaskRenderingStage( unsigned int geometryType, unsigned int maskRole = DEFAULT_ROLE_MASK );

    /** \brief
      * Deletes.
      */
    virtual ~MaskRenderingStage();

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
      * Tells whether filled regions are rendered (instead of borders only).
      */
    bool isFilling() const;

    /** \brief
      * Sets whether to render \a filled regions (instead of borders only).
      */
    void setFilling( bool filled );

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
      * Sets the border rendering mode and the rendering color.
      */
    virtual void configureShader() override;

    /** \brief
      * Does nothing.
      */
    virtual void configureShader( const base::Renderable& ) override;

}; // DRRStage



}  // namespace LibCarna :: presets

}  // namespace LibCarna

#endif // MASKRENDERINGSTAGE_H_6014714286
