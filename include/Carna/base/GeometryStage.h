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

#include <Carna/base/RenderStage.h>
#include <Carna/base/RenderQueue.h>
#include <Carna/base/GeometryFeature.h>
#include <Carna/base/math.h>
#include <memory>
#include <map>

/** \file   GeometryStage.h
  * \brief  Defines \ref Carna::base::GeometryStage.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// GeometryStage
// ----------------------------------------------------------------------------------

/** \brief
  * Partially implements a \ref RenderStage "rendering stage" that uses at least one
  * \ref RenderQueue "render queue" for rendering geometry from the scene graph.
  *
  * \param RenderableCompare
  * Defines partial order of \ref Renderable "renderables" that is applied to the
  * \ref rq "default rendering queue" of this stage. Set this to \c void if no
  * particular order is required.
  *
  * \see
  * Refer to the documentation of the \ref RenderingProcess "rendering process" for
  * further notes on how rendering stages operate.
  *
  * \author Leonid Kostrykin
  * \date   21.2.15 - 6.3.15
  */
template< typename RenderableCompare >
class GeometryStage : public RenderStage
{

    typedef GeometryFeature::VideoResourceAcquisition VideoResource;

    Node* root;
    std::size_t passesRendered;
    std::map< GeometryFeature*, VideoResource* > acquiredFeatures;
    GLContext* myContext;

protected:

    RenderQueue< RenderableCompare > rq;

public:

    GeometryStage
        ( unsigned int geometryType
        , unsigned int geometryTypeMask = RenderQueue< RenderableCompare >::EXACT_MATCH_GEOMETRY_TYPE_MASK );

    /** \brief
      * Releases acquired video resources.
      */
    virtual ~GeometryStage();
    
    virtual void reshape( const FrameRenderer& fr, const Viewport& vp ) override;
    
    virtual bool isInitialized() const override;

    virtual void prepareFrame( Node& root ) override;
    
    virtual void renderPass( const math::Matrix4f& viewTransform, RenderTask& rt, const Viewport& vp ) override;

    std::size_t renderedPassesCount() const;
    
    template< typename GeometryFeatureType >
    typename GeometryFeatureType::VideoResourceAcquisition& videoResource( GeometryFeatureType& ) const;
    
    template< typename GeometryFeatureType >
    const typename GeometryFeatureType::VideoResourceAcquisition& videoResource( const GeometryFeatureType& ) const;

protected:

    void activateGLContext() const;

    virtual void updateRenderQueues( Node& root, const math::Matrix4f& viewTransform, bool viewTransformTriggered );

    virtual void rewindRenderQueues();

    virtual void render( GLContext& glc, const Renderable& renderable ) = 0;

}; // GeometryStage


template< typename RenderableCompare >
GeometryStage< RenderableCompare >::GeometryStage( unsigned int geometryType, unsigned int geometryTypeMask )
    : root( nullptr )
    , passesRendered( 0 )
    , myContext( nullptr )
    , rq( geometryType, geometryTypeMask )
{
}


template< typename RenderableCompare >
GeometryStage< RenderableCompare >::~GeometryStage()
{
    activateGLContext();
    std::for_each( acquiredFeatures.begin(), acquiredFeatures.end(),
        [&]( const std::pair< GeometryFeature*, VideoResource* >& entry )
        {
            delete entry.second;
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
std::size_t GeometryStage< RenderableCompare >::renderedPassesCount() const
{
    return passesRendered;
}


template< typename RenderableCompare >
void GeometryStage< RenderableCompare >::updateRenderQueues( Node& root, const math::Matrix4f& vt, bool vtTriggered )
{
    rq.build( root, vt );
}


template< typename RenderableCompare >
void GeometryStage< RenderableCompare >::rewindRenderQueues()
{
    rq.rewind();
}


template< typename RenderableCompare >
void GeometryStage< RenderableCompare >::renderPass( const math::Matrix4f& viewTransform, RenderTask& rt, const Viewport& vp )
{
    CARNA_ASSERT( myContext != nullptr );

    const bool isFirstPass = passesRendered == 0;
    if( ++passesRendered == 1 || !isViewTransformFixed() )
    {
        const bool viewTransformTriggered = passesRendered != 1;
        updateRenderQueues( *root, viewTransform, viewTransformTriggered );
    }
    else
    {
        rewindRenderQueues();
    }

    std::set< GeometryFeature* > usedFeatures;
    while( !rq.isEmpty() )
    {
        const Renderable& renderable = rq.poll();
        if( isFirstPass )
        {
            renderable.geometry().visitFeatures( [&]( GeometryFeature& gf, unsigned int role )
                {
                    /* Denote that the geometry definition was used.
                     */
                    usedFeatures.insert( &gf );

                    /* Check whether video resources need to be acquired.
                     */
                    if( acquiredFeatures.find( &gf ) == acquiredFeatures.end() )
                    {
                        VideoResource* const vr = gf.acquireVideoResource( *myContext );
                        acquiredFeatures[ &gf ] = vr;
                    }
                }
            );
        }
        render( *myContext, renderable );
    }
    if( isFirstPass )
    {
        // release unused video resources
        for( auto itr = acquiredFeatures.begin(); itr != acquiredFeatures.end(); )
        {
            if( usedFeatures.find( itr->first ) == usedFeatures.end() )
            {
                acquiredFeatures.erase( itr++ );
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


template< typename RenderableCompare >
template< typename GeometryFeatureType >
typename GeometryFeatureType::VideoResourceAcquisition& GeometryStage< RenderableCompare >
    ::videoResource( GeometryFeatureType& gf ) const
{
    const auto itr = acquiredFeatures.find( &gf );
    CARNA_ASSERT( itr != acquiredFeatures.end() );
    return static_cast< typename GeometryFeatureType::VideoResourceAcquisition& >( *itr->second );
}


template< typename RenderableCompare >
template< typename GeometryFeatureType >
const typename GeometryFeatureType::VideoResourceAcquisition& GeometryStage< RenderableCompare >
    ::videoResource( const GeometryFeatureType& gf ) const
{
    return videoResource( const_cast< GeometryFeatureType& >( gf ) );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // GEOMETRYSTAGE_H_6014714286
