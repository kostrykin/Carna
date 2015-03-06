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

#ifndef GLEW_H_6014714286
#define GLEW_H_6014714286

/** \file   glew.h
  * \brief  Includes \c GL/glew.h and \c windows.h if \c _WIN32 is defined.
  *         Also defines the \ref CARNA_GLEW_INIT macro.
  *
  * \author Leonid Kostrykin
  * \date   24.2.2011
  */

#ifdef _WIN32
#   include <windows.h>
#endif
#include <GL/glew.h>



// ----------------------------------------------------------------------------------
// CARNA_GLEW_INIT
// ----------------------------------------------------------------------------------

/** \brief
  * Invokes \c glewInit and raises an
  * \ref Carna::base::AssertionFailure "AssertionFailure" if it fails.
  */
#define CARNA_GLEW_INIT \
    { \
        const GLenum glew_state = glewInit(); \
        CARNA_ASSERT( glew_state == GLEW_OK ); \
    }



#endif // GLEW_H_6014714286
