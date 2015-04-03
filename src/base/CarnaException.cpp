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

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// CarnaException
// ----------------------------------------------------------------------------------

CarnaException::CarnaException
    ( const std::string& type
    , const std::string& message
    , const std::string& details )

    : type( type )
    , message( message )
    , details( details )
{
}


CarnaException::CarnaException
    ( const std::logic_error& error
    , const std::string& details )

    : type( "Assertion Error" )
    , message( error.what() )
    , details( details )
{
}


CarnaException::CarnaException
    ( const std::runtime_error& error
    , const std::string& details )

    : type( "Unhandled Exception" )
    , message( error.what() )
    , details( details )
{
}


CarnaException::~CarnaException()
{
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
