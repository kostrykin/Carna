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

namespace LibCarna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// mathTest
// ----------------------------------------------------------------------------------

/** \brief
  * Unit-tests of the \ref LibCarna::base::math namespace.
  *
  * \author Leonid Kostrykin
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
      * Test cases for \ref LibCarna::base::math::epsilon
      */
    void test_epsilon();

    /** \brief
      * Test cases for \ref LibCarna::base::math::length2
      */
    void test_length2();

    /** \brief
      * Test cases for \ref LibCarna::base::math::isEqual
      */
    void test_isEqual();

    /** \brief
      * Test cases for \ref LibCarna::base::math::identity4f
      */
    void test_identity4f();

    /** \brief
      * Test cases for \ref LibCarna::base::math::zeros
      */
    void test_zeros();

    /** \brief
      * Test cases for \ref LibCarna::base::math::basis4f
      */
    void test_basis4f();

    /** \brief
      * Test cases for \ref LibCarna::base::math::translation4f
      */
    void test_translation4f();

    /** \brief
      * Test cases for \ref LibCarna::base::math::scaling4f
      */
    void test_scaling4f();

    /** \brief
      * Test cases for \ref LibCarna::base::math::orthogonal3f
      */
    void test_orthogonal3f();

    /** \brief
      * Test cases for \ref LibCarna::base::math::plane4f
      */
    void test_plane4f();

    /** \brief
      * Test cases for \ref LibCarna::base::math::round_ui
      */
    void test_round_ui();

    /** \brief
      * Test cases for \ref LibCarna::base::math::makeEven
      */
    void test_makeEven();

    /** \brief
      * Test cases for \ref LibCarna::base::math::mix
      */
    void test_mix();
    
    /** \brief
      * Test cases for \ref LibCarna::base::math::LIBCARNA_FOR_VECTOR3UI
      */
    void test_LIBCARNA_FOR_VECTOR3UI();
    
}; // mathTest



}  // namespace LibCarna :: testing

}  // namespace LibCarna
