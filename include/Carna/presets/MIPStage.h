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
  * \image html MIPStageTest/channelReplace.png "exemplary rendering with two channels from the code above"
  *
  * \section MIPStageChannels Channels
  *
  * \todo Add documentation on channel functions.
  *
  * The next code snippet turns the second channel to *additive*-mode:
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

    const static unsigned int ROLE_HU_VOLUME = 0;

    MIPStage( unsigned int geometryType );

    virtual ~MIPStage();

    virtual void reshape( const base::FrameRenderer& fr, unsigned int width, unsigned int height ) override;

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;
    
    /** \brief
      * Swaps positions of \a channel with it's successor in the \ref MIP_Channels "channels list".
      */
    void ascendChannel( const MIPChannel& channel );
    
    /** \brief
      * Appends \a channel to the \ref MIP_Channels "channels list" and takes it's ownership.
      */
    void appendChannel( MIPChannel* channel );
    
    /** \brief
      * Removes \a channel from the \ref MIP_Channels "channels list".
      * The ownership is transferred to the caller.
      */
    MIPChannel* removeChannel( const MIPChannel& channel );

    std::size_t channelsCount() const;

    MIPChannel& channel( std::size_t channelIndex );

    const MIPChannel& channel( std::size_t channelIndex ) const;

    void clearChannels();

protected:

    virtual void createVolumeSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler ) override;

    virtual const base::ShaderProgram& acquireShader() override;

    virtual const std::string& uniformName( unsigned int role ) const override;

    virtual void configureShader() override;

}; // MIPStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // MIPSTAGE_H_6014714286
