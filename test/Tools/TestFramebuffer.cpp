/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include "TestFramebuffer.hpp"
#include <LibCarna/base/LibCarnaException.hpp>
#include <LibCarna/base/Framebuffer.hpp>
#include <LibCarna/base/Texture.hpp>
#include <LibCarna/base/math.hpp>
#include <QFileInfo>
#include <regex>

namespace LibCarna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// createRenderTexture
// ----------------------------------------------------------------------------------

static base::Texture< 2 >* createRenderTexture( base::GLContext& glContext )
{
    glContext.makeCurrent();
    return base::Framebuffer::createRenderTexture();
}



// ----------------------------------------------------------------------------------
// TestFramebuffer
// ----------------------------------------------------------------------------------

const double TestFramebuffer::DEFAULT_EPSILON = 0.01;


TestFramebuffer::TestFramebuffer( base::GLContext& glContext, unsigned int width, unsigned int height )
    : frame( new QImage( width, height, QImage::Format_RGB888 ) )
    , renderTexture( createRenderTexture( glContext ) )
    , fbo( new base::Framebuffer( width, height, *renderTexture ) )
    , fboBinding( new base::Framebuffer::Binding( *fbo ) )
    , glContext( glContext )
    , epsilon( DEFAULT_EPSILON )
    , numIgnore( 0 )
{
}


TestFramebuffer::~TestFramebuffer()
{
    glContext.makeCurrent();
}


unsigned int TestFramebuffer::width() const
{
    return frame->width();
}


unsigned int TestFramebuffer::height() const
{
    return frame->height();
}


void TestFramebuffer::grabFrame() const
{
    glContext.makeCurrent();
    base::Framebuffer::MinimalBinding binding( *fbo );

    glReadBuffer( GL_COLOR_ATTACHMENT0_EXT );
    glReadPixels( 0, 0, fbo->width(), fbo->height(), GL_RGB, GL_UNSIGNED_BYTE, frame->bits() );

    *frame = frame->mirrored();
}


void TestFramebuffer::verifyFramebuffer( const std::string& signature ) const
{
    const std::regex expr( ".*::([^:]+)::test_([^\\(]+?)\\(" );
    std::smatch match;
    if( std::regex_search( signature, match, expr ) )
    {
        const std::string filename = std::string( match[ 1 ] ) + "/" + std::string( match[ 2 ] ) + ".png";
        verifyFramebuffer( filename, filename );
    }
    else
    {
        LIBCARNA_FAIL( "Unknown signature format." );
    }
}


void TestFramebuffer::verifyFramebuffer
    ( const std::string& expectedImageFilename
    , const std::string& failureImageOutputFilename ) const
{
    const std::string expectedPath = SOURCE_PATH + "/res/renderings/" + expectedImageFilename;
    const std::string   actualPath = BINARY_PATH + "/test_output/"    + failureImageOutputFilename;

    grabFrame();

    if( !QFileInfo( QString::fromStdString( expectedPath ) ).isFile() )
    {
        if( saveActualResult( *frame, actualPath ) )
        {
            QFAIL( ( "Expected comparison bitmap file missing: \"" + expectedPath +
                "\" Actual results have been written to: \"" + actualPath + "\"" ).c_str() );
        }
        else
        {
            QFAIL( ( "Expected comparison bitmap file missing: \"" + expectedPath +
                "\" Saving actual results FAILED!" ).c_str() );
        }
    }

    const QImage expected( expectedPath.c_str() );
    if( !areSimilar( *frame, expected, epsilon, numIgnore ) )
    {
        if( saveActualResult( *frame, actualPath ) )
        {
            QFAIL( ( "Rendered image differs from expected. Result has been written to: " + actualPath ).c_str() );
        }
        else
        {
            QFAIL( ( "Rendered image differs from expected. Result FAILED to be written to: " + actualPath ).c_str() );
        }
    }
}


bool TestFramebuffer::saveActualResult( const QImage& frame, const std::string& filename )
{
    const QFileInfo actual_path_info( QString::fromStdString( filename ) );
    const QDir actual_path_parent = actual_path_info.dir();
    if( !actual_path_parent.exists() )
    {
        actual_path_parent.mkpath( "." );
    }
    return frame.save( QString::fromStdString( filename ) );
}


bool TestFramebuffer::areSimilar( const QImage& img1, const QImage& img2, double epsilon, std::size_t numIgnore )
{
    if( img1.width() != img2.width() || img1.height() != img2.height() )
    {
        return false;
    }

    std::vector< double > sqErrorList;
    double sqError = 0;
    for( int y = 0; y < img1.height(); ++y )
    for( int x = 0; x < img1.width(); ++x )
    {
        const QRgb rgb1 = img1.pixel( x, y );
        const QRgb rgb2 = img2.pixel( x, y );
        
        const base::math::Vector3i color1( qRed( rgb1 ), qGreen( rgb1 ), qBlue( rgb1 ) );
        const base::math::Vector3i color2( qRed( rgb2 ), qGreen( rgb2 ), qBlue( rgb2 ) );
        
        const double sqError = ( ( color1 - color2 ).cast< float >() / 255 ).squaredNorm();
        sqErrorList.push_back(sqError);
    }

    /* Ignore the `numIgnore` largest errors.
     */
    if( numIgnore > 0 )
    {
        std::sort( sqErrorList.begin(), sqErrorList.end() );
        sqErrorList.erase( sqErrorList.end() - numIgnore, sqErrorList.end() );
    }

    const double sqErrorSum = std::accumulate( sqErrorList.begin(), sqErrorList.end(), 0.0 );
    const double rms = std::sqrt( sqErrorSum / sqErrorList.size() );
    return rms < epsilon;
}



}  // namespace LibCarna :: testing

}  // namespace LibCarna
