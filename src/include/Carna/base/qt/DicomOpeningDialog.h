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

#ifndef DICOMOPENINGDIALOG_H_6014714286
#define DICOMOPENINGDIALOG_H_6014714286

/** \file   DicomOpeningDialog.h
  * \brief  Defines \ref Carna::base::qt::DicomOpeningDialog.
  */

#include <Carna/Carna.h>
#include <Carna/base/model/SeriesLoadingRequest.h>
#include <QDialog>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// DicomOpeningDialog
// ----------------------------------------------------------------------------------

class DicomOpeningDialog : public QDialog
{

    Q_OBJECT

public:

    explicit DicomOpeningDialog( QWidget* parent = nullptr );

    virtual ~DicomOpeningDialog();


    const model::SeriesLoadingRequest& getRequest() const;


private slots:

    void loadingRequested( const Carna::base::model::SeriesLoadingRequest& );


private:

    model::SeriesLoadingRequest request;

}; // DicomOpeningDialog



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // DICOMOPENINGDIALOG_H_6014714286
