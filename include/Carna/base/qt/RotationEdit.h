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

#ifndef ROTATIONEDIT_H_6014714286
#define ROTATIONEDIT_H_6014714286

/** \file   RotationEdit.h
  * \brief  Defines \ref Carna::base::qt::RotationEdit.
  */

#include <Carna/Carna.h>
#include <Carna/base/Transformation.h>
#include <QWidget>

class QMouseEvent;
class QSpinBox;

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// RotationEdit
// ----------------------------------------------------------------------------------

/** \brief  Manipulating rotations in 3D space.
  *
  * \ingroup    Widgets
  *
  * \image  html    RotationEdit01.png
  *
  * \par    User interaction and behavior
  *
  * Pan and tilt can be adjusted by holding the primary mouse button pressed upon the
  * view while moving the cursor around.
  *
  * The scroll wheel can be used to adjust the spin quickly, when not hovering any
  * input field with the cursor; the hovered attribute (pan, tilt or spin) is
  * manipulated else.
  *
  * \par    Details
  *
  * The rotation is described by three degrees of freedom:
  * - rotation around own x-axis, which is called \em spin
  * - rotation around parent z-axis, which is \em tilting the parent xy-plane
  * - rotation around tilted y-axis, which is rotation in parent xy-plane (\em pan)
  *
  * The spin is not visualized by the view.
  *
  * \author Leonid Kostrykin
  * \date   21.7.11
  */
class CARNA_EXPORT RotationEdit : public QWidget
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    explicit RotationEdit( QWidget* parent = 0 );


    /** \brief  Tells currently set rotation in parent xy-plane, in degrees.
      */
    int getPan() const;

    /** \brief  Tells currently set tilt of parent xy-plane, in degrees.
      */
    int getTilt() const;

    /** \brief  Tells currently set rotation along own z-axis, in degrees.
      */
    int getSpin() const;


public slots:

    /** \brief  Sets rotation in parent xy-plane, in degrees.
      */
    void setPan( int deg );

    /** \brief  Sets tilt of parent xy-plane, in degrees.
      */
    void setTilt( int deg );

    /** \brief  Sets rotation along own z-axis, in degrees.
      */
    void setSpin( int deg );


protected:

    /** \brief  Implements user interaction.
      */
    virtual void mousePressEvent( QMouseEvent* ev );

    /** \brief  Implements user interaction.
      */
    virtual void mouseReleaseEvent( QMouseEvent* ev );

    /** \brief  Implements user interaction.
      */
    virtual void mouseMoveEvent( QMouseEvent* ev );

    /** \brief  Implements user interaction.
      */
    virtual void wheelEvent( QWheelEvent* ev );

    /** \brief  Implements user interaction.
      */
    virtual void resizeEvent( QResizeEvent* );


private:

    RotationView* view;

    QSpinBox* sbPan,
            * sbTilt,
            * sbSpin;

    QPoint lastMousePos;
    QPoint mousePressPos;


private slots:

    void stateChanged( int );


signals:

    /** \brief  Emitted when the currently set rotation is changed to \a newRotation through user interaction.
      *
      * This signal is not emitted by \ref setPan, \ref setTilt or \ref setSpin.
      */
    void rotationChanged( const Carna::base::Transformation& newRotation );
    
    /** \brief  Emitted when the currently set rotation is changed through user interaction.
      *
      * This signal is not emitted by \ref setPan, \ref setTilt or \ref setSpin.
      */
    void rotationChanged();

}; // RotationEdit



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // ROTATIONEDIT_H_6014714286
