/*
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#pragma once

namespace LibCarna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// GLContextTest
// ----------------------------------------------------------------------------------

/** \brief
  * Unit-tests of the \ref LibCarna::base::GLContext class.
  *
  * \author Leonid Kostrykin
  */
class GLContextTest : public QObject
{

    Q_OBJECT

private slots:

    /** \brief
      * Called before the first test function is executed.
      */
    void initTestCase();

    /** \brief
      * Called after the last test function is executed.
      */
    void cleanupTestCase();

    /** \brief
      * Called before each test function is executed.
      */
    void init();

    /** \brief
      * Called after each test function is executed.
      */
    void cleanup();

 // ----------------------------------------------------------------------------------

    void test_vendor();
    
    void test_renderer();

 // ---------------------------------------------------------------------------------

private:
    std::unique_ptr< QGLContextHolder > qglContextHolder;
    
}; // GLContextTest



}  // namespace LibCarna :: testing

}  // namespace LibCarna
