/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/base/view/glew.h>
#include <Carna/base/view/glError.h>
#include <Carna/base/view/UInt16TextureUploader.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Text.h>
#include <sstream>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// UInt16TextureUploader
// ----------------------------------------------------------------------------------

void UInt16TextureUploader::uploadTexture( const Vector3ui& size, const void* bufferPtr ) const
{
    REPORT_GL_ERROR;

#ifdef NO_GL_ERROR_CHECKING
    glGetError();
#endif

    glTexImage3D( GL_TEXTURE_3D, 0, GL_INTENSITY16
                , size.x, size.y, size.z
                , 0, GL_RED, GL_UNSIGNED_SHORT, bufferPtr );

    const unsigned int err = glGetError();

    if( err != GL_NO_ERROR )
    {
        std::string err_msg;

        switch( err )
        {
            case GL_INVALID_ENUM:
                err_msg = "invalid enumerator";
                break;

            case GL_INVALID_VALUE:
                err_msg = "invalid value";
                break;

            case GL_INVALID_OPERATION:
                err_msg = "invalid operation";
                break;

            case GL_OUT_OF_MEMORY:
                err_msg = "out of memory";
                break;

            default:
                err_msg = base::Text::lexical_cast< std::string >( err );
        }

        std::stringstream ss;
        ss << "3D texture acquisition failed: "
           << err_msg;

        CARNA_FAIL( ss.str() );
    }
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
