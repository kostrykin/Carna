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

#include <Carna/base/CarnaException.h>
#include <Carna/base/Log.h>
#include <iostream>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// CarnaException :: Details
// ----------------------------------------------------------------------------------

struct CarnaException::Details
{

    std::string whatBuffer;

    void initialize( CarnaException* self );

private:

    void reportException( CarnaException* self );

    void initializeWhatBuffer( CarnaException* self );

}; // CarnaException :: Details


void CarnaException::Details::initialize( CarnaException* self )
{
    initializeWhatBuffer( self );
    reportException( self );
}


void CarnaException::Details::reportException( CarnaException* self )
{
    std::stringstream msg;
    msg << self->type << ": " << self->message << std::endl;
    if( !self->details.empty() )
    {
        msg << self->details;
    }
    Log::instance().record( Log::fatal, msg.str() );
}


void CarnaException::Details::initializeWhatBuffer( CarnaException* self )
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
// CarnaException
// ----------------------------------------------------------------------------------

CarnaException::CarnaException
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


CarnaException::CarnaException
    ( const std::logic_error& error
    , const std::string& details )

    : pimpl( new Details() )
    , type( "Assertion Error" )
    , message( error.what() )
    , details( details )
{
    pimpl->initialize( this );
}


CarnaException::CarnaException
    ( const std::runtime_error& error
    , const std::string& details )

    : pimpl( new Details() )
    , type( "Unhandled Exception" )
    , message( error.what() )
    , details( details )
{
    pimpl->initialize( this );
}


CarnaException::CarnaException( const CarnaException& other )
    : pimpl( new Details() )
    , type( other.type )
    , message( other.message )
    , details( other.details )
{
    pimpl->initialize( this );
}


CarnaException::~CarnaException()
{
}


const char* CarnaException::what() const
{
    return pimpl->whatBuffer.c_str();
}



// ----------------------------------------------------------------------------------
// AssertionFailure
// ----------------------------------------------------------------------------------

AssertionFailure::AssertionFailure( const std::string& details )
    : CarnaException
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



}  // namespace Carna :: base

}  // namespace Carna
