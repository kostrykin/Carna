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

#ifndef APPLICATION_H_6014714286
#define APPLICATION_H_6014714286

/** \file   Application.h
  * \brief  Defines \ref Carna::base::Application.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <QApplication>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Application
// ----------------------------------------------------------------------------------

/** \brief
  * Extension of \c QApplication that reports uncatched exceptions to the user visually.
  *
  * \date 1.15.13 - 15.3.13
  * \author Leonid Kostrykin
  */
class CARNA_LIB Application : public QApplication
{

    NON_COPYABLE

public:

    /** \brief  Instantiates.
      *
      * The arguments are not processed and just delegated to the constructor of \c QApplication.
      */
    explicit Application( int argc, char** argv );


    /** \brief  Reports uncatched exceptions to the user through \c QMessageBox.
      */
    virtual bool notify( QObject* receiver, QEvent* ev ) override;

}; // Application



}  // namespace Carna :: base

}  // namespace Carna

#endif // APPLICATION_H_6014714286
