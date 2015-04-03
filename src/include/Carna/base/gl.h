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

#ifndef GL_H_6014714286
#define GL_H_6014714286

/** \file   gl.h
  * \brief  Includes \c GL/gl.h and \c windows.h if \c _WIN32 is defined.
  *
  * \author Leonid Kostrykin
  * \date   24.2.2011
  */

#ifdef _WIN32   // ----------------------- Win32 -----------------------

    #include <windows.h>

#endif          // -----------------------------------------------------
#include <GL/gl.h>



#endif GL_H_6014714286
