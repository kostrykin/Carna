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

#ifndef ROTATIONVIEW_H_6014714286
#define ROTATIONVIEW_H_6014714286

/** \file   RotationView.h
  * \brief  Defines \ref Carna::base::qt::RotationView.
  */

#include <Carna/Carna.h>
#include <Carna/base/Transformation.h>
#include <QWidget>

class QPaintEvent;

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// RotationView
// ----------------------------------------------------------------------------------

/** \brief  Visualizes rotations in 3D space.
  *
  * \ingroup    Widgets
  *
  * Example:
  *
  * \image  html    RotationView01.png
  *
  * \author Leonid Kostrykin
  * \date   21.7.11
  */
class CARNA_EXPORT RotationView : public QWidget
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      *
      * The initially set rotation is an identity matrix.
      * The horizontal plane is visualized by default.
      */
    explicit RotationView( QWidget* parent = 0 );


    /** \brief  Tells the current rotation.
      */
    const Transformation& getRotation() const
    {
        return rotation;
    }


    /** \brief  Tells whether the horizontal plane is visualized.
      */
    bool showsHorizontalPlane() const
    {
        return showHorizontalPlane;
    }


public slots:

    /** \brief  Sets current rotation.
      *
      * Posts a repaint event.
      */
    void setRotation( const Carna::base::Transformation& rotation )
    {
        this->rotation = rotation;
        this->update();
    }


    /** \brief  Sets whether the horizontal plane is visualized.
      */
    void setShowHorizontalPlane( bool b )
    {
        showHorizontalPlane = b;
    }


protected:

    /** \brief  Paints the widget.
      */
    virtual void paintEvent( QPaintEvent* );


private:

    Transformation rotation;

    bool showHorizontalPlane;

}; // RotationView



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // ROTATIONVIEW_H_6014714286
