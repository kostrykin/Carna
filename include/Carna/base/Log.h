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

#ifndef LOG_H_6014714286
#define LOG_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/Singleton.h>
#include <Carna/base/noncopyable.h>
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
  * Records log messages.
  *
  * \date   22.2.15
  * \author Leonid Kostrykin
  */
class CARNA_LIB Log : public Singleton< Log >
{

    NON_COPYABLE

protected:

    friend class BaseSingleton;

    Log();

public:

    enum Severity
    {
        fatal, error, warning, debug
    };

    class CARNA_LIB Writer
    {

        NON_COPYABLE

    public:

        virtual ~Writer();

        virtual void write( Severity, const std::string& ) const = 0;

    }; // Writer

    class CARNA_LIB TextWriter : public Writer
    {

    public:

        virtual void write( Severity, const std::string& ) const override;

    protected:

        virtual void writeFormatted( Severity, const std::string& ) const = 0;

    }; // TextWriter

    class CARNA_LIB StdWriter : public TextWriter
    {

    protected:

        virtual void writeFormatted( Severity, const std::string& ) const override;

    }; // StdWriter

    void setWriter( Writer* );

    void record( Severity, const std::string& ) const;

private:

    std::unique_ptr< Writer > writer;

}; // Log



}  // namespace Carna :: base

}  // namespace Carna

#endif // LOG_H_6014714286
