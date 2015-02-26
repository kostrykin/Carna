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
  * \brief  Defines \ref Carna::base::view::RenderTask.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// RenderTask
// ----------------------------------------------------------------------------------

class CARNA_LIB RenderTask
{

    Framebuffer* myOutput;
    
    std::size_t nextRenderStage;

    math::Matrix4f viewTransform;

public:

    RenderTask( const FrameRenderer& renderer, const math::Matrix4f& projection, const math::Matrix4f& viewTransform );

    RenderTask( const FrameRenderer& renderer, const math::Matrix4f& projection, const math::Matrix4f& viewTransform, Framebuffer& output );
    
    /** \brief
      * Forks \a parent. The result of the fork will be rendered to \a output.
      */
    RenderTask( const RenderTask& parent, Framebuffer& output );

    const FrameRenderer& renderer;

    const math::Matrix4f& projection;

    void overrideViewTransform( const math::Matrix4f& );
    
    /** \brief
      * Invokes \ref RenderStage::render an all associated scene processors remained.
      */
    void render( const Viewport& vp );
    
    /** \brief
      * Finishes this task prematurely.
      */
    void finish();

}; // RenderTask



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERTASK_H_6014714286
