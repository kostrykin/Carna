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
// ColorMapTest
// ----------------------------------------------------------------------------------

/** \brief
 * Unit-tests of the \ref LibCarna::base::ColorMap class.
 *
 * \author Leonid Kostrykin
 */
class ColorMapTest : public QObject
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

    void test_setMinimumIntensity();

    void test_setMaximumIntensity();
    
}; // ColorMapTest



}  // namespace LibCarna :: testing

}  // namespace LibCarna
