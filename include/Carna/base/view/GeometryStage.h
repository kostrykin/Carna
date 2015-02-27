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
#include <Carna/base/view/GeometryAggregate.h>
#include <Carna/base/math.h>
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
class GeometryStage : public RenderStage
{

    Node* root;
    std::size_t passesRendered;
    std::set< GeometryAggregate* > acquiredVideoResources;
    GLContext* myContext;

protected:

    RenderQueue< RenderableCompare > rq;

public:

    GeometryStage( int geometryType );

    /** \brief
      * Releases acquired video resources.
      */
    virtual ~GeometryStage();
    
    virtual void reshape( const FrameRenderer& fr, const Viewport& vp ) override;
    
    virtual bool isInitialized() const override;

    virtual void prepareFrame( Node& root ) override;
    
    virtual void renderPass( const math::Matrix4f& viewTransform, RenderTask& rt, const Viewport& vp ) override;

protected:

    void activateGLContext() const;

    virtual void render( GLContext& glc, const Renderable& renderable ) = 0;

}; // GeometryStage


template< typename RenderableCompare >
GeometryStage< RenderableCompare >::GeometryStage( int geometryType )
    : root( nullptr )
    , passesRendered( 0 )
    , myContext( nullptr )
    , rq( geometryType )
{
}


template< typename RenderableCompare >
GeometryStage< RenderableCompare >::~GeometryStage()
{
    activateGLContext();
    std::for_each( acquiredVideoResources.begin(), acquiredVideoResources.end(), [&]( GeometryAggregate* ga )
        {
            ga->releaseVideoResources();
        }
    );
}


template< typename RenderableCompare >
void GeometryStage< RenderableCompare >::activateGLContext() const
{
    if( myContext != nullptr )
    {
        myContext->makeActive();
    }
}


template< typename RenderableCompare >
void GeometryStage< RenderableCompare >::prepareFrame( Node& root )
{
    RenderStage::prepareFrame( root );
    this->root = &root;
    this->passesRendered = 0;
}


template< typename RenderableCompare >
void GeometryStage< RenderableCompare >::renderPass( const math::Matrix4f& viewTransform, RenderTask& rt, const Viewport& vp )
{
    CARNA_ASSERT( myContext != nullptr );

    const bool isFirstPass = passesRendered == 0;
    if( ++passesRendered == 1 || !isViewTransformFixed() )
    {
        rq.build( *root, viewTransform );
    }
    else
    {
        rq.rewind();
    }

    std::set< GeometryAggregate* > usedVideoResources;
    while( !rq.isEmpty() )
    {
        const Renderable& renderable = rq.poll();
        if( isFirstPass )
        {
            renderable.geometry().visitAggregates( [&]( GeometryAggregate& ga, unsigned int role )
                {
                    /* Denote that the geometry definition was used.
                     */
                    usedVideoResources.insert( &ga );

                    /* Check whether video resources need to be acquired.
                     */
                    if( acquiredVideoResources.find( &ga ) == acquiredVideoResources.end() )
                    {
                        ga.acquireVideoResources();
                        acquiredVideoResources.insert( &ga );
                    }
                }
            );
        }
        render( *myContext, renderable );
    }
    if( isFirstPass )
    {
        // release unused video resources
        for( auto itr = acquiredVideoResources.begin(); itr != acquiredVideoResources.end(); )
        {
            if( usedVideoResources.find( *itr ) == usedVideoResources.end() )
            {
                acquiredVideoResources.erase( itr++ );
            }
            else
            {
                ++itr;
            }
        }
    }
}


template< typename RenderableCompare >
void GeometryStage< RenderableCompare >::reshape( const FrameRenderer& fr, const Viewport& vp )
{
    this->myContext = &fr.glContext();
}


template< typename RenderableCompare >
bool GeometryStage< RenderableCompare >::isInitialized() const
{
    return this->myContext != nullptr;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // GEOMETRYSTAGE_H_6014714286
