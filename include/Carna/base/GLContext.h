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
#include <stack>
#include <memory>

/** \file   GLContext.h
  * \brief  Defines \ref Carna::base::GLContext.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// GLContext
// ----------------------------------------------------------------------------------

/** \brief
  * Wraps and represents an OpenGL context.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 10.3.15
  */
class CARNA_LIB GLContext
{

    static GLContext* myCurrent;

    const ShaderProgram* myShader;

    const std::unique_ptr< RenderState > myRenderState;

protected:

    friend class RenderState;

    std::stack< const RenderState* > renderStates;

public:

    const static unsigned int DEPTH_BUFFER_BIT;
    const static unsigned int COLOR_BUFFER_BIT;

    GLContext( bool isDoubleBuffered );

    virtual ~GLContext();

    const bool isDoubleBuffered;

    static GLContext& current();

    virtual void makeActive() const = 0;

    bool isActive() const;

    void setShader( const ShaderProgram& );

    const ShaderProgram& shader() const;

    void clearBuffers( unsigned int flags );

}; // GLContext



// ----------------------------------------------------------------------------------
// QGLContextAdapter
// ----------------------------------------------------------------------------------

/** \brief
  * If you're using Qt, this class template implements the abstract \ref GLContext
  * class as an adapter to the \c QGLContext class from Qt.
  *
  * Example:
  *     \code
  *     #include <QGLContext>
  *     typedef Carna::base::QGLContextAdapter< QGLContext > GLContext;
  *     \endcode
  */
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



}  // namespace Carna :: base

}  // namespace Carna

#endif // GLCONTEXT_H_6014714286
