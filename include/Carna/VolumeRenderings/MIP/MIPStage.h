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

#include <Carna/VolumeRenderings/RayMarchingStage.h>
#include <Carna/Carna.h>

/** \file   MIPStage.h
  * \brief  Defines \ref Carna::base::view::MIPStage.
  */

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// MIPStage
// ----------------------------------------------------------------------------------

class CARNA_LIB MIPStage : public RayMarchingStage
{

public:

    const static unsigned int ROLE_HU_VOLUME = 0;

    virtual void renderPass( base::view::RenderTask& rt, const base::view::Viewport& vp ) override;

protected:

    virtual const base::view::ShaderProgram& loadShader() override;

    virtual const std::string& uniformName( unsigned int role ) const override;

    virtual void configureShader( base::view::GLContext& ) override;

}; // MIPStage



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // MIPSTAGE_H_6014714286
