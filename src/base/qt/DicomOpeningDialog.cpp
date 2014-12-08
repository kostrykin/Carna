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

#include <Carna/base/qt/DicomOpeningDialog.h>
#include <Carna/base/qt/DicomController.h>
#include <QVBoxLayout>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// DicomOpeningDialog
// ----------------------------------------------------------------------------------

DicomOpeningDialog::DicomOpeningDialog( QWidget* parent )
    : QDialog( parent )
{
    this->setLayout( new QVBoxLayout() );
    this->layout()->setContentsMargins( 0, 0, 0, 0 );

    DicomController* const controller = new DicomController();

    this->layout()->addWidget( controller );
    controller->setMinimumHeight( 400 );

    connect( controller, SIGNAL( seriesLoadingRequested( const Carna::base::model::SeriesLoadingRequest& ) ),
        this, SLOT( loadingRequested( const Carna::base::model::SeriesLoadingRequest& ) ) );
}


DicomOpeningDialog::~DicomOpeningDialog()
{
}


void DicomOpeningDialog::loadingRequested( const model::SeriesLoadingRequest& request )
{
    this->request = request;
    this->accept();
}


const model::SeriesLoadingRequest& DicomOpeningDialog::getRequest() const
{
    return this->request;
}



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna
