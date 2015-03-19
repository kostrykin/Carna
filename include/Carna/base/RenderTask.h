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

#ifndef RENDERTASK_H_6014714286
#define RENDERTASK_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/math.h>

/** \file   RenderTask.h
  * \brief  Defines \ref Carna::base::RenderTask.
  */

namespace Carna
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
  * \date   22.2.15 - 19.3.15
  */
class CARNA_LIB RenderTask
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
      * References the frame renderer.
      */
    const FrameRenderer& renderer;

    /** References the \ref ClippingCoordinates "projection matrix" to be used.
      */
    const math::Matrix4f& projection;

    /** References the \ref ViewSpace "view matrix" to be used.
      */
    const math::Matrix4f& viewTransform() const;

    /** Overrides the \ref ViewSpace "view matrix" to be used for further rendering.
      */
    void overrideViewTransform( const math::Matrix4f& );
    
    /** \brief
      * Invokes \ref RenderStage::render an all associated scene processors remained.
      * Disabled stages are skipped.
      */
    void render( const Viewport& vp, unsigned int clearBuffersMask = 0 );
    
    /** \brief
      * Finishes this task prematurely.
      */
    void finish();

protected:

    /** Renders \a rs with \a viewport.
      */
    virtual void renderStage( RenderStage& rs, const Viewport& viewport );

}; // RenderTask



}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERTASK_H_6014714286
