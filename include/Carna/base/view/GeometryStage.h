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

#ifndef GEOMETRYSTAGE_H_6014714286
#define GEOMETRYSTAGE_H_6014714286

#include <Carna/base/view/RenderStage.h>
#include <Carna/base/view/RenderQueue.h>
#include <Carna/base/Matrix4f.h>
#include <memory>

/** \file   GeometryStage.h
  * \brief  Defines \ref Carna::base::view::GeometryStage.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// GeometryStage
// ----------------------------------------------------------------------------------

template< typename RenderableCompare >
class CARNA_LIB GeometryStage : public RenderStage
{
    
    RenderQueue< RenderableCompare > rq;
    bool initialized;
    Node* root;
    std::size_t passesRendered;

public:

    GeometryStage( int geometryType );
    
    virtual void reshape( unsigned int width, unsigned int height ) override;
    
    virtual bool isInitialized() const override;

    virtual void prepareFrame( Node& root ) override;
    
    /** \brief
      * Builds the render queue.
      */
    virtual void preparePass( const Matrix4f& viewTransform ) override;
    
    virtual void renderPass( RenderTask& ) override;

protected:

    virtual void render( const Renderable& ) = 0;

}; // GeometryStage


template< typename RenderableCompare >
GeometryStage< RenderableCompare >::GeometryStage( int geometryType )
    : rq( geometryType )
    , initialized( false )
    , root( nullptr )
    , passesRendered( 0 )
{
}


template< typename RenderableCompare >
void GeometryStage< RenderableCompare >::prepareFrame( Node& root )
{
    RenderStage::prepareFrame( root );
    this->root = &root;
    this->passesRendered = 0;
}


template< typename RenderableCompare >
void GeometryStage< RenderableCompare >::preparePass( const Matrix4f& viewTransform )
{
    if( ++passesRendered == 1 || !isViewTransformFixed() )
    {
        rq.build( *root, viewTransform );
    }
    else
    {
        rq.rewind();
    }
}


template< typename RenderableCompare >
void GeometryStage< RenderableCompare >::renderPass( RenderTask& rt )
{
    while( !rq.isEmpty() )
    {
        const Renderable& renderable = rq.poll();
        render( renderable );
    }
}


template< typename RenderableCompare >
void GeometryStage< RenderableCompare >::reshape( unsigned int width, unsigned int height )
{
    initialized = true;
}


template< typename RenderableCompare >
bool GeometryStage< RenderableCompare >::isInitialized() const
{
    return initialized;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // GEOMETRYSTAGE_H_6014714286
