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

#ifndef OBJECT3DCHOOSER_H_6014714286
#define OBJECT3DCHOOSER_H_6014714286

/** \file   Object3DChooser.h
  * \brief  Defines \ref Carna::base::qt::Object3DChooser.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <QWidget>

class QComboBox;

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// Object3DChooser
// ----------------------------------------------------------------------------------

/** \brief  Visual component that shows existing
  *         \ref Carna::base::model::Object3D "3D objects" and lets the user choose
  *         either one or none.
  *
  * \ingroup    Widgets
  *
  * \image  html    Object3DSelectionWidget01.png
  *
  * \author Leonid Kostrykin
  * \date   19.9.11 - 20.9.11
  */
class CARNA_EXPORT Object3DChooser : public QWidget
{

    Q_OBJECT

    NON_COPYABLE

public:

    /** \brief  Instantiates.
      */
    explicit Object3DChooser
        ( model::Scene& model
        , QWidget* parent = nullptr );

    virtual ~Object3DChooser();


    /** \brief  Tells whether an Object3D instance is selected.
      */
    bool isObject3DSelected() const;

    /** \brief  References the currently selected Object3D instance.
      *
      * \pre    <code>isObject3DSelected() == true</code>
      */
    model::Object3D& selectedObject3D() const;


    /** \brief  References the data model.
      */
    model::Scene& model;


signals:

    /** \brief  Some object has been selected.
      */
    void selectionChanged();

    /** \brief  The given object has been selected.
      */
    void selectionChanged( Carna::base::model::Object3D& );

    /** \brief  No object is selected now.
      */
    void selectionDissolved();


public slots:

    /** \brief  Ensures no object is selected.
      */
    void selectNone();

    /** \brief  Ensures that \a object is selected.
      *
      * \pre \a object is part of \ref model.
      *
      * \post <code>isObject3DSelected() && &selectedObject3D() == &object</code>
      *
      * The pre- and post-conditions have been added in \ref v_2_1_2.
      */
    void selectObject( Carna::base::model::Object3D& object );


private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // Object3DChooser



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // OBJECT3DCHOOSER_H_6014714286
