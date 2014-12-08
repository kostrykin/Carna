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

#pragma once

#include <QDebug>
#include <QtGlobal>
#include <iostream>

namespace Carna
{
    /** \namespace  Carna::testing
      * \brief      This namespace contains the test suite components.
      * \todo       Document how the test suite works.
      */
    namespace testing
    {
    }
}

using namespace Carna::testing;



// ----------------------------------------------------------------------------------
// noDebugOutput
// ----------------------------------------------------------------------------------

void noDebugOutput( QtMsgType msgType, const char* msgContent )
{
    switch( msgType )
    {

        case QtDebugMsg:
        {
            break;
        }

        case QtWarningMsg:
        {
            std::cout << "WARNING: " << msgContent << std::endl;
            break;
        }

        case QtCriticalMsg:
        {
            std::cout << "CRITIC.: " << msgContent << std::endl;
            break;
        }

        case QtFatalMsg:
        {
            std::cout << "FATAL  : " << msgContent << std::endl;
            break;
        }

    }
}



// ----------------------------------------------------------------------------------
// disableQDebug
// ----------------------------------------------------------------------------------

void disableQDebug()
{
    qInstallMsgHandler( noDebugOutput );
}



// ----------------------------------------------------------------------------------
// EXPECT_ASSERTION_FAILURE
// ----------------------------------------------------------------------------------

#define EXPECT_ASSERTION_FAILURE( expression ) \
    try \
    { \
        expression; \
        QFAIL( "Expected assertion failure did not raise upon expression." ); \
    } \
    catch( const Carna::base::AssertionFailure& ) \
    { \
    }
