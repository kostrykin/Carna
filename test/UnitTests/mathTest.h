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

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// mathTest
// ----------------------------------------------------------------------------------

/** \brief
  * Unit-tests of the \ref Carna::base::math namespace.
  *
  * \author Leonid Kostrykin
  * \date   6.3.15
  */
class mathTest : public QObject
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

    /** \brief
      * Test cases for \ref Carna::base::math::epsilon
      */
    void test_epsilon();

    /** \brief
      * Test cases for \ref Carna::base::math::length2
      */
    void test_length2();

    /** \brief
      * Test cases for \ref Carna::base::math::isEqual
      */
    void test_isEqual();

    /** \brief
      * Test cases for \ref Carna::base::math::identity4f
      */
    void test_identity4f();

    /** \brief
      * Test cases for \ref Carna::base::math::zeros
      */
    void test_zeros();

    /** \brief
      * Test cases for \ref Carna::base::math::basis4f
      */
    void test_basis4f();

    /** \brief
      * Test cases for \ref Carna::base::math::translation4f
      */
    void test_translation4f();

    /** \brief
      * Test cases for \ref Carna::base::math::scaling4f
      */
    void test_scaling4f();

    /** \brief
      * Test cases for \ref Carna::base::math::orthogonal3f
      */
    void test_orthogonal3f();

    /** \brief
      * Test cases for \ref Carna::base::math::plane4f
      */
    void test_plane4f();

    /** \brief
      * Test cases for \ref Carna::base::math::round_ui
      */
    void test_round_ui();

    /** \brief
      * Test cases for \ref Carna::base::math::makeEven
      */
    void test_makeEven();

    /** \brief
      * Test cases for \ref Carna::base::math::mix
      */
    void test_mix();
    
    /** \brief
      * Test cases for \ref Carna::base::math::LIBCARNA_FOR_VECTOR3UI
      */
    void test_LIBCARNA_FOR_VECTOR3UI();
    
}; // mathTest



}  // namespace Carna :: testing

}  // namespace Carna
