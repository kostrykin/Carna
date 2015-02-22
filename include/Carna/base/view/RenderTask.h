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
    
    std::size_t nextRenderStage;

public:

    RenderTask( const FrameRenderer& renderer );

    RenderTask( const FrameRenderer& renderer, Framebuffer& output );
    
    /** \brief
      * Forks \a parent. The result of the fork will be rendered to \a output.
      */
    RenderTask( const RenderTask& parent, Framebuffer& output );

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
