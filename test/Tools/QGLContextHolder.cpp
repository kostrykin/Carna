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

#include <Carna/base/GLContext.h>
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
        QGLFormat format = QGLFormat::defaultFormat();
        format.setDoubleBuffer( false );
        format.setVersion( 3, 3 );
        return format;
    }
();


Carna::base::GLContext* QGLContextHolder::createGLContextWrapper( QGLPixelBuffer& pbuffer )
{
    pbuffer.makeCurrent();
    return new Carna::base::QGLContextAdapter< QGLContext >();
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
