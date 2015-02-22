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

#ifndef GLCONTEXT_H_6014714286
#define GLCONTEXT_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>

/** \file   GLContext.h
  * \brief  Defines \ref Carna::base::view::GLContext.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// GLContext
// ----------------------------------------------------------------------------------

class CARNA_LIB GLContext
{

    static GLContext* myCurrent;

public:

    GLContext( bool isDoubleBuffered );

    virtual ~GLContext();

    const bool isDoubleBuffered;

    static GLContext& current();

    virtual void makeActive() = 0;

    bool isActive() const;

}; // GLContext



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // GLCONTEXT_H_6014714286
