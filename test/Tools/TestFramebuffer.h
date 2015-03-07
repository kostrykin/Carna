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

#pragma once

#include <Carna/Carna.h>
#include <Carna/base/Framebuffer.h>
#include <boost/current_function.hpp>

class QImage;

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// TestFramebuffer
// ----------------------------------------------------------------------------------

class TestFramebuffer
{

    const std::unique_ptr< QImage > frame;
    const std::unique_ptr< base::RenderTexture > renderTexture;
    const std::unique_ptr< base::Framebuffer > fbo;
    const std::unique_ptr< base::Framebuffer::Binding > fboBinding;

    void grabFrame() const;

public:

    base::GLContext& glContext;

    TestFramebuffer( base::GLContext& glContext, unsigned int width, unsigned int height );

    ~TestFramebuffer();

    void verifyFramebuffer( const std::string& signature ) const;

    void verifyFramebuffer
        ( const std::string& expectedImageFilename
        , const std::string& failureImageOutputFilename ) const;

private:

    static bool areSimilar( const QImage&, const QImage& );

    static bool saveActualResult( const QImage& frame, const std::string& filename );

}; // TestFramebuffer



// ----------------------------------------------------------------------------------
// VERIFY_FRAMEBUFFER
// ----------------------------------------------------------------------------------

#define VERIFY_FRAMEBUFFER( testFramebuffer ) \
    ( testFramebuffer ).verifyFramebuffer( BOOST_CURRENT_FUNCTION );



}  // namespace testing

}  // namespace Carna
