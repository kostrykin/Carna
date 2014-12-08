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

#include <Carna/base/Application.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/view/ShaderCompilationError.h>
#include <QMessageBox>
#include <QPushButton>
#include <cstdlib>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Application
// ----------------------------------------------------------------------------------

Application::Application( int argc, char** argv )
    : QApplication( argc, argv )
{
}


bool Application::notify( QObject* receiver, QEvent* ev )
{
    try
    {
        try
        {
            return QApplication::notify( receiver, ev );
        }
        catch( const std::runtime_error& ex )
        {
            throw CarnaException( ex );
        }
        catch( const std::logic_error& ex )
        {
            throw CarnaException( ex );
        }
        catch( const std::bad_alloc& ex )
        {
            throw CarnaException
                ( "Fatal Error"
                , "Not enough memory to finish operation."
                , ex.what() );
        }
        catch( const std::exception& ex )
        {
            throw CarnaException
                ( "Fatal Error"
                , "Fatal error: An unknown exception has occurred."
                , ex.what() );
        }
        catch( const CarnaException& )
        {
            throw;
        }
        catch( ... )
        {
            throw CarnaException( "Fatal Error", "Fatal error: An unknown exception has occurred." );
        }
    }
    catch( CarnaException& ex )
    {
#if 0
        QMessageBox msgBox;

        const bool suppressible = ( dynamic_cast< SuppressibleException* >( &ex ) != nullptr );

        std::stringstream message;
        message << ex.message;

        QMessageBox::StandardButtons buttons = QMessageBox::Ok;

        if( suppressible )
        {
            message
                << std::endl << std::endl
                << "You might want to 'Ignore' this exception in the future.";

            buttons |= QMessageBox::Ignore;
        }

        msgBox.setWindowModality( Qt::ApplicationModal );
        msgBox.setModal( true );
        msgBox.setIcon( suppressible ? QMessageBox::Warning : QMessageBox::Critical );
        msgBox.setWindowTitle( QString::fromStdString( ex.type ) );
        msgBox.setText( QString::fromStdString( std::string( message.str() ) ) );
        msgBox.setDetailedText( QString::fromStdString( ex.details ) );
        msgBox.setStandardButtons( buttons );
        msgBox.setDefaultButton( QMessageBox::Ok );
        msgBox.setEscapeButton( QMessageBox::Ok );

        if( msgBox.exec() == QMessageBox::Ignore )
        {
            static_cast< SuppressibleException& >( ex ).setSuppressed( true );
        }
#endif

        QMessageBox msgBox;

        msgBox.setWindowModality( Qt::ApplicationModal );
        msgBox.setModal( true );
        msgBox.setIcon( QMessageBox::Critical );
        msgBox.setWindowTitle( QString::fromStdString( ex.type ) );
        msgBox.setText( QString::fromStdString( ex.message ) );
        msgBox.setDetailedText( QString::fromStdString( ex.details ) );
        msgBox.setStandardButtons( QMessageBox::Ignore );
        msgBox.setDefaultButton( QMessageBox::Ignore );
        msgBox.setEscapeButton( QMessageBox::Ignore );

        QPushButton* const exitButton = msgBox.addButton( "Exit", QMessageBox::ResetRole );

        msgBox.exec();

        if( msgBox.clickedButton() == exitButton )
        {
            ::exit( EXIT_FAILURE );
        }
    }

    return false;
}



}  // namespace Carna :: base

}  // namespace Carna
