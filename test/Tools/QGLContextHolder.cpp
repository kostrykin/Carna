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

#include <LibCarna/base/GLContext.h>
#include <QGLContextHolder.h>
#include <QGLPixelBuffer>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// QGLContextHolder
// ----------------------------------------------------------------------------------

const QGLFormat QGLContextHolder::format = []()->QGLFormat
    {
        QGLFormat format = Carna::base::QGLContextAdapter< QGLContext, QGLFormat >::desiredFormat();
        format.setDoubleBuffer( false );
        return format;
    }
();


Carna::base::GLContext* QGLContextHolder::createGLContextWrapper( QGLPixelBuffer& pbuffer )
{
    const bool success = pbuffer.makeCurrent();
    LIBCARNA_ASSERT( success );
    return new Carna::base::QGLContextAdapter< QGLContext, QGLFormat >();
}


QGLContextHolder::QGLContextHolder()
    : pbuffer( new QGLPixelBuffer( 8, 8, format ) )
    , glContextWrapper( createGLContextWrapper( *pbuffer ) )
{
}


Carna::base::GLContext& QGLContextHolder::glContext()
{
    return *glContextWrapper;
}



}  // namespace Carna :: testing

}  // namespace Carna
