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

    const ShaderProgram* myShader;

public:

    GLContext( bool isDoubleBuffered );

    virtual ~GLContext();

    const bool isDoubleBuffered;

    static GLContext& current();

    virtual void makeActive() const = 0;

    bool isActive() const;

    void setShader( const ShaderProgram& );

    const ShaderProgram& shader() const;

}; // GLContext



// ----------------------------------------------------------------------------------
// QGLContextAdapter
// ----------------------------------------------------------------------------------

template< typename QGLContext >
class QGLContextAdapter : public GLContext
{

    QGLContext& qglcontext;

public:

    QGLContextAdapter();

    QGLContextAdapter( const QGLContext& qglcontext );

    virtual void makeActive() const override;

}; // QGLContextAdapter


template< typename QGLContext >
QGLContextAdapter< QGLContext >::QGLContextAdapter()
    : GLContext( QGLContext::currentContext()->format().doubleBuffer() )
    , qglcontext( const_cast< QGLContext& >( *QGLContext::currentContext() ) )
{
}


template< typename QGLContext >
QGLContextAdapter< QGLContext >::QGLContextAdapter( const QGLContext& qglcontext )
    : GLContext( qglcontext.format().doubleBuffer() )
    , qglcontext( const_cast< QGLContext& >( qglcontext ) )
{
}


template< typename QGLContext >
void QGLContextAdapter< QGLContext >::makeActive() const
{
    qglcontext.makeCurrent();
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // GLCONTEXT_H_6014714286
