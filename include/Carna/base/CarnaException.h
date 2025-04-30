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

#ifndef CARNAEXCEPTION_H_6014714286
#define CARNAEXCEPTION_H_6014714286

/** \file   CarnaException.h
  * \brief  Defines
  *             \ref Carna::base::CarnaException,
  *             \ref Carna::base::AssertionFailure.
  */

#include <Carna/Carna.h>
#include <string>
#include <sstream>
#include <stdexcept>
#include <csignal>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// CarnaException
// ----------------------------------------------------------------------------------

/** \brief
  * Base exception class that provides distinct attributes for the \em type (category)
  * of the exception, a \em message and an optional \em details section.
  *
  * \author Leonid Kostrykin
  * \date   5.3.13 - 19.3.13
  */
class CARNA_LIB CarnaException
{

public:

    /** \brief  Initializes the attributes plainly from the arguments.
      */
    CarnaException( const std::string& type, const std::string& message, const std::string& details = "" );
    
    /** \brief  Initializes the type as ''Assertion Error'' and the message from the given exception.
      */
    CarnaException( const std::logic_error& error, const std::string& details = "" );
    
    /** \brief  Initializes the type as ''Unhandled Exception'' and the message from the given exception.
      */
    CarnaException( const std::runtime_error& error, const std::string& details = "" );

    virtual ~CarnaException();


    /** \brief  Tells the type (category) of this exception.
      */
    const std::string type;

    /** \brief  Tells the message of this exception.
      */
    const std::string message;
    
    /** \brief  Tells the details of this exception. May be empty.
      */
    const std::string details;

}; // CarnaException



// ----------------------------------------------------------------------------------
// AssertionFailure
// ----------------------------------------------------------------------------------

/** \brief
  * Carna exception that indicates a failed assertion.
  *
  * \author Leonid Kostrykin
  * \date   5.3.13 - 19.3.13
  */
class CARNA_LIB AssertionFailure : public CarnaException
{

public:

    /** \brief  Instantiates.
      *
      * \param details
      * describes the failed assertion.
      */
    explicit AssertionFailure( const std::string& details );

}; // AssertionFailure



// ----------------------------------------------------------------------------------
// BreakForDebug
// ----------------------------------------------------------------------------------

/** \brief
  * Controls whether the \ref CARNA_BREAK_FOR_DEBUG macro produces a break point or not.
  *
  * Default configuration will produce <code>%BreakForDebug::isEnabled() == true</code>.
  *
  * \author Leonid Kostrykin
  * \date   27.5.13
  */
class CARNA_LIB BreakForDebug
{

    BreakForDebug();

    static bool enabled;

public:

    /** \brief  Makes future invocations of \ref CARNA_BREAK_FOR_DEBUG will produce break point.
      */
    static void enable();
    
    /** \brief  Prevents future invocations of \ref CARNA_BREAK_FOR_DEBUG from producing break point.
      */
    static void disable();
    
    /** \brief  Tells whether future invocations of \ref CARNA_BREAK_FOR_DEBUG will produce break point.
      */
    static bool isEnabled();

}; // BreakForDebug



// ----------------------------------------------------------------------------------
// CARNA_BREAK_FOR_DEBUG
// ----------------------------------------------------------------------------------

/** \brief
  * Causes a break point in debug mode, does nothing in release mode.
  *
  * \see
  * The behavior in debug mode can be controlled via the \ref Carna::base::BreakForDebug "BreakForDebug" class.
  *
  * \author Leonid Kostrykin
  * \date   5.3.13
  */
#ifndef NDEBUG
    #ifdef _MSC_VER
        #define CARNA_BREAK_FOR_DEBUG \
            if( ::Carna::base::BreakForDebug::isEnabled() ) \
            { \
                __debugbreak(); \
            }
    #else
        #define CARNA_BREAK_FOR_DEBUG \
            if( ::Carna::base::BreakForDebug::isEnabled() ) \
            { \
                std::raise( SIGINT ); \
            }
    #endif
#else
    #define CARNA_BREAK_FOR_DEBUG
#endif



// ----------------------------------------------------------------------------------
// CARNA_FAIL
// ----------------------------------------------------------------------------------

/** \brief
  * Causes a break point in debug mode and throws an \ref Carna::base::AssertionFailure "AssertionFailure".
  *
  * \author Leonid Kostrykin
  * \date   5.3.13
  */
#define CARNA_FAIL( description ) \
    CARNA_BREAK_FOR_DEBUG \
    { \
        std::stringstream details; \
        details \
            << "Description: " << description << std::endl \
            << "Where: " << __FILE__ << ":" << __LINE__; \
        throw ::Carna::base::AssertionFailure( details.str() ); \
    }



// ----------------------------------------------------------------------------------
// CARNA_ASSERT
// ----------------------------------------------------------------------------------

/** \brief
  * If the given expression is \em false, a break point is raised in debug mode and
  * an \ref Carna::base::AssertionFailure "AssertionFailure" thrown.
  *
  * \author Leonid Kostrykin
  * \date   5.3.13
  */
#define CARNA_ASSERT( expression ) \
    if( !( expression ) ) \
    { \
        CARNA_BREAK_FOR_DEBUG \
        std::stringstream details; \
        details \
            << "Failed expression: " << #expression << std::endl \
            << "Where: " << __FILE__ << ":" << __LINE__; \
        throw ::Carna::base::AssertionFailure( details.str() ); \
    }



// ----------------------------------------------------------------------------------
// CARNA_ASSERT_EX
// ----------------------------------------------------------------------------------

/** \brief
  * If the given expression is \em false, a break point is raised in debug mode and an
  * \ref Carna::base::AssertionFailure "AssertionFailure" thrown, \a description being noted within the exception.
  *
  * \author Leonid Kostrykin
  * \date   5.3.13
  */
#define CARNA_ASSERT_EX( expression, description ) \
    if( !( expression ) ) \
    { \
        CARNA_BREAK_FOR_DEBUG \
        std::stringstream details; \
        details \
            << "Failed expression: " << #expression << std::endl \
            << "Description: " << description << std::endl \
            << "Where: " << __FILE__ << ":" << __LINE__; \
        throw ::Carna::base::AssertionFailure( details.str() ); \
    }



}  // namespace Carna :: base

}  // namespace Carna

#endif // CARNAEXCEPTION_H_6014714286
