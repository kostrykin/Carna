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

#ifndef GLEW_H_6014714286
#define GLEW_H_6014714286

/** \file   glew.h
  * \brief  Includes \c GL/glew.h and \c windows.h if \c _WIN32 is defined.
  *         Also defines the \ref CARNA_GLEW_INIT macro.
  *
  * \author Leonid Kostrykin
  * \date   24.2.11 - 20.3.15
  */
  
#include <Carna/base/Log.h>

#ifdef _WIN32
#   include <windows.h>
#endif

#include <Carna/base/GL/glew.h>



// ----------------------------------------------------------------------------------
// CARNA_GLEW_INIT
// ----------------------------------------------------------------------------------

/** \brief
  * Invokes \c glewInit and raises an \ref Carna::base::AssertionFailure "AssertionFailure" if it fails.
  */
#define CARNA_GLEW_INIT \
    { \
        const GLenum glew_state = glewInit(); \
        const auto glew_ok = ( glew_state == GLEW_OK ); \
        if( glew_ok ) \
        { \
            Carna::base::Log::instance().record( Carna::base::Log::debug, "GLEW initialized successfully." ); \
        } \
        else \
        { \
            CARNA_FAIL( "GLEW initialization failed (" << glewGetErrorString( glew_state ) << ", Code: " << glew_state << ")" ); \
        } \
    }



#endif // GLEW_H_6014714286
