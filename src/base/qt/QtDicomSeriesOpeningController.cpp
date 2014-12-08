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

#include <Carna/base/qt/QtDicomSeriesOpeningController.h>
#include <QMessageBox>
#include <sstream>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// QtDicomSeriesOpeningController
// ----------------------------------------------------------------------------------

bool QtDicomSeriesOpeningController::failedReadFile( const std::string& file, const std::vector< std::string >& succeeded )
{
    failedFiles.push_back( file );
    /*
    emit onFailed( "Failed reading file: '" + QString::fromStdString( file ) + "'" );
    return false;
    */
    return true;
}


void QtDicomSeriesOpeningController::hint( const std::string& hint )
{
    QMessageBox::information( NULL, "", QString::fromStdString( hint ) );
}


void QtDicomSeriesOpeningController::cancel()
{
    if( m_bCanceled || i + 1 == n ) return;

    QMessageBox msgBox;
    msgBox.setIcon( QMessageBox::Warning );
    msgBox.setText( "DicomSeries won't be loaded entirely." );
    msgBox.setInformativeText( "Do you want to proceed with partially loaded datasets?" );
    if( !m_bFailed )
    {
        msgBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No | QMessageBox::Ignore );
        msgBox.setDefaultButton( QMessageBox::Ignore );
        msgBox.setEscapeButton( QMessageBox::Ignore );
    }
    else
    {
        msgBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
        msgBox.setDefaultButton( QMessageBox::Yes );
        msgBox.setEscapeButton( QMessageBox::No );
    }
    switch( msgBox.exec() )
    {

    case QMessageBox::Yes:
        m_bCanceled = true;
        abort();
        break;

    case QMessageBox::No:
        m_bInvalidState = true;
        m_bCanceled = true;
        emit onCanceled();
        abort();
        break;

    default:
    case QMessageBox::Cancel:
        break;

    }
}



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna
