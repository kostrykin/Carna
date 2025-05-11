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

#pragma once

#include <LibCarna/LibCarna.hpp>
#include <QGLFormat>

class QGLFormat;
class QGLPixelBuffer;

namespace LibCarna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// QGLContextHolder
// ----------------------------------------------------------------------------------

class QGLContextHolder
{

    const std::unique_ptr< QGLPixelBuffer > pbuffer;

    const std::unique_ptr< LibCarna::base::GLContext > glContextWrapper;

    const static QGLFormat format;

    static LibCarna::base::GLContext* createGLContextWrapper( QGLPixelBuffer& );

public:

    QGLContextHolder();

    LibCarna::base::GLContext& glContext();

}; // QGLContextHolder



}  // namespace testing

}  // namespace LibCarna
