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

#include <Carna/VolumeRenderings/MIP/MIPStage.h>
#include <Carna/base/view/ShaderManager.h>
#include <Carna/base/Matrix4f.h>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// MIPStage
// ----------------------------------------------------------------------------------

void MIPStage::renderPass( base::view::RenderTask& rt, const base::view::Viewport& vp )
{
    RayMarchingStage::renderPass( rt, vp );
}


const base::view::ShaderProgram& MIPStage::loadShader()
{
    return base::view::ShaderManager::instance().acquireShader( "mip" );
}


const std::string& MIPStage::uniformName( unsigned int role ) const
{
    const static std::string ROLE_HU_VOLUME_NAME = "huVolume";
    switch( role )
    {

    case ROLE_HU_VOLUME:
        return ROLE_HU_VOLUME_NAME;

    default:
        CARNA_FAIL( "unknown role" );

    }
}


void MIPStage::configureShader( base::view::GLContext& glc )
{
    base::view::ShaderProgram::putUniform1f( "minIntensity", 0 );
    base::view::ShaderProgram::putUniform1f( "maxIntensity", 1 );
    base::view::ShaderProgram::putUniform4f( "color", base::Vector4f( 1, 1, 1, 1 ) );
}



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
