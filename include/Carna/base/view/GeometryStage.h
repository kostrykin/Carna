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

template< typename GeometryRenderer, typename RenderableCompare >
class CARNA_LIB GeometryStage : public RenderStage
{
    
    RenderQueue< RenderableCompare > rq;
    std::unique_ptr< GeometryRenderer > gr;
    bool initialized = false;

public:

    GeometryStage( GeometryRenderer* gr, int geometryType );
    
    /** \brief
      * Builds the render queue.
      */
    virtual void prepareFrame( const Camera& cam, Node& root ) override;
    
    /** \brief
      * Orders this scene processor to reshape it's buffers according to the specified dimensions.
      */
    virtual void reshape( unsigned int width, unsigned int height ) override;
    
    /** \brief
      * Tells whether this scene processor is ready for rendering.
      *
      * In particular this method returns \c true if this scene processor only requires
      * \ref reshape to be called if the dimensions actually have changed. If this method
      * returns \c false, \ref reshape will only be called when the dimensions change.
      */
    virtual bool isInitialized() const override;
    
    virtual void render( RenderManager& ) const override;

}; // GeometryStage


template< typename RenderableCompare >
GeometryStage::GeometryStage( GeometryRenderer* gr, int geometryType )
    : rq( geometryType )
    , gr( gr )
{
}


template< typename RenderableCompare >
void GeometryStage::prepareFrame( const Camera& cam, Node& root )
{
    rq.build( cam, root );
}


template< typename RenderableCompare >
void GeometryStage::render( RenderTask& rt ) const
{
    rq.rewind();
    while( !rq.isEmpty() )
    {
        const Renderable& renderable = rq.poll();
        gr->render( renderable );
    }
}


template< typename RenderableCompare >
void GeometryStage::reshape( unsigned int width, unsigned int height )
{
    gr->reshape( width, height );
    initialized = true;
}


template< typename RenderableCompare >
bool GeometryStage::isInitialized() const
{
    return initialized;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // GEOMETRYSTAGE_H_6014714286
