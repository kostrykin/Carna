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

#include <Carna/base/ShaderCompilationError.h>
#include <sstream>

namespace Carna
{

namespace base
{


// ----------------------------------------------------------------------------------
// Types & Globals
// ----------------------------------------------------------------------------------

const static std::string EXCEPTION_TYPE = "Shader Compilation Error";
const static std::string EXCEPTION_BASE_MESSAGE = "Shader compilation has failed.";



// ----------------------------------------------------------------------------------
// ShaderCompilationError
// ----------------------------------------------------------------------------------

ShaderCompilationError::ShaderCompilationError( const std::string& details )
    : CarnaException
        ( EXCEPTION_TYPE
        , EXCEPTION_BASE_MESSAGE
        , details )
{
}



// ----------------------------------------------------------------------------------
// ShaderCompilationError
// ----------------------------------------------------------------------------------

static std::string createMessage
    ( const std::string& vertexShaderFile
    , const std::string& fragmentShaderFile )
{
    std::stringstream message;

    message
        << EXCEPTION_BASE_MESSAGE << std::endl << std::endl
        << "Vertex Shader: " << vertexShaderFile << std::endl
        << "Fragment Shader: " << fragmentShaderFile;
    
    return message.str();
}



// ----------------------------------------------------------------------------------
// ShaderCompilationError
// ----------------------------------------------------------------------------------

ShaderCompilationError::ShaderCompilationError
    ( const std::string& details
    , const std::string& vertexShaderFile
    , const std::string& fragmentShaderFile )

    : CarnaException
        ( EXCEPTION_TYPE
        , createMessage( vertexShaderFile, fragmentShaderFile )
        , details )
{
}



}  // namespace Carna :: base

}  // namespace Carna
