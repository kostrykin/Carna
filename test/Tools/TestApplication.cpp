/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include <TestApplication.hpp>
#include <LibCarna/base/Log.hpp>
#include <QDebug>

namespace LibCarna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// QDebugLogWriter
// ----------------------------------------------------------------------------------

class QDebugLogWriter : public base::Log::TextWriter
{

public:

    virtual void write( base::Log::Severity severity, const std::string& msg ) const override;

protected:

    virtual void writeLine( base::Log::Severity severity, const std::string& msg ) const override;

};


void QDebugLogWriter::write( base::Log::Severity severity, const std::string& msg ) const
{
    base::Log::TextWriter::write( severity, msg );
    if( severity == base::Log::fatal )
    {
        qFatal( "\n==================================================================\n%s", msg.c_str() );
    }
}


void QDebugLogWriter::writeLine( base::Log::Severity severity, const std::string& msg ) const
{
    switch( severity )
    {

    case base::Log::fatal:
    case base::Log::error:
        qCritical( "%s", msg.c_str() );
        break;

    case base::Log::warning:
        qWarning( "%s", msg.c_str() );
        break;

    case base::Log::debug:
        qDebug( "%s", msg.c_str() );
        break;

    case base::Log::verbose:
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
    LibCarna::base::Log::instance().setWriter( new LibCarna::testing::QDebugLogWriter() );
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
    catch( base::LibCarnaException& ex )
    {
        std::stringstream msg;
        msg << ex.message << " (" << ex.details << ")";
        base::Log::instance().record( base::Log::fatal, msg.str() );
        ::exit( EXIT_FAILURE );
        return false;
    }
}



}  // namespace LibCarna :: testing

}  // namespace LibCarna
