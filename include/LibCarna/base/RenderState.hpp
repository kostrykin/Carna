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

#ifndef RENDERSTATE_H_6014714286
#define RENDERSTATE_H_6014714286

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/noncopyable.hpp>
#include <memory>

/** \file
  * \brief
  * Defines \ref LibCarna::base::RenderState.
  */

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// RenderState
// ----------------------------------------------------------------------------------

/** \brief
  * Manages the OpenGL render state.
  *
  * Each \ref GLContext "OpenGL context" has a so-called render state. This describes
  * things like whether alpha blending is to be used for rendering primitives or not.
  * The purpose of this class is to reduce the state transitions, that can be quite
  * expansive, and make it easy to enforce a fully-defined state.
  *
  * The mechanism behind this class exploits the fact that the *current* OpenGL
  * context is a thread-local variable as described \ref GLContext "here". This
  * allows us to utilize the execution stack to track the render state:
  *
  * \code
  * void render()
  * {
  *     RenderState rs;
  *     rs.setDepthTest( false );
  *     rs.setBlend( true );
  *     render1();
  *     render2();
  * }
  *
  * void render1()
  * {
  *     RenderState rs;
  *     rs.setDepthTest( false );
  *     rs.setBlendFunction( BlendFunction( GL_ONE, GL_ONE ) );
  *     renderSomething();
  * }
  *
  * void render2()
  * {
  *     RenderState rs;
  *     rs.setDepthTest( false );
  *     renderSomething();
  * }
  * \endcode
  *
  * The constructor derives a new render state, that immediately becomes the
  * *current* one and equals the previous state. The method `render` disables depth
  * testing and enables alpha blending. Whether state transitions actually happen
  * here depends on how the previous state was configured. When `render1` turns off
  * depth testing again, nothing happens, because it already was disabled within the
  * previous state. Same is true for `render2`. The updated blend function of
  * `render1` does not affect `render2`. Note that it is forbidden to modify a render
  * state that is not the current one.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA RenderState
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

protected:

    friend class GLContext;

    /** \brief
      * Creates the root instance of \a glContext. For internal usage only.
      */
    explicit RenderState( GLContext& glContext );

    /** \brief
      * Commits the full state. For internal usage only.
      */
    void commit() const;

public:

    /** \brief
      * Lists supported face culling modes.
      */
    enum CullFace
    {

        /** \brief
          * All polygons will pass the cull test.
          */
        cullNone,
        
        /** \brief
          * Polygons recognized as back-faces will be discarded.
          */
        cullBack,
        
        /** \brief
          * Polygons recognized as front-faces will be discarded.
          */
        cullFront

    }; // CullFace

    /** \brief
      * Derives new render state from the current one of the
      * \ref GLContext "current OpenGL context".
      */
    RenderState();

    /** \brief
      * Restores previous render state.
      */
    ~RenderState();

    /** \brief
      * Enables or disables depth testing.
      */
    void setDepthTest( bool dt );

    /** \brief
      * Enables or disables depth writing.
      */
    void setDepthWrite( bool dw );

    /** \brief
      * Configures how the depth test is to be performed. Depth test only applies if
      * \ref setDepthTest "it is activated."
      *
      * This is the equivalent of `glDepthFunc`.
      */
    void setDepthTestFunction( int dtf );

    /** \brief
      * Enables or disables alpha blending. Alpha blending is configured through
      * \ref setBlendFunction and \ref setBlendEquation.
      */
    void setBlend( bool b );

    /** \brief
      * Configures how alpha blending is to be done. Alpha blending only applies if
      * \ref setBlend "it is activated."
      *
      * This is the equivalent of `glBlendFunc`.
      */
    void setBlendFunction( const BlendFunction& bf );

    /** \brief
      * Sets `glBlendEquation` to \a be.
      */
    void setBlendEquation( int be );

    /** \brief
      * Configures face culling. Also see \ref setFrontFace.
      */
    void setCullFace( CullFace cf );

    /** \brief
      * Sets whether front faces are winded contra-clockwise. Also see
      * \ref setCullFace.
      */
    void setFrontFace( bool ccw );
    
    /** \brief
      * Sets `glPointSize` to \a pointSize if \a point size is non-negative. Enables `GL_PROGRAM_POINT_SIZE` and leaves
      * `glPointSize` unchanged otherwise. This enables support for `gl_PointSize` in shaders.
      */
    void setPointSize( float pointSize );

    /** \brief
      * Sets `glLineWidth` to \a lineWidth if \a lineWidth is positive.
      */
    void setLineWidth( float lineWidth );

private:

    void commitDepthTest() const;

    void commitDepthWrite() const;

    void commitDepthTestFunction() const;

    void commitBlend() const;

    void commitBlendFunction() const;

    void commitBlendEquation() const;

    void commitCullFace() const;

    void commitFrontFace() const;
    
    void commitPointSize() const;

    void commitLineWidth() const;

}; // RenderState



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // RENDERSTATE_H_6014714286
