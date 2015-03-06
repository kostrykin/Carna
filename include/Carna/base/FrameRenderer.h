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
#include <Carna/base/noncopyable.h>
#include <Carna/base/math.h>
#include <memory>
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
  *     A frame renderer mainly defines a \em sequence of
  *     \ref RenderStage "rendering stages". Each stage contributes several rendering
  *     instructions to the overall rendering process. Such instructions can be the
  *     rendering of 3D objects or also something completely different, like taking
  *     care of stereoscopic rendering.
  *
  *     The rendering process is made up by the execution of occasionally nested
  *     \em render \em tasks. At the beginning of the process there is always exactly
  *     one render task. It's purpose is to invoke the rendering stages of the frame
  *     renderer successively.
  *
  *     In the most simple case, there is only one render task all the time. An
  *     invocation to \ref FrameRenderer::render triggers it's execution, which in
  *     turn invokes rendering on each render stage once.
  *
  *     However, there is an important concept about render tasks, that allows
  *     realization of more complex rendering processes: Rendering stages may
  *     \ref RenderTask::RenderTask(const RenderTask&, Framebuffer&) "fork" the task
  *     they are invoked by. Such a forked task is basically a clone of the original
  *     one: It continuous processing rendering stages with the next stage that it
  *     was created by. It's purpose usually is to obtain a rendering from the
  *     succeeding stages, store it to a texture and process this texture somehow
  *     afterwards. When finished, the original task continuous with the same.
  *
  * \author Leonid Kostrykin
  * \date   21.2.15 - 6.3.15
  */
class CARNA_LIB FrameRenderer
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:
    
    FrameRenderer( GLContext& glContext, unsigned int width, unsigned int height, bool fitSquare );

    ~FrameRenderer();

    GLContext& glContext() const;

    std::size_t stages() const;
    
    void appendStage( RenderStage* );
    
    void clearStages();
    
    RenderStage& stageAt( std::size_t position ) const;
    
    unsigned int width() const;
    
    unsigned int height() const;

    const Viewport& viewport() const;

    void setBackgroundColor( math::Vector4f& );
    
    void reshape( unsigned int width, unsigned int height, bool fitSquare );
    
    void render( Camera& cam, Node& root ) const;

    struct RenderTextureParams
    {
        unsigned int unit;

        bool useDefaultSampler;

        bool useDefaultShader;

        std::string textureUniformName;

        float alphaFactor;

        RenderTextureParams( unsigned int unit );
    };

    void renderTexture( const RenderTextureParams& ) const;

private:

    void render( Camera& cam, Node& root, const Viewport& vp ) const;

}; // FrameRenderer



}  // namespace Carna :: base

}  // namespace Carna

#endif // FRAMERENDERER_H_6014714286
