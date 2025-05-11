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

#pragma once

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/Framebuffer.hpp>
#include <boost/current_function.hpp>

class QImage;

namespace LibCarna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// TestFramebuffer
// ----------------------------------------------------------------------------------

class TestFramebuffer
{

    const std::unique_ptr< QImage > frame;
    const std::unique_ptr< base::Texture< 2 > > renderTexture;
    const std::unique_ptr< base::Framebuffer > fbo;
    const std::unique_ptr< base::Framebuffer::Binding > fboBinding;

    void grabFrame() const;

public:

    const static double DEFAULT_EPSILON;

    base::GLContext& glContext;
    
    double epsilon;
    
    std::size_t numIgnore;

    TestFramebuffer( base::GLContext& glContext, unsigned int width, unsigned int height );

    ~TestFramebuffer();

    unsigned int width() const;

    unsigned int height() const;

    void verifyFramebuffer( const std::string& signature ) const;

    void verifyFramebuffer
        ( const std::string& expectedImageFilename
        , const std::string& failureImageOutputFilename ) const;

private:

    static bool areSimilar( const QImage&, const QImage&, double epsilon, std::size_t n_ignore = 0 );

    static bool saveActualResult( const QImage& frame, const std::string& filename );

}; // TestFramebuffer



// ----------------------------------------------------------------------------------
// VERIFY_FRAMEBUFFER
// ----------------------------------------------------------------------------------

#define VERIFY_FRAMEBUFFER_EX( testFramebuffer, name ) \
    ( testFramebuffer ).verifyFramebuffer( name );

#define VERIFY_FRAMEBUFFER( testFramebuffer ) \
    VERIFY_FRAMEBUFFER_EX( testFramebuffer, BOOST_CURRENT_FUNCTION );



}  // namespace testing

}  // namespace LibCarna
