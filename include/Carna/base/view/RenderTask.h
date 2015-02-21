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
#include <Carna/base/Matrix4f.h>

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
    
    const Camera* cam;
    
    std::size_t nextRenderStage;
    
    Matrix4f myWorldViewTransform;

public:

    explicit RenderTask( const FrameRenderer& renderer, const Camera& cam );

    explicit RenderTask( const FrameRenderer& renderer, const Camera& cam, Framebuffer& output );
    
    /** \brief
      * Forks \a parent. The result of the fork will be rendered to \a output.
      */
    RenderTask( RenderTask& parent, Framebuffer& output );
    
    const Camera& camera() const;
    
    const Matrix4f& worldViewTransform() const;

    const FrameRenderer& renderer;
    
    /** \brief
      * Invokes \ref RenderStage::render an all associated scene processors remained.
      */
    void render();
    
    /** \brief
      * Finishes this task prematurely.
      */
    void finish();

}; // RenderTask



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERTASK_H_6014714286
