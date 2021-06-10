/*
 *  Copyright (C) 2021 Leonid Kostrykin
 *
 */

#pragma once

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// HUVTest
// ----------------------------------------------------------------------------------

/** \brief
  * Unit-tests of the \ref Carna::base::HUV class.
  *
  * \author Leonid Kostrykin
  * \date   June 2021
  */
class HUVTest : public QObject
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

    void test_HUV_rel();
    
    void test_HUV_abs();
    
}; // HUVTest



}  // namespace Carna :: testing

}  // namespace Carna
