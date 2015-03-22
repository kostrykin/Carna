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

#include <Carna/base/Log.h>
#include <Carna/base/CarnaException.h>
#include <iostream>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Log :: Details
// ----------------------------------------------------------------------------------

struct Log::Details
{
    Details();

    std::unique_ptr< Writer > writer;
    std::set< OnShutdownListener* > onShutdownListeners;
    bool isShutDown;
};


Log::Details::Details()
    : writer( new Log::StdWriter() )
    , isShutDown( false )
{
}



// ----------------------------------------------------------------------------------
// Log
// ----------------------------------------------------------------------------------

Log::Log()
    : pimpl( new Details() )
{
}


Log::~Log()
{
    shutdown();
}


void Log::setWriter( Writer* writer )
{
    CARNA_ASSERT( writer != nullptr )
    pimpl->writer.reset( writer );
}


void Log::record( Severity severity, const std::string& message ) const
{
    if( !pimpl->isShutDown )
    {
        pimpl->writer->write( severity, message );
    }
}


void Log::addOnShutdownListener( OnShutdownListener& listener )
{
    if( !pimpl->isShutDown )
    {
        pimpl->onShutdownListeners.insert( &listener );
    }
}


void Log::removeOnShutdownListener( const OnShutdownListener& listener )
{
    if( !pimpl->isShutDown )
    {
        pimpl->onShutdownListeners.erase( const_cast< OnShutdownListener* >( &listener ) );
    }
}


void Log::shutdown()
{
    if( !pimpl->isShutDown )
    {
        pimpl->isShutDown = true;
        std::for_each( pimpl->onShutdownListeners.begin(), pimpl->onShutdownListeners.end(),
            []( OnShutdownListener* listener )
            {
                listener->onLogShutdown();
            }
        );
    }
}



// ----------------------------------------------------------------------------------
// Log :: Writer
// ----------------------------------------------------------------------------------

Log::Writer::~Writer()
{
}



// ----------------------------------------------------------------------------------
// Log :: TextWriter
// ----------------------------------------------------------------------------------

void Log::TextWriter::write( Severity severity, const std::string& message ) const
{
    std::stringstream linePrefix;
    switch( severity )
    {

    case fatal:
        linePrefix << "[! FATAL !]";
        break;

    case error:
        linePrefix << "[* ERROR *]";
        break;

    case warning:
        linePrefix << "[ WARNING ]";
        break;

    case debug:
        linePrefix << "[  debug  ]";
        break;

    }
    linePrefix << "  ";
    std::string msgLine;
    std::stringstream in( message );
    while( std::getline( in, msgLine ) )
    {
        writeLine( severity, linePrefix.str() + msgLine );
    }
}



// ----------------------------------------------------------------------------------
// Log :: StdWriter
// ----------------------------------------------------------------------------------

void Log::StdWriter::writeLine( Severity severity, const std::string& message ) const
{
    std::ostream& out = ( severity == fatal || severity == error ? std::cerr : std::cout );
    out << message << std::endl;
}



// ----------------------------------------------------------------------------------
// Log :: OnShutdownListener
// ----------------------------------------------------------------------------------

Log::OnShutdownListener::~OnShutdownListener()
{
}



}  // namespace Carna :: base

}  // namespace Carna
