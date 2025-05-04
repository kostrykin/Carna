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

#ifndef GLEW_H_6014714286
#define GLEW_H_6014714286

/** \file   glew.h
  * \brief  Includes \c GL/glew.h and \c windows.h if \c _WIN32 is defined.
  *         Also defines the \ref LIBCARNA_GLEW_INIT macro.
  *
  * \author Leonid Kostrykin
  */
  
#include <LibCarna/base/Log.h>

#ifdef _WIN32
#   include <windows.h>
#endif

#include <LibCarna/base/GL/glew.h>



// ----------------------------------------------------------------------------------
// LIBCARNA_GLEW_INIT
// ----------------------------------------------------------------------------------

/** \brief
  * Invokes \c glewInit and raises an \ref LibCarna::base::AssertionFailure "AssertionFailure" if it fails.
  */
#define LIBCARNA_GLEW_INIT \
    { \
        const GLenum glew_state = glewInit(); \
        const auto glew_ok = ( glew_state == GLEW_OK ); \
        if( glew_ok ) \
        { \
            LibCarna::base::Log::instance().record( LibCarna::base::Log::debug, "GLEW initialized successfully." ); \
        } \
        else \
        { \
            LIBCARNA_FAIL( "GLEW initialization failed (" << glewGetErrorString( glew_state ) << ", Code: " << glew_state << ")" ); \
        } \
    }



#endif // GLEW_H_6014714286
