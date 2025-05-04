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

#ifndef GLCONTEXT_H_6014714286
#define GLCONTEXT_H_6014714286

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/CarnaException.h>
#include <LibCarna/base/noncopyable.h>
#include <LibCarna/base/Log.h>
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
  * \date   22.2.15 - 15.3.15
  */
class LIBCARNA GLContext
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

protected:

    /** \brief
      * Instantiates `%GLContext` that represents the *current* OpenGL context.
      */
    explicit GLContext( bool isDoubleBuffered );

    friend class RenderState;
    
    /** \brief
      * Makes \a rs the \ref currentRenderState "current render state".
      */
    void pushRenderState( const RenderState& rs );
    
    /** \brief
      * Restores previous render state.
      */
    void popRenderState();
    
    /** \brief
      * References the \ref pushRenderState "latest render state".
      */
    const RenderState& currentRenderState() const;

public:

    const static unsigned int DEPTH_BUFFER_BIT; ///< Wraps `GL_DEPTH_BUFFER_BIT`.
    const static unsigned int COLOR_BUFFER_BIT; ///< Wraps `GL_COLOR_BUFFER_BIT`

    /** \brief
      * Deletes.
      */
    virtual ~GLContext();

    /** \brief
      * Tells whether the represented OpenGL context uses double buffering.
      */
    const bool isDoubleBuffered;

    /** \brief
      * References the current OpenGL context wrapper.
      */
    static GLContext& current();

    /** \brief
      * Makes the OpenGL context represented by this object the current one.
      */
    void makeCurrent() const;

    /** \brief
      * Tells whether the OpenGL context represented by this object is the current
      * one.
      */
    bool isCurrent() const;

    /** \brief
      * Makes \a shader the current shader of the represented OpenGL context.
      * \pre `isCurrent() == true`
      */
    void setShader( const ShaderProgram& shader );

    /** \brief
      * References the \ref setShader "shader set last".
      * \pre \ref setShader has been called previously.
      */
    const ShaderProgram& shader() const;

    /** \brief
      * Wraps `glClear`. Automatically enables on `glDepthMask` temporarily if the
      * \ref DEPTH_BUFFER_BIT is supplied.
      */
    void clearBuffers( unsigned int flags );
    
protected:

    /** \brief
      * Activates the OpenGL context represented by this object.
      */
    virtual void activate() const = 0;

}; // GLContext



// ----------------------------------------------------------------------------------
// QGLContextAdapter
// ----------------------------------------------------------------------------------

/** \brief
  * If you're using Qt, this class template implements the abstract \ref GLContext class as an adapter to the
  * \c QGLContext class from Qt.
  *
  * Example:
  *
  *     \code
  *     #include <QGLContext>
  *     #include <QGLFormat>
  *     typedef Carna::base::QGLContextAdapter< QGLContext, QGLFormat > GLContext;
  *     \endcode
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 20.3.15
  */
template< typename QGLContext, typename QGLFormat >
class QGLContextAdapter : public GLContext
{

    QGLContext& qglcontext;

public:

    /** \brief
      * Creates \ref GLContext wrapper for the current `QGLContext` object.
      */
    QGLContextAdapter();
    
    /** \brief
      * Holds the recommended format that shall be used to create a `QGLContext`.
      */
    static QGLFormat desiredFormat();
    
protected:

    virtual void activate() const override;

}; // QGLContextAdapter


template< typename QGLContext, typename QGLFormat >
QGLContextAdapter< QGLContext, QGLFormat >::QGLContextAdapter()
    : GLContext( QGLContext::currentContext()->format().doubleBuffer() )
    , qglcontext( const_cast< QGLContext& >( *QGLContext::currentContext() ) )
{
    const QGLFormat& format = QGLContext::currentContext()->format();
    if( format.majorVersion() < 3 || ( format.majorVersion() == 3 && format.minorVersion() < 3 ) )
    {
        std::stringstream msg;
        msg << "OpenGL context version " << format.majorVersion() << "." << format.minorVersion() << " is too low.";
        LIBCARNA_FAIL( msg.str() );
    }
    else
    {
        LIBCARNA_ASSERT( format.profile() != QGLFormat::NoProfile );
        std::stringstream msg;
        msg << "Recognized OpenGL " << format.majorVersion() << "." << format.minorVersion() << " context (";
        msg << ( format.profile() == QGLFormat::CoreProfile ? "core" : "compatibility" ) << " profile)";
        Log::instance().record( Log::debug, msg.str() );
    }
}

    
template< typename QGLContext, typename QGLFormat >
QGLFormat QGLContextAdapter< QGLContext, QGLFormat >::desiredFormat()
{
    QGLFormat format = QGLFormat::defaultFormat();
    format.setVersion( 3, 3 );
    format.setProfile( QGLFormat::CompatibilityProfile );
    return format;
}


template< typename QGLContext, typename QGLFormat >
void QGLContextAdapter< QGLContext, QGLFormat >::activate() const
{
    qglcontext.makeCurrent();
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // GLCONTEXT_H_6014714286
