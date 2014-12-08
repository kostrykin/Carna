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

#ifndef GLERROR_H_6014714286
#define GLERROR_H_6014714286

/** \file   glError.h
  * \brief  Defines the \ref REPORT_GL_ERROR macro.
  *
  * \author Leonid Kostrykin
  * \date   16.8.2012
  */



// ----------------------------------------------------------------------------------
// GCC compatibility
// ----------------------------------------------------------------------------------

#ifndef __func__

/** \brief  Overloads the \c __func__ macro with \c __FUNCTION__ for GCC compatibility.
  *
  * \author Leonid Kostrykin
  * \date   16.8.2012
  */
#define __func__ __FUNCTION__

#endif



// ----------------------------------------------------------------------------------
// REPORT_GL_ERROR
// ----------------------------------------------------------------------------------

#ifndef NO_GL_ERROR_CHECKING

    #include <QDebug>

    /** \brief  Defines the implementation of the \ref REPORT_GL_ERROR macro.
      *
      * \author Leonid Kostrykin
      * \date   16.8.2012
      */
    #define __REPORT_GL_ERROR_IMPL { \
            const unsigned int err = glGetError(); \
            if( err ){ qDebug() << "*** " << __func__ << " PRODUCED GL ERROR STATE:" \
                                << QString::fromStdString( std::string( reinterpret_cast< const char* >( gluErrorString( err ) ) ) ) \
                                << "[" << err << "] (" << __FILE__ << ":" << __LINE__ << ")"; } }
#else

    #define __REPORT_GL_ERROR_IMPL

#endif

/** \brief  Queries the GL error state and prints it to the debug output.
  *
  * Define \c NO_GL_ERROR_CHECKING to disable this functionality, i.e. in the release build.
  *
  * \author Leonid Kostrykin
  * \date   16.8.2012
  */
#define REPORT_GL_ERROR __REPORT_GL_ERROR_IMPL



#endif // GLERROR_H_6014714286
