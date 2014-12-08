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

#ifndef AXIALPLANE_AXIALPLANECONTROLLERUI_H_6014714286
#define AXIALPLANE_AXIALPLANECONTROLLERUI_H_6014714286

/** \file   AxialPlaneControllerUI.h
  * \brief  Defines \ref Carna::MPR::AxialPlaneControllerUI.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <QWidget>
#include <set>

class QSlider;
class QLabel;

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: AxialPlaneControllerUI
// ----------------------------------------------------------------------------------

/** \brief  Configuration widget for a set of \ref AxialPlaneRenderer "views"
  *
  * \image html AxialPlaneController01.png
  *
  * Refer to \ref MPRExample for an example on how to use this controller UI.
  *
  * \note
  * It is safe to delete an \ref AxialPlaneRenderer instance that is still assigned to an \ref AxialPlaneControllerUI.
  *
  * \author Leonid Kostrykin
  * \date   2010 - 2013
  */
class CARNA_EXPORT AxialPlaneControllerUI : public QWidget
{

    Q_OBJECT

    NON_COPYABLE

public:

    /** \brief  Instantiates.
      */
    explicit AxialPlaneControllerUI( base::model::Scene& );

    /** \brief  Releases acquired resources.
      */
    virtual ~AxialPlaneControllerUI();


    /** \brief  Makes given view be configured by this widget.
      */
    void addRenderer( AxialPlaneRenderer& );

    /** \brief  Removes given view from being configured by this widget.
      */
    void removeRenderer( AxialPlaneRenderer& );


    /** \brief  References the data model.
      */
    base::model::Scene& model;


    /** \brief
      * Tells whether this widget will delete itself right after
      * the last \ref removeRenderer "renderer has been removed".
      */
    bool isDeletedWhenEmpty() const;
    
    /** \brief
      * Sets whether this widget will delete itself right after
      * the last \ref removeRenderer "renderer has been removed".
      *
      * \post   <code>isDeletedWhenEmpty() == deletedWhenEmpty</code>
      */
    void setDeletedWhenEmpty( bool deletedWhenEmpty );


private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // MPR :: AxialPlaneControllerUI



}  // namespace Carna :: MPR

}  // namespace Carna

#endif // AXIALPLANE_AXIALPLANECONTROLLERUI_H_6014714286
