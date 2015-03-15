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
  * \attention
  * If you implement this class, you *must* call \ref shutdownContext from the
  * destructor while the represented OpenGL context is still alive. In particular
  * it must still be legal to call `makeCurrent` on that object.
  *
  * This is a good explanation of the essence of OpenGL contexts:
  *
  * > An OpenGL context represents many things. A context stores all of the state
  * > associated with this instance of OpenGL. [...] Each context can represent a
  * > separate viewable surface, like a window in an application. Contexts can share
  * > many kinds of objects between each other. Any OpenGL object types which are not
  * > containers are sharable, as well as [...] GLSL Objects. [...] In order for any
  * > OpenGL commands to work, a context must be current; all OpenGL commands affect
  * > the state of whichever context is current. The current context is a
  * > thread-local variable, so a single process can have several threads, each of
  * > which has its own current context.
  *
  * Reference: https://www.opengl.org/wiki/OpenGL_Context
  *
  * %Carna assumes that all OpenGL contexts within your application are capable of
  * sharing objects. %Carna does not support multi-threading environments, i.e. all
  * %Carna-related code must be run on the same thread. The states are maintained
  * through the \ref RenderState class.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 14.3.15
  */
class CARNA_LIB GLContext
{

    const ShaderProgram* myShader;

    const std::unique_ptr< RenderState > myRenderState;
    
    bool wasContextShutdown;

protected:

    GLContext( bool isDoubleBuffered );

    /** \brief
      * Tells the base class that the represented OpenGL context is being shut down.
      *
      * If this is the last OpenGL context left within the application, the base
      * class will notify those participants who
      * \ref addOnGLShutdownListener "enrolled" for this notification. This will give
      * them a last chance to properly clean up their OpenGL resources.
      */
    void shutdownContext();

    friend class RenderState;

    std::stack< const RenderState* > renderStates;

public:

    const static unsigned int DEPTH_BUFFER_BIT;
    const static unsigned int COLOR_BUFFER_BIT;

    virtual ~GLContext();

    const bool isDoubleBuffered;

    static GLContext& current();

    virtual void makeCurrent() const = 0;

    bool isCurrent() const;

    void setShader( const ShaderProgram& );

    const ShaderProgram& shader() const;

    void clearBuffers( unsigned int flags );
    
    // ------------------------------------------------------------------------------
    // GLContext :: OnGLShutdownListener
    // ------------------------------------------------------------------------------
    
    struct CARNA_LIB OnGLShutdownListener
    {
        virtual ~OnGLShutdownListener();
        
        /** \brief
          * Notifies that the last remained \lastGLContext is about to shut down.
          * This is also the currently active context.
          */
        virtual void onGLShutdown( const GLContext& lastGLContext ) = 0;
    };
    
    static void addOnGLShutdownListener( OnGLShutdownListener& );
    
    static void removeOnGLShutdownListener( const OnGLShutdownListener& );

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
    
    virtual ~QGLContextAdapter();

    virtual void makeCurrent() const override;

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
QGLContextAdapter< QGLContext >::~QGLContextAdapter()
{
    shutdownContext();
}


template< typename QGLContext >
void QGLContextAdapter< QGLContext >::makeCurrent() const
{
    qglcontext.makeCurrent();
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // GLCONTEXT_H_6014714286
