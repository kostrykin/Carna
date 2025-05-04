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

#include <LibCarna/base/LibCarnaException.h>
#include <LibCarna/base/Log.h>
#include <iostream>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// LibCarnaException :: Details
// ----------------------------------------------------------------------------------

struct LibCarnaException::Details
{

    std::string whatBuffer;

    void initialize( LibCarnaException* self );

private:

    void reportException( LibCarnaException* self );

    void initializeWhatBuffer( LibCarnaException* self );

}; // LibCarnaException :: Details


void LibCarnaException::Details::initialize( LibCarnaException* self )
{
    initializeWhatBuffer( self );
    reportException( self );
}


void LibCarnaException::Details::reportException( LibCarnaException* self )
{
    std::stringstream msg;
    msg << self->type << ": " << self->message << std::endl;
    if( !self->details.empty() )
    {
        msg << self->details;
    }
    Log::instance().record( Log::fatal, msg.str() );
}


void LibCarnaException::Details::initializeWhatBuffer( LibCarnaException* self )
{
    if( self->details.empty() )
    {
        whatBuffer = self->message;
    }
    else
    {
        std::stringstream ss;
        ss << self->message << std::endl << self->details;
        whatBuffer = ss.str();
    }
}



// ----------------------------------------------------------------------------------
// LibCarnaException
// ----------------------------------------------------------------------------------

LibCarnaException::LibCarnaException
    ( const std::string& type
    , const std::string& message
    , const std::string& details )

    : pimpl( new Details() )
    , type( type )
    , message( message )
    , details( details )
{
    pimpl->initialize( this );
}


LibCarnaException::LibCarnaException
    ( const std::logic_error& error
    , const std::string& details )

    : pimpl( new Details() )
    , type( "Assertion Error" )
    , message( error.what() )
    , details( details )
{
    pimpl->initialize( this );
}


LibCarnaException::LibCarnaException
    ( const std::runtime_error& error
    , const std::string& details )

    : pimpl( new Details() )
    , type( "Unhandled Exception" )
    , message( error.what() )
    , details( details )
{
    pimpl->initialize( this );
}


LibCarnaException::LibCarnaException( const LibCarnaException& other )
    : pimpl( new Details() )
    , type( other.type )
    , message( other.message )
    , details( other.details )
{
    pimpl->initialize( this );
}


LibCarnaException::~LibCarnaException()
{
}


const char* LibCarnaException::what() const
{
    return pimpl->whatBuffer.c_str();
}



// ----------------------------------------------------------------------------------
// AssertionFailure
// ----------------------------------------------------------------------------------

AssertionFailure::AssertionFailure( const std::string& details )
    : LibCarnaException
    ( "Assertion Failed"
    , "An assertion has failed. Refer to details for further information."
    , details )
{
}



// ----------------------------------------------------------------------------------
// BreakForDebug
// ----------------------------------------------------------------------------------

bool BreakForDebug::enabled = true;


BreakForDebug::BreakForDebug()
{
}


void BreakForDebug::enable()
{
    enabled = true;
}


void BreakForDebug::disable()
{
    enabled = false;
}


bool BreakForDebug::isEnabled()
{
    return enabled;
}



}  // namespace LibCarna :: base

}  // namespace LibCarna
