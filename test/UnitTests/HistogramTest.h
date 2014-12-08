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

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// HistogramTest
// ----------------------------------------------------------------------------------

/** \brief  Unit-test of the \ref Carna::base::model::Histogram class template.
  *
  * \author Leonid Kostrykin
  * \date   19.1.14
  */
class HistogramTest : public QObject
{

    Q_OBJECT

private slots:

    /** \brief  Called before the first test function is executed.
      */
    void initTestCase();

    /** \brief  Called after the last test function is executed.
      */
    void cleanupTestCase();

    /** \brief  Called before each test function is executed
      */
    void init();

    /** \brief  Called after each test function is executed
      */
    void cleanup();

 // ----------------------------------------------------------------------------------

    /** \brief  Test case for \c Histogram with a \c IndexedCounter
      */
    void testWithIndexedCounter();

    /** \brief  Test case for \c Histogram with a \c AssociativeCounter
      */
    void testWithAssociativeCounter();
    
}; // HistogramTest



}  // namespace testing

}  // namespace Carna
