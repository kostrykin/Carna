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

#ifndef SLICEPLANES_SLICEPLANECONTROLLERUI_DETAILS_H_6014714286
#define SLICEPLANES_SLICEPLANECONTROLLERUI_DETAILS_H_6014714286

/** \file   SlicePlaneControllerDetails.h
  * \brief  Defines \ref Carna::SlicePlanes::SlicePlaneControllerUI::Details.
  */

#include <Carna/SlicePlanes/SlicePlaneControllerUI.h>

class QLabel;
class QLineEdit;
class QDoubleSpinBox;

namespace Carna
{

namespace SlicePlanes
{



// ----------------------------------------------------------------------------------
// SlicePlaneControllerUI :: Details
// ----------------------------------------------------------------------------------

class SlicePlaneControllerUI :: Details : public QWidget
{

    Q_OBJECT

    NON_COPYABLE

    SlicePlaneControllerUI& self;

public:

    explicit Details( SlicePlaneControllerUI& );

    virtual ~Details();


    QLineEdit* lineEdit_n1;

    QLineEdit* lineEdit_n2;

    QLineEdit* lineEdit_n3;

    QDoubleSpinBox* sbDistance;

    QLineEdit* lineEdit_border;

    QLabel* laFailure;


public slots:

    void commit();

    void commit( double )
    {
        commit();
    }

}; // SlicePlaneControllerUI :: Details



}  // namespace Carna :: SlicePlanes

}  // namespace Carna

#endif // SLICEPLANES_SLICEPLANECONTROLLERUI_DETAILS_H_6014714286
