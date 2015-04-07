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

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// FrameRendererIntegrationTest
// ----------------------------------------------------------------------------------

/** \brief
  * Integration-tests of the \ref Carna::base::FrameRenderer class.
  *
  * \author Leonid Kostrykin
  * \date   22.2.14 - 4.4.15
  */
class FrameRendererIntegrationTest : public QObject
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

 // ---------------------------------------------------------------------------------

    void test_typical();

 // ---------------------------------------------------------------------------------

private:

}; // FrameRendererIntegrationTest



}  // namespace testing

}  // namespace Carna
