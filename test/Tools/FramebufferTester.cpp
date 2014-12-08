#include "FramebufferTester.h"
#include <QFileInfo>



// ----------------------------------------------------------------------------------
// FramebufferTester
// ----------------------------------------------------------------------------------

FramebufferTester::FramebufferTester
    ( Carna::base::OffscreenDisplay& display
    , const std::string& expected_image_filename
    , const std::string& failure_image_output_filename )

    : display( display )
    , expectedImageFilename( expected_image_filename )
    , failureImageOutputFilename( failure_image_output_filename )
{
}


void FramebufferTester::verifyFramebuffer() const
{
    const std::string expected_path = SOURCE_PATH + "/res/renderings/" + expectedImageFilename;
    const std::string actual_path = BINARY_PATH + "/test_output/" + failureImageOutputFilename;

    if( !QFileInfo( QString::fromStdString( expected_path ) ).isFile() )
    {
        QFAIL( ( "Expected comparison bitmap file missing: \"" + expected_path + "\"" ).c_str() );
    }

    const QImage& actual = display.framebuffer();
    const QImage expected( expected_path.c_str() );
    if( !areSimilar( actual, expected ) )
    {
        const QFileInfo actual_path_info( QString::fromStdString( actual_path ) );
        const QDir actual_path_parent = actual_path_info.dir();
        if( !actual_path_parent.exists() )
        {
            actual_path_parent.mkpath( "." );
        }
        if( actual.save( QString::fromStdString( actual_path ) ) )
        {
            QFAIL( ( "Rendered image differs from expected. Result has been written to: " + actual_path ).c_str() );
        }
        else
        {
            QFAIL( ( "Rendered image differs from expected. Result FAILED to be written to: " + actual_path ).c_str() );
        }
    }
}


bool FramebufferTester::areSimilar( const QImage& img1, const QImage& img2 )
{
    if( img1.width() != img2.width() || img1.height() != img2.height() )
    {
        return false;
    }

    for( int y = 0; y < img1.height(); ++y )
    for( int x = 0; x < img1.width(); ++x )
    {
        const QRgb color1 = img1.pixel( x, y );
        const QRgb color2 = img2.pixel( x, y );
        if( color1 != color2 )
        {
            return false;
        }
    }

    return true;
}
