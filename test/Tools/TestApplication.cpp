/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <TestApplication.h>
#include <Carna/base/Log.h>
#include <QDebug>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// QDebugLogWriter
// ----------------------------------------------------------------------------------

class QDebugLogWriter : public base::Log::TextWriter
{

protected:

    virtual void writeFormatted( base::Log::Severity, const std::string& ) const override;

};


void QDebugLogWriter::writeFormatted( base::Log::Severity severity, const std::string& msg ) const
{
    switch( severity )
    {

    case base::Log::fatal:
    case base::Log::error:
        qFatal( "%s", msg.c_str() );
        break;

    case base::Log::warning:
        qWarning( "%s", msg.c_str() );
        break;

    case base::Log::debug:
        qDebug( "%s", msg.c_str() );
        break;

    }
}



// ----------------------------------------------------------------------------------
// TestApplication
// ----------------------------------------------------------------------------------

TestApplication::TestApplication( int& argc, char** argv )
    : QApplication( argc, argv )
{
    Carna::base::Log::instance().setWriter( new Carna::testing::QDebugLogWriter() );
}


TestApplication::~TestApplication()
{
    /* We need to do this as long as QApplication is still alive, so that QDebug is
     * also still available.
     */
    base::Log::instance().shutdown();
}


bool TestApplication::notify( QObject* receiver, QEvent* ev )
{
    try
    {
        return QApplication::notify( receiver, ev );
    }
    catch( base::CarnaException& ex )
    {
        std::stringstream msg;
        msg << ex.message << " (" << ex.details << ")";
        base::Log::instance().record( base::Log::fatal, msg.str() );
        ::exit( EXIT_FAILURE );
        return false;
    }
}



}  // namespace Carna :: testing

}  // namespace Carna
