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

#ifndef RENDERTASK_H_6014714286
#define RENDERTASK_H_6014714286

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/math.h>

/** \file   RenderTask.h
  * \brief  Defines \ref LibCarna::base::RenderTask.
  */

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// RenderTask
// ----------------------------------------------------------------------------------

/** \brief
  * Invokes the rendering stages of the frame renderer successively.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA RenderTask
{

    Framebuffer* myOutput;
    
    std::size_t nextRenderStage;

    math::Matrix4f myViewTransform;

public:

    /** \brief
      * Instantiates. For internal usage only.
      */
    RenderTask( const FrameRenderer& renderer, const math::Matrix4f& projection, const math::Matrix4f& viewTransform );

    /** \overload
      */
    RenderTask( const FrameRenderer& renderer, const math::Matrix4f& projection, const math::Matrix4f& viewTransform, Framebuffer& output );
    
    /** \brief
      * Forks \a parent. The result of the fork will be rendered to \a output.
      */
    RenderTask( const RenderTask& parent, Framebuffer& output );
    
    /** \brief
      * Forks \a parent.
      */
    explicit RenderTask( const RenderTask& parent );

    /** \brief
      * References the frame renderer.
      */
    const FrameRenderer& renderer;

    /** \brief
      * References the \ref ClippingCoordinates "projection matrix" to be used.
      */
    const math::Matrix4f& projection;

    /** \brief
      * References the \ref ViewSpace "view matrix" to be used.
      */
    const math::Matrix4f& viewTransform() const;

    /** \brief
      * Overrides the \ref ViewSpace "view matrix" to be used for further rendering.
      */
    void overrideViewTransform( const math::Matrix4f& );
    
    /** \brief
      * Invokes \ref renderStage an all associated render stages remained. Disabled
      * stages are skipped.
      */
    void render( const Viewport& vp, unsigned int clearBuffersMask = 0 );
    
    /** \brief
      * Finishes this task prematurely.
      */
    void finish();

protected:

    /** \brief
      * Renders \a rs with \a viewport through \ref RenderStage::renderPass.
      */
    virtual void renderStage( RenderStage& rs, const Viewport& viewport );

}; // RenderTask



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // RENDERTASK_H_6014714286
