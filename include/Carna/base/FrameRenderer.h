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

#ifndef FRAMERENDERER_H_6014714286
#define FRAMERENDERER_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/RenderStageSequence.h>
#include <Carna/base/Aggregation.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/math.h>
#include <string>

/** \file   FrameRenderer.h
  * \brief  Defines \ref Carna::base::FrameRenderer.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// FrameRenderer
// ----------------------------------------------------------------------------------

/** \brief
  * Defines logic for rendering frames from given scenes.
  *
  * \section RenderingProcess Rendering Process
  *
  * The `%FrameRenderer` mainly defines a *sequence* of
  * \ref RenderStage "rendering stages". Each stage contributes several rendering
  * instructions to the overall rendering process. Such instructions can be the
  * rendering of 3D objects or also something completely different, like taking care
  * of stereoscopic rendering.
  *
  * The rendering process is made up by the execution of occasionally nested *render
  * tasks*. At the beginning of the process there is always exactly one render task.
  * It's purpose is to invoke the rendering stages of the frame renderer
  * successively. The render task API is documented \ref RenderTask "here".
  *
  * In the most simple case, there is only one render task all the time. An
  * invocation to \ref FrameRenderer::render triggers it's execution, which in turn
  * invokes rendering on each render stage once.
  *
  * However, there is an important concept about render tasks, that allows
  * realization of more complex rendering processes: Rendering stages may *fork* the
  * task they are invoked by. Such a forked task is basically a clone of the original
  * one: It continuous processing rendering stages with the next stage that it was
  * created by. It's purpose usually is to obtain a rendering from the succeeding
  * stages, store it to a texture and process this texture somehow afterwards. When
  * finished, the original task continuous with the same.
  *
  * \author Leonid Kostrykin
  * \date   21.2.15 - 6.3.15
  */
class CARNA_LIB FrameRenderer : public RenderStageSequence
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:
    
    /** \brief
      * Instantiates with empty rendering stages sequence.
      *
      * \see
      *     All parameters except \a glContext can be changed via \ref reshape later.
      *
      * \param glContext
      *     OpenGL context wrapper that this renderer will be associated with.
      *
      * \param width
      *     Width of future rendered frames.
      *
      * \param height
      *     Height of future rendered frames.
      *
      * \param fitSquare
      *     If \c true, the viewport will be square-shaped and centered within the
      *     frame. You will also have to update the \ref Camera::setProjection
      *     "projection matrix" w.r.t. to the new aspect ratio of width and height
      *     if \a fitSquare is \c false.
      */
    FrameRenderer( GLContext& glContext, unsigned int width, unsigned int height, bool fitSquare );

    /** \brief
      * Deletes all stages contained by this renderer.
      */
    virtual ~FrameRenderer();

    /** \brief
      * Represents the OpenGL context that this renderer is associated with.
      */
    GLContext& glContext() const;
    
    /** \brief
      * Activates \ref glContext and deletes all stages from the rendering stages
      * sequence.
      */
    virtual void clearStages() override;
    
    /** \brief
      * Tells the current frame width. Value is changed through \ref reshape.
      */
    unsigned int width() const;
    
    /** \brief
      * Tells the current frame height. Value is changed through \ref reshape.
      */
    unsigned int height() const;

    /** \brief
      * Tells the root viewport that frames are currently rendered with.
      * Value is changed through \ref reshape.
      */
    const Viewport& viewport() const;

    /** \brief
      * Sets frame background color. Default value is \ref Color::BLACK_NO_ALPHA.
      */
    void setBackgroundColor( const math::Vector4f& );
    
    /** \brief
      * Sets width, height and root viewport of future rendered frames.
      *
      * \param width
      *     Width of future rendered frames.
      *
      * \param height
      *     Height of future rendered frames.
      *
      * \param fitSquare
      *     If \c true, the viewport will be square-shaped and centered within the
      *     frame. You will also have to update the \ref Camera::setProjection
      *     "projection matrix" w.r.t. to the new aspect ratio of width and height
      *     if \a fitSquare is \c false.
      */
    void reshape( unsigned int width, unsigned int height, bool fitSquare );
    
    /** \overload
      *
      * Takes same value for \a fitSquare that was given last time.
      */
    void reshape( unsigned int width, unsigned int height );
    
    /** \brief
      * Renders scene \a root from \a cam point of view to the currently bound
      * framebuffer by issuing the \ref RenderingProcess "rendering process". The
      * scene graph concept is explained \ref SceneGraph "here".
      */
    void render( Camera& cam, Node& root ) const;
    
    /** \overload
      * 
      * Uses the root of \a cam as the scene root, which needs to be looked up first,
      * which may take a while if \a cam resides deep in the scene graph.
      */
    void render( Camera& cam ) const;

    /** \brief
      * Specifies how \ref renderTexture is to be performed.
      *
      * The \ref unit "texture unit" will be linked with the \ref textureUniformName
      * "specified shader uniform variable", that is set to \c colorMap initially.
      */
    struct RenderTextureParams
    {
        /** \brief
          * The texture unit that \ref renderTexture will instruct the shader to use.
          */
        unsigned int unit;
        
        /** \brief
          * Holds whether a predefined sampler is to be bound to \ref unit before
          * drawing. This predefined sampler uses \ref Sampler::FILTER_LINEAR and
          * \ref Sampler::WRAP_MODE_CLAMP.
          *
          * The default value is \c true.
          */
        bool useDefaultSampler;
        
        /** \brief
          * The currently set shader will be used for drawing if this is \c false.
          * If the value is \c true, the default shader will be bound, that
          * reproduces the texture bound to \ref unit.
          *
          * The default value is \c true.
          */
        bool useDefaultShader;

        /** \brief
          * Holds the name of the uniform variable defined by the shader that will be
          * used, that will be linked with the specified \ref unit "texture unit".
          *
          * The default value is \c colorMap.
          */
        std::string textureUniformName;

        /** \brief
          * If \ref useDefaultShader is \c true, the alpha values sampled from the
          * texture bound to the \ref unit "specified texture unit" will be
          * multiplied with this value. Otherwise this is ignored.
          *
          * The default value is \c 1.
          */
        float alphaFactor;

        /** \brief
          * Instantiates default configuration. This will simply reproduce the
          * texture that is currently bound to texture \a unit on a rectangle that
          * covers the whole viewport.
          */
        RenderTextureParams( unsigned int unit );
    };

    /** \brief
      * Renders rectangle that covers the \em current viewport, according to
      * \a params. Refer to \ref RenderTextureParams for further notes.
      *
      * \warning
      *     Based on the \ref RenderTextureParams::useDefaultShader "particular
      *     values" specified in \a params, this method might change the
      *     \ref GLContext::setShader "active shader".
      */
    void renderTexture( const RenderTextureParams& params ) const;
    
    /** \brief
      * Tells the average of the reciprocal frame rendering time.
      */
    const math::Statistics< double >& framesPerSecond() const;

private:

    void render( Camera& cam, Node& root, const Viewport& vp ) const;

}; // FrameRenderer



}  // namespace Carna :: base

}  // namespace Carna

#endif // FRAMERENDERER_H_6014714286
