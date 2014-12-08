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

#ifndef SLICEPLANECONTROLLERUI_H_6014714286
#define SLICEPLANECONTROLLERUI_H_6014714286

/** \file   SlicePlaneControllerUI.h
  * \brief  Defines \ref Carna::SlicePlanes::SlicePlaneControllerUI.
  */

#include <Carna/base/noncopyable.h>
#include <Carna/Carna.h>
#include <QWidget>

namespace Carna
{

namespace SlicePlanes
{



// ----------------------------------------------------------------------------------
// SlicePlanes :: SlicePlaneControllerUI
// ----------------------------------------------------------------------------------

/** \brief  Controller component of \ref SlicePlaneVisualization
  *
  * \image html SlicePlaneController01.png
  *
  * \author Leonid Kostrykin
  * \date   26.5.11 - 18.11.13
  */
class CARNA_EXPORT SlicePlaneControllerUI : public QWidget
{

    Q_OBJECT

    NON_COPYABLE

public:

    /** \brief  Instantiates.
      */
    SlicePlaneControllerUI
        ( SlicePlaneRenderer& renderer
        , QWidget* parent = nullptr );

    /** \brief  Destroys itself.
      */
    virtual ~SlicePlaneControllerUI();


    /** \brief  References the view this component controls.
      */
    SlicePlaneRenderer& renderer;


private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // SlicePlanes :: SlicePlaneControllerUI



}  // namespace Carna :: SlicePlanes

}  // namespace Carna

#endif // SLICEPLANECONTROLLERUI_H_6014714286
