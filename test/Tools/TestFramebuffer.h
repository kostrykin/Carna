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
    const std::unique_ptr< base::Texture< 2 > > renderTexture;
    const std::unique_ptr< base::Framebuffer > fbo;
    const std::unique_ptr< base::Framebuffer::Binding > fboBinding;

    void grabFrame() const;
    
    double myEpsilon;

public:

    const static double DEFAULT_EPSILON;

    base::GLContext& glContext;

    TestFramebuffer( base::GLContext& glContext, unsigned int width, unsigned int height );

    ~TestFramebuffer();

    unsigned int width() const;

    unsigned int height() const;
    
    void setEpsilon( double epsilon );
    
    double epsilon() const;

    void verifyFramebuffer( const std::string& signature ) const;

    void verifyFramebuffer
        ( const std::string& expectedImageFilename
        , const std::string& failureImageOutputFilename ) const;

private:

    static bool areSimilar( const QImage&, const QImage&, double epsilon );

    static bool saveActualResult( const QImage& frame, const std::string& filename );

}; // TestFramebuffer



// ----------------------------------------------------------------------------------
// VERIFY_FRAMEBUFFER
// ----------------------------------------------------------------------------------

#define VERIFY_FRAMEBUFFER_EX( testFramebuffer, name ) \
    ( testFramebuffer ).verifyFramebuffer( BOOST_CURRENT_FUNCTION );

#define VERIFY_FRAMEBUFFER( testFramebuffer ) \
    VERIFY_FRAMEBUFFER_EX( testFramebuffer, BOOST_CURRENT_FUNCTION );



}  // namespace testing

}  // namespace Carna
