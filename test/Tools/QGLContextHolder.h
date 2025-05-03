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

#pragma once

#include <LibCarna/LibCarna.h>
#include <QGLFormat>

class QGLFormat;
class QGLPixelBuffer;

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// QGLContextHolder
// ----------------------------------------------------------------------------------

class QGLContextHolder
{

    const std::unique_ptr< QGLPixelBuffer > pbuffer;

    const std::unique_ptr< Carna::base::GLContext > glContextWrapper;

    const static QGLFormat format;

    static Carna::base::GLContext* createGLContextWrapper( QGLPixelBuffer& );

public:

    QGLContextHolder();

    Carna::base::GLContext& glContext();

}; // QGLContextHolder



}  // namespace testing

}  // namespace Carna
