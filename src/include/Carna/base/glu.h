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

#ifndef GLU_H_6014714286
#define GLU_H_6014714286

/** \file   glu.h
  * \brief  Includes \c GL/glu.h and \c windows.h if \c _WIN32 is defined.
  *
  * \author Leonid Kostrykin
  * \date   14.3.2011
  */

#ifdef _WIN32   // ----------------------- Win32 -----------------------

    #include <windows.h>

#endif          // -----------------------------------------------------
#include <GL/glu.h>



#endif // GLU_H_6014714286
