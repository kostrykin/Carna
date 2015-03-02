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
  * \brief  Defines \ref Carna::base::view::FrameRenderer.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// FrameRenderer
// ----------------------------------------------------------------------------------

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

    void setBackgroundColor( math::Vector4f& );
    
    void reshape( unsigned int width, unsigned int height, bool fitSquare );
    
    void render( Camera& cam, Node& root ) const;

    void renderTexture
        ( unsigned int unit
        , bool useDefaultSampler = true
        , bool useDefaultShader = true
        , const std::string uniformName = "colorMap" ) const;

private:

    void render( Camera& cam, Node& root, const Viewport& vp ) const;

}; // FrameRenderer



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // FRAMERENDERER_H_6014714286
