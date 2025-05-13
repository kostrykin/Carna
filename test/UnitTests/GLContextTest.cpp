/*
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include "GLContextTest.hpp"
#include <LibCarna/base/GLContext.hpp>

namespace LibCarna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// GLContextTest
// ----------------------------------------------------------------------------------

void GLContextTest::initTestCase()
{
}


void GLContextTest::cleanupTestCase()
{
}


void GLContextTest::init()
{
    qglContextHolder.reset( new QGLContextHolder() );
}


void GLContextTest::cleanup()
{
    qglContextHolder.reset();
}


void GLContextTest::test_vendor()
{
    const std::string& vendor = qglContextHolder->glContext().vendor();
    QVERIFY( vendor.size() > 0u );
}


void GLContextTest::test_renderer()
{
    const std::string& renderer = qglContextHolder->glContext().renderer();
    QVERIFY( renderer.size() > 1u );
}



}  // namespace LibCarna :: testing

}  // namespace LibCarna
