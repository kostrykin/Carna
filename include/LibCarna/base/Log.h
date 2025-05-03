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

#ifndef LOG_H_6014714286
#define LOG_H_6014714286

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/Singleton.h>
#include <LibCarna/base/noncopyable.h>
#include <functional>
#include <string>
#include <memory>

/** \file   Log.h
  * \brief  Defines \ref Carna::base::Log.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Log
// ----------------------------------------------------------------------------------
    
/** \brief
  * Records log messages. The log writing is delegated to implementations of the
  * \ref Log::Writer interface. The default writer is an instance from
  * \ref Log::StdWriter class.
  *
  * \date   22.2.15 - 6.4.15
  * \author Leonid Kostrykin
  */
class LIBCARNA Log : public Singleton< Log >
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

    NON_COPYABLE
    
    void pushTag( const std::string& tag );
    
    void popTag();

protected:

    friend class Singleton< Log >;

    /** \brief
      * Instantiates.
      */
    Log();

public:

    /** \brief
      * Deletes. Notifies its \ref addOnShutdownListener "shutdown listeners" if they
      * haven't been notified yet.
      */
    virtual ~Log();
    
    /** \brief
      * Tells current log tag.
      */
    const std::string& tag() const;

    /** \brief
      * Describes the severity of a log entry.
      */
    enum Severity
    {
        fatal,   ///< Indicates errors that might lead to misbehaving program logic.
        error,   ///< Indicates errors like memory leaks.
        warning, ///< Indicates warnings.
        debug,   ///< Indicates messages that might be of interest when searching bugs.
        verbose  ///< Indicates statistics and suchlike.
    };
    
    // ------------------------------------------------------------------------------
    // Log :: TagScope
    // ------------------------------------------------------------------------------

    /** \brief
      * Enforces the log to use a specific tag for the duration of the existence of
      * the object from this class. The \ref LIBCARNA_LOG_TAG_SCOPE macro is provided
      * for convenience.
      *
      * \date   6.4.15
      * \author Leonid Kostrykin
      */
    struct LIBCARNA TagScope
    {
        /** \brief
          * Enforces the log to use \a tag for the duration of the existence of the
          * created object.
          */
        explicit TagScope( const std::string& tag );
        
        /** \brief
          * Removes the tag from the log.
          */
        ~TagScope();
        
    private:
    
        const bool valid;
        static void* operator new( std::size_t );   ///< Prevents heap-allocation of scalar objects.
        static void* operator new[]( std::size_t ); ///< Prevents heap-allocation of array of objects.
    };

    // ------------------------------------------------------------------------------
    // Log :: Writer
    // ------------------------------------------------------------------------------

    /** \brief
      * Writes log entries.
      *
      * \date   22.2.15
      * \author Leonid Kostrykin
      */
    class LIBCARNA Writer
    {

        NON_COPYABLE

    public:

        /** \brief
          * Deletes.
          */
        virtual ~Writer();

        /** \brief
          * Logs \a entry with \a severity.
          */
        virtual void write( Severity severity, const std::string& entry ) const = 0;

    }; // Log :: Writer

    // ------------------------------------------------------------------------------
    // Log :: TextWriter
    // ------------------------------------------------------------------------------
    
    /** \brief
      * Abstract implementation of the \ref Writer interface, that formats log
      * messages along with their severity as pure ASCII character chains.
      *
      * \date   22.2.15
      * \author Leonid Kostrykin
      */
    class LIBCARNA TextWriter : public Writer
    {

    public:

        virtual void write( Severity, const std::string& ) const override;

    protected:

        /** \brief
          * Logs \a formattedEntry with \a severity.
          */
        virtual void writeLine( Severity severity, const std::string& formattedEntry ) const = 0;

    }; // Log :: TextWriter

    // ------------------------------------------------------------------------------
    // Log :: StdWriter
    // ------------------------------------------------------------------------------

    /** \brief
      * Writes `fatal` and `error` classified log messages to `std::cerr` and such
      * classified as `warning` or `debug` to `std::cout`.
      *
      * \date   22.2.15
      * \author Leonid Kostrykin
      */
    class LIBCARNA StdWriter : public TextWriter
    {

    protected:

        virtual void writeLine( Severity, const std::string& ) const override;

    }; // Log :: StdWriter

    /** \brief
      * Sets object that log writing will be delegated to.
      */
    void setWriter( Writer* );

    /** \brief
      * Instructs \ref setWriter "current writer" to write \a entry with \a severity.
      */
    void record( Severity severity, const std::string& entry ) const;

    // ------------------------------------------------------------------------------
    // Log :: OnShutdownListener
    // ------------------------------------------------------------------------------

    /** \brief
      * Defines callback interface that is invoked when the \ref Log system is about
      * to shut down.
      *
      * \author Leonid Kostrykin
      * \date   18.3.15
      */
    class LIBCARNA OnShutdownListener
    {

    public:

        /** \brief
          * Does nothing.
          */
        virtual ~OnShutdownListener();

        /** \brief
          * Indicates that `%Log::instance` is about to shut down.
          */
        virtual void onLogShutdown() = 0;

    }; // Log :: OnShutdownListener

    /** \brief
      * Makes \a listener be notified when the log system is about to shut down.
      */
    void addOnShutdownListener( OnShutdownListener& listener );
    
    /** \brief
      * Makes \a listener no longer be notified when the log system is about to shut
      * down.
      */
    void removeOnShutdownListener( const OnShutdownListener& listener );

    /** \brief
      * Notifies the \ref addOnShutdownListener "shutdown listeners" that the log
      * system is about to shut down, if they haven't been notified yet. Usually
      * there should be no necessity to call this method, unless the
      * \ref setWriter "current writer" is becoming invalid. No further messages will
      * be logged.
      */
    void shutdown();

}; // Log



// ----------------------------------------------------------------------------------
// LIBCARNA_LOG_TAG_SCOPE
// ----------------------------------------------------------------------------------

/** \brief
  * Enforces the \ref Carna::base::Log to use particular tag within the local scope.
  */
#define LIBCARNA_LOG_TAG_SCOPE( tag ) \
    const ::Carna::base::Log::TagScope _tagScope##__COUNTER__( tag )



}  // namespace Carna :: base

}  // namespace Carna

#endif // LOG_H_6014714286
