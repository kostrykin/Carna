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

#ifndef LIBCARNAEXCEPTION_H_6014714286
#define LIBCARNAEXCEPTION_H_6014714286

/** \file   LibCarnaException.h
  * \brief  Defines
  *             \ref LibCarna::base::LibCarnaException,
  *             \ref LibCarna::base::AssertionFailure.
  */

#include <memory>
#include <LibCarna/LibCarna.hpp>
#include <string>
#include <sstream>
#include <stdexcept>
#include <csignal>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// LibCarnaException
// ----------------------------------------------------------------------------------

/** \brief
  * Base exception class that provides distinct attributes for the \em type (category)
  * of the exception, a \em message and an optional \em details section.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA LibCarnaException
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief  Initializes the attributes plainly from the arguments.
      */
    LibCarnaException( const std::string& type, const std::string& message, const std::string& details = "" );
    
    /** \brief  Initializes the type as ''Assertion Error'' and the message from the given exception.
      */
    LibCarnaException( const std::logic_error& error, const std::string& details = "" );
    
    /** \brief  Initializes the type as ''Unhandled Exception'' and the message from the given exception.
      */
    LibCarnaException( const std::runtime_error& error, const std::string& details = "" );

    LibCarnaException( const LibCarnaException& other );

    virtual ~LibCarnaException();


    /** \brief  Tells the type (category) of this exception.
      */
    const std::string type;

    /** \brief  Tells the message of this exception.
      */
    const std::string message;
    
    /** \brief  Tells the details of this exception. May be empty.
      */
    const std::string details;


    /** \brief  Tells the message and the details of this exception. 
      */
    const char* what() const;

}; // LibCarnaException



// ----------------------------------------------------------------------------------
// AssertionFailure
// ----------------------------------------------------------------------------------

/** \brief
  * Carna exception that indicates a failed assertion.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA AssertionFailure : public LibCarnaException
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
  * Controls whether the \ref LIBCARNA_BREAK_FOR_DEBUG macro produces a break point or not.
  *
  * Default configuration will produce <code>%BreakForDebug::isEnabled() == true</code>.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA BreakForDebug
{

    BreakForDebug();

    static bool enabled;

public:

    /** \brief  Makes future invocations of \ref LIBCARNA_BREAK_FOR_DEBUG will produce break point.
      */
    static void enable();
    
    /** \brief  Prevents future invocations of \ref LIBCARNA_BREAK_FOR_DEBUG from producing break point.
      */
    static void disable();
    
    /** \brief  Tells whether future invocations of \ref LIBCARNA_BREAK_FOR_DEBUG will produce break point.
      */
    static bool isEnabled();

}; // BreakForDebug



// ----------------------------------------------------------------------------------
// LIBCARNA_BREAK_FOR_DEBUG
// ----------------------------------------------------------------------------------

/** \brief
  * Causes a break point in debug mode, does nothing in release mode.
  *
  * \see
  * The behavior in debug mode can be controlled via the \ref LibCarna::base::BreakForDebug "BreakForDebug" class.
  *
  * \author Leonid Kostrykin
  */
#ifndef NDEBUG
    #ifdef _MSC_VER
        #define LIBCARNA_BREAK_FOR_DEBUG \
            if( ::LibCarna::base::BreakForDebug::isEnabled() ) \
            { \
                __debugbreak(); \
            }
    #else
        #define LIBCARNA_BREAK_FOR_DEBUG \
            if( ::LibCarna::base::BreakForDebug::isEnabled() ) \
            { \
                std::raise( SIGINT ); \
            }
    #endif
#else
    #define LIBCARNA_BREAK_FOR_DEBUG
#endif



// ----------------------------------------------------------------------------------
// LIBCARNA_FAIL
// ----------------------------------------------------------------------------------

/** \brief
  * Causes a break point in debug mode and throws an \ref LibCarna::base::AssertionFailure "AssertionFailure".
  *
  * \author Leonid Kostrykin
  */
#define LIBCARNA_FAIL( description ) \
    LIBCARNA_BREAK_FOR_DEBUG \
    { \
        std::stringstream details; \
        details \
            << "Description: " << description << std::endl \
            << "Where: " << __FILE__ << ":" << __LINE__; \
        throw ::LibCarna::base::AssertionFailure( details.str() ); \
    }



// ----------------------------------------------------------------------------------
// LIBCARNA_ASSERT
// ----------------------------------------------------------------------------------

/** \brief
  * If the given expression is \em false, a break point is raised in debug mode and
  * an \ref LibCarna::base::AssertionFailure "AssertionFailure" thrown.
  *
  * \author Leonid Kostrykin
  */
#define LIBCARNA_ASSERT( expression ) \
    if( !( expression ) ) \
    { \
        LIBCARNA_BREAK_FOR_DEBUG \
        std::stringstream details; \
        details \
            << "Failed expression: " << #expression << std::endl \
            << "Where: " << __FILE__ << ":" << __LINE__; \
        throw ::LibCarna::base::AssertionFailure( details.str() ); \
    }



// ----------------------------------------------------------------------------------
// LIBCARNA_ASSERT_EX
// ----------------------------------------------------------------------------------

/** \brief
  * If the given expression is \em false, a break point is raised in debug mode and an
  * \ref LibCarna::base::AssertionFailure "AssertionFailure" thrown, \a description being noted within the exception.
  *
  * \author Leonid Kostrykin
  */
#define LIBCARNA_ASSERT_EX( expression, description ) \
    if( !( expression ) ) \
    { \
        LIBCARNA_BREAK_FOR_DEBUG \
        std::stringstream details; \
        details \
            << "Failed expression: " << #expression << std::endl \
            << "Description: " << description << std::endl \
            << "Where: " << __FILE__ << ":" << __LINE__; \
        throw ::LibCarna::base::AssertionFailure( details.str() ); \
    }



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // LIBCARNAEXCEPTION_H_6014714286
