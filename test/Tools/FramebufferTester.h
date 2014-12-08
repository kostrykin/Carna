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

#pragma once

#include <Carna/base/OffscreenDisplay.h>
#include <boost/current_function.hpp>
#include <boost/assign/list_of.hpp>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// FramebufferTester
// ----------------------------------------------------------------------------------

class FramebufferTester
{

public:

    FramebufferTester
        ( Carna::base::OffscreenDisplay& display
        , const std::string& expected_image_filename
        , const std::string& failure_image_output_filename );

    void verifyFramebuffer() const;

    Carna::base::OffscreenDisplay& display;

    const std::string expectedImageFilename;

    const std::string failureImageOutputFilename;

private:

    static bool areSimilar( const QImage&, const QImage& );

}; // FramebufferTester



// ----------------------------------------------------------------------------------
// VERIFY_FRAMEBUFFER
// ----------------------------------------------------------------------------------

#define VERIFY_FRAMEBUFFER( display, expected_filename ) \
{ \
    FramebufferTester tester( display, expected_filename, expected_filename ); \
    tester.verifyFramebuffer(); \
}



}  // namespace testing

}  // namespace Carna
