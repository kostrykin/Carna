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

#include <Carna/presets/RayMarchingStage.h>
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

class CARNA_LIB MIPStage : public RayMarchingStage
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    const static unsigned int ROLE_HU_VOLUME = 0;

    MIPStage( unsigned int geometryType );

    virtual ~MIPStage();

    virtual void reshape( const base::FrameRenderer& fr, const base::Viewport& vp ) override;

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

    virtual void createSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler ) override;

    virtual const base::ShaderProgram& loadShader() override;

    virtual const std::string& uniformName( unsigned int role ) const override;

    virtual void configureShader( base::GLContext& ) override;

}; // MIPStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // MIPSTAGE_H_6014714286
