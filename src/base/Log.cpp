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
// Log
// ----------------------------------------------------------------------------------

Log::Log()
    : writer( new Log::StdWriter() )
{
}


void Log::setWriter( Writer* writer )
{
    CARNA_ASSERT( writer != nullptr )
    this->writer.reset( writer );
}


void Log::record( Severity severity, const std::string& message ) const
{
    writer->write( severity, message );
}



// ----------------------------------------------------------------------------------
// Log :: Writer
// ----------------------------------------------------------------------------------

Log::Writer::~Writer()
{
}



// ----------------------------------------------------------------------------------
// Log :: StdWriter
// ----------------------------------------------------------------------------------

void Log::StdWriter::write( Severity severity, const std::string& message ) const
{
    std::ostream& out = ( severity == fatal || severity == error ? std::cerr : std::cout );

    switch( severity )
    {

    case fatal:
        out << "[* FATAL *]";
        break;

    case error:
        out << "[* ERROR *]";
        break;

    case warning:
        out << "[ WARNING ]";
        break;

    case debug:
        out << "[  debug  ]";
        break;

    }

    out << "  " << message << std::endl;
}



}  // namespace Carna :: base

}  // namespace Carna
