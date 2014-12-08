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

#include <Carna/Carna.h>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// SceneTest
// ----------------------------------------------------------------------------------

/** \brief  Unit-test of the \ref Carna::base::model::Scene class.
  *
  * \author Leonid Kostrykin
  * \date   24.5.13 - 1.5.14
  */
class SceneTest : public QObject
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

    /** \publicsection
      */

    /** \brief  Data-driven instantiation test case by \ref instantiation_data
      */
    void instantiation();

    /** \brief  Supplies the test data for the \ref instantiation test case
      */
    void instantiation_data();

    /** \brief  Defect test for bug #26
      */
    void instantiation_bug26();
    
    /** \brief  Explicit instantiation test case
      */
    void instantiation_null_volume();
    
    /** \brief  Data-driven test case of \c recommendedWindowingLevel, \c setRecommendedWindowingLevel
      *         and the corresponding signals.
      *
      * The data is supplied by \ref recommendedWindowingLevel_data.
      */
    void recommendedWindowingLevel();
    
    /** \brief  Supplies the test data for the \ref recommendedWindowingLevel test case
      */
    void recommendedWindowingLevel_data();
    
    /** \brief  Data-driven test case of \c recommendedWindowingWidth, \c setRecommendedWindowingWidth
      *         and the corresponding signals.
      *
      * The data is supplied by \ref recommendedWindowingWidth_data.
      */
    void recommendedWindowingWidth();
    
    /** \brief  Supplies the test data for the \ref recommendedWindowingWidth test case
      */
    void recommendedWindowingWidth_data();
    
    /** \brief  Data-driven test case of \c recommendedVoidThreshold, \c setRecommendedVoidThreshold
      *         and the corresponding signals.
      *
      * The data is supplied by \ref recommendedVoidThreshold_data.
      */
    void recommendedVoidThreshold();
    
    /** \brief  Supplies the test data for the \ref recommendedVoidThreshold test case
      */
    void recommendedVoidThreshold_data();
    
    /** \brief  Tests the \c Object3D composition API
      *
      * The entities under test are particularly:
      *
      * - \c Model::objectsCount
      * - \c Model::objectByIndex
      * - \c Object3D::Object3D
      * - \c Object3D::~Object3D
      *
      * The \c objectsChanged signal is also tested.
      */
    void objects3D();

 // ----------------------------------------------------------------------------------

private:

    /** \brief  Returns new \c Scene instance.
      */
    static Carna::base::model::Scene* createScene();

    /** \brief  Supplies the 'huv' test data for data-driven test cases.
      *
      * The selected data samples are chosen as extreme values:
      *
      * - \f$-1024\f$, \f$0\f$ and \f$3071\f$ for the legal \f$[-1024;3071]\f$ class
      * - \f$-1025\f$ for the illegal \f$(-\infty;-1025]\f$ class
      * - \f$3071\f$ for the illegal \f$[3071;\infty)\f$ class
      *
      * Additionally the following samples are tested:
      *
      * - \f$-2000\f$, \f$4000\f$, \f$-321\f$, \f$1234\f$, \f$0\f$
      */
    static void createHuvTestData();

    typedef std::function< void( Carna::base::model::Scene&, int ) > HuvSetter;
    typedef std::function< int( const Carna::base::model::Scene& ) > HuvGetter;

    static void runHuvPropertyTest( const HuvSetter&, const HuvGetter&, const char* signal = nullptr );
    
}; // SceneTest



}  // namespace testing

}  // namespace Carna
