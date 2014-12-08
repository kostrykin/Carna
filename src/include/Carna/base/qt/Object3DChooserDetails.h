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

#ifndef OBJECT3DCHOOSER_DETAILS_H_6014714286
#define OBJECT3DCHOOSER_DETAILS_H_6014714286

/** \file   Object3DChooserDetails.h
  * \brief  Defines \ref Carna::base::qt::Object3DChooser::Details.
  */

#include <Carna/base/qt/Object3DChooser.h>

class QComboBox;

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// Object3DChooser :: Details
// ----------------------------------------------------------------------------------

class Object3DChooser :: Details : public QObject
{

    Q_OBJECT

    NON_COPYABLE

    Object3DChooser& self;

public:

    explicit Details( Object3DChooser& self );


    /** \brief  Internally used widget.
      */
    QComboBox* const cbObject3D;

    /** \brief  References the currently selected Object3D instance.
      */
    model::Object3D* selectedObject3D;

    /** \brief  If \c true, no signals will be emitted when the selection changes.
      */
    bool ignoreSelectionChanges;


public slots:

    /** \brief  Updates the list of Object3D instances presented to the user.
      */
    void rebuildList();

    /** \brief  Occasionally invokes Object3DChooser::rebuildList.
      */
    void occasionallyRebuildList( const Carna::base::model::Object3DEvent& );

    /** \brief  Occasionally emits signals of \c ignoreSelectionChanges is not set to
      *         \c true.
      */
    void processSelectionUpdate( int );

}; // Object3DChooser :: Details



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // OBJECT3DCHOOSER_DETAILS_H_6014714286
