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

template< typename GeometryRenderer, typename GeometryCompare = std::less< const Geometry* > >
class CARNA_LIB GeometryStage : public RenderStage
{
    
    RenderQueue< GeometryCompare > rq;
    std::unique_ptr< GeometryRenderer > gr;
    bool initialized = false;

public:

    GeometryStage( GeometryRenderer* gr, int geometryType );
    
    /** \brief
      * Builds the render queue.
      */
    virtual void prepareFrame( Node& root ) override;
    
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


template< typename GeometryCompare >
GeometryStage::GeometryStage( GeometryRenderer* gr, int geometryType )
    : rq( geometryType )
    , gr( gr )
{
}


template< typename GeometryCompare >
void GeometryStage::prepareFrame( Node& root )
{
    rq.build( root );
}


template< typename GeometryCompare >
void GeometryStage::render( RenderTask& rt ) const
{
    rq.rewind();
    while( !rq.isEmpty() )
    {
        const Geometry& geom = rq.poll();
        const Matrix4f modelViewTransform = rt.worldViewTransform() * geom.worldTransform();
        gr->render( modelViewTransform );
    }
}


template< typename GeometryCompare >
void GeometryStage::reshape( unsigned int width, unsigned int height )
{
    gr->reshape( width, height );
    initialized = true;
}


template< typename GeometryCompare >
bool GeometryStage::isInitialized() const
{
    return initialized;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // GEOMETRYSTAGE_H_6014714286
