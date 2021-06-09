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

#ifndef MIPSTAGE_H_6014714286
#define MIPSTAGE_H_6014714286

#include <Carna/presets/VolumeRenderingStage.h>
#include <Carna/Carna.h>
#include <memory>

/** \file   MIPStage.h
  * \brief  Defines \ref Carna::presets::MIPStage.
  */

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// MIPStage
// ----------------------------------------------------------------------------------

/** \brief
  * Renders maximum intensity projections of volume geometries in the scene.
  *
  * \section MIPStageBasics Basics
  *
  * The `%MIPStage` constructor takes a geometry type parameter:
  *
  * \snippet ModuleTests/MIPStageTest.cpp mip_instantiation
  *
  * The concept of geometry types is explained \ref GeometryTypes "here".
  *
  * \note
  * In the \ref RenderingProcess "rendering process" this stage will usually be
  * inserted \em after such stages that render opaque geometry, like
  * \ref CuttingPlanesStage and \ref OpaqueRenderingStage.
  *
  * \image html MIPStageTest/layerReplace.png "exemplary rendering with two layers from the code above"
  *
  * \section MIPStageLayers Layers
  *
  * \todo Add documentation on layers.
  *
  * The next code snippet turns the second layer to *additive*-mode:
  *
  * \snippet ModuleTests/MIPStageTest.cpp mip_setup_additive
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 11.3.15
  */
class CARNA_LIB MIPStage : public VolumeRenderingStage
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:
    
    /** \brief
      * Holds the \ref GeometryFeatures "role" that HU volume data is expected to
      * take when attached to \ref base::Geometry nodes.
      */
    const static unsigned int ROLE_INTENSITY_VOLUME = 0;

    /** \brief
      * Instantiates.
      */
    MIPStage( unsigned int geometryType );

    /** \brief
      * Deletes.
      */
    virtual ~MIPStage();
    
    MIPStage* clone() const override;

    virtual void reshape( base::FrameRenderer& fr, unsigned int width, unsigned int height ) override;

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;
    
    /** \brief
      * Swaps positions of \a layer with it's successor in the \ref MIPStageLayers "layers list".
      */
    void ascendLayer( const MIPLayer& layer );
    
    /** \brief
      * Appends \a layer to the \ref MIPStageLayers "layers list" and takes it's ownership.
      */
    void appendLayer( MIPLayer* layer );
    
    /** \brief
      * Removes \a layer from the \ref MIPStageLayers "layers list".
      * The ownership is transferred to the caller.
      */
    MIPLayer* removeLayer( const MIPLayer& layer );

    /** \brief
      * Tells number of \ref MIPStageLayers "layers".
      */
    std::size_t layersCount() const;

    /** \brief
      * References the \ref MIPStageLayers "layer" with \a layerIndex.
      */
    MIPLayer& layer( std::size_t layerIndex );

    /** \overload
      */
    const MIPLayer& layer( std::size_t layerIndex ) const;

    /** \brief
      * Clears the \ref MIPStageLayers "layers list".
      */
    void clearLayers();

protected:

    virtual void createVolumeSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler ) override;

    virtual const base::ShaderProgram& acquireShader() override;

    virtual const std::string& uniformName( unsigned int role ) const override;

    virtual void configureShader() override;

    /** \brief
      * Does nothing.
      */
    virtual void configureShader( const base::Renderable& ) override;

}; // MIPStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // MIPSTAGE_H_6014714286
