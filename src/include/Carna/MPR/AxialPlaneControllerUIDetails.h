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

#ifndef MPR_AXIALPLANECONTROLLERUI_DETAILS_H_6014714286
#define MPR_AXIALPLANECONTROLLERUI_DETAILS_H_6014714286

/** \file   AxialPlaneControllerDetails.h
  * \brief  Defines \ref Carna::MPR::AxialPlaneControllerUI::Details.
  */

#include <Carna/MPR/AxialPlaneControllerUI.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: AxialPlaneControllerUI :: Details
// ----------------------------------------------------------------------------------

/** \brief  PIMPL-implementation of \ref AxialPlane::AxialPlaneControllerUI "AxialPlaneControllerUI".
  *
  * \author Leonid Kostrykin
  * \date   4.3.12
  */
class CARNA_EXPORT AxialPlaneControllerUI :: Details : public QObject
{

    NON_COPYABLE

    Q_OBJECT

public:

    explicit Details( AxialPlaneControllerUI& self );


    AxialPlaneControllerUI& self;

    QSlider* const slHuv0;
    QLabel* const laHuv0;

    QSlider* const slHuv1;
    QLabel* const laHuv1;

    base::qt::Object3DChooser* const cbFollowedObject;

    bool deletedWhenEmpty;


    typedef std::set< AxialPlaneRenderer* > Renderers;

    Renderers renderers;


public slots:

    void setHuv0( int );

    void setHuv1( int );

    void follow();

    void occasionallyFollow( const Carna::base::model::Object3DEvent& );

    void removeRenderer( QObject* );

}; // MPR :: AxialPlaneControllerUI :: Details



}  // namespace MPR

}  // namespace Carna

#endif // MPR_AXIALPLANECONTROLLERUI_DETAILS_H_6014714286
