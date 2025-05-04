/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#ifndef GEOMETRYSTAGE_H_6014714286
#define GEOMETRYSTAGE_H_6014714286

#include <LibCarna/base/FrameRenderer.h>
#include <LibCarna/base/RenderStage.h>
#include <LibCarna/base/RenderQueue.h>
#include <LibCarna/base/GeometryFeature.h>
#include <LibCarna/base/math.h>
#include <memory>
#include <map>

/** \file   GeometryStage.h
  * \brief  Defines \ref LibCarna::base::GeometryStage.
  */

namespace LibCarna
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
  * \ref rq "predefined rendering queue" of this stage. Set this to \c void if no
  * particular order is required.
  *
  * Override \ref updateRenderQueues and \ref rewindRenderQueues if you require
  * further rendering queues.
  *
  * \see
  * Refer to the documentation of the \ref RenderingProcess "rendering process" for
  * further notes on how rendering stages operate.
  *
  * \author Leonid Kostrykin
  */
template< typename RenderableCompare >
class GeometryStage : public RenderStage
{

    typedef GeometryFeature::ManagedInterface VideoResource;

    Node* root;
    std::size_t passesRendered;
    std::map< GeometryFeature*, VideoResource* > acquiredFeatures;

protected:

    /** \brief
      * Holds the predefined rendering queue of this rendering stage.
      */
    RenderQueue< RenderableCompare > rq;

public:

    /** \brief
      * Instantiates s.t. the \ref rq "predefined rendering queue" enqueues such
      * \ref LibCarna::base::Geometry scene graph nodes, whose geometry type
      * *AND*-linked with \a geometryTypeMask equals the \a geometryType specified
      * here.
      */
    GeometryStage
        ( unsigned int geometryType
        , unsigned int geometryTypeMask = RenderQueue< RenderableCompare >::EXACT_MATCH_GEOMETRY_TYPE_MASK );
        
    const unsigned int geometryType;     ///< Renders such geometries whose type *AND*-linked with \ref geometryTypeMask equals this.
    const unsigned int geometryTypeMask; ///< Renders such geometries whose type *AND*-linked with this equals \ref geometryType.

    /** \brief
      * Releases acquired video resources.
      */
    virtual ~GeometryStage();

    /** \copydoc LibCarna::base::RenderStage::prepareFrame
      */
    virtual void prepareFrame( Node& root ) override;

    virtual void renderPass( const math::Matrix4f& viewTransform, RenderTask& rt, const Viewport& vp ) override;

    /** \brief
      * Tells the number of \ref renderPass "passes rendered so far" since the
      * \ref prepareFrame "beginning of the current frame".
      */
    std::size_t renderedPassesCount() const;
    
    /** \brief
      * Interfaces the \a geometryFeature video resources that were acquired by
      * this rendering stage.
      */
    template< typename GeometryFeatureType >
    typename GeometryFeatureType::ManagedInterface& videoResource( GeometryFeatureType& geometryFeature ) const;
    
    /** \overload
      */
    template< typename GeometryFeatureType >
    const typename GeometryFeatureType::ManagedInterface& videoResource( const GeometryFeatureType& geometryFeature ) const;

protected:

    /** \brief
      * Ensures that the \ref LibCarna::base::GLContext "OpenGL context" of the hosting
      * \ref LibCarna::base::FrameRenderer is the current one.
      */
    void activateGLContext() const;
    
    /** \brief
      * Builds the rendering queues of this stage.
      */
    virtual void buildRenderQueues( Node& root, const math::Matrix4f& viewTransform );

    /** \brief
      * Rewinds the rendering queues of this stage.
      */
    virtual void rewindRenderQueues();

    /** \brief
      * Recomputes the \ref ViewSpace "model-view transforms" of the renderables
      * enqueued by this stage.
      */
    virtual void updateRenderQueues( const math::Matrix4f& viewTransform );

    /** \brief
      * Renders the \a renderable.
      */
    virtual void render( const Renderable& renderable ) = 0;

}; // GeometryStage


template< typename RenderableCompare >
GeometryStage< RenderableCompare >::GeometryStage( unsigned int geometryType, unsigned int geometryTypeMask )
    : root( nullptr )
    , passesRendered( 0 )
    , rq( geometryType, geometryTypeMask )
    , geometryType( geometryType )
    , geometryTypeMask( geometryTypeMask )
{
}


template< typename RenderableCompare >
GeometryStage< RenderableCompare >::~GeometryStage()
{
    activateGLContext();
    std::for_each( acquiredFeatures.begin(), acquiredFeatures.end(),
        [&]( const std::pair< GeometryFeature*, VideoResource* >& entry )
        {
            if( entry.second != nullptr )
            {
                delete entry.second;
            }
        }
    );
}


template< typename RenderableCompare >
void GeometryStage< RenderableCompare >::activateGLContext() const
{
    if( isInitialized() )
    {
        renderer().glContext().makeCurrent();
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
void GeometryStage< RenderableCompare >::buildRenderQueues( Node& root, const math::Matrix4f& viewTransform )
{
    rq.build( root, viewTransform );
}


template< typename RenderableCompare >
void GeometryStage< RenderableCompare >::rewindRenderQueues()
{
    rq.rewind();
}


template< typename RenderableCompare >
void GeometryStage< RenderableCompare >::updateRenderQueues( const math::Matrix4f& viewTransform )
{
    rq.updateModelViewTransforms( viewTransform );
}


template< typename RenderableCompare >
void GeometryStage< RenderableCompare >::renderPass( const math::Matrix4f& viewTransform, RenderTask& rt, const Viewport& vp )
{
    const bool isFirstPass = passesRendered == 0;
    
    /* Maintain the render queues.
     */
    if( isFirstPass )
    {
        buildRenderQueues( *root, viewTransform );
    }
    else
    {
        rewindRenderQueues();
        if( isViewTransformFixed() )
        {
            updateRenderQueues( viewTransform );
        }
    }

    std::set< GeometryFeature* > usedFeatures;
    while( !rq.isEmpty() )
    {
        const Renderable& renderable = rq.poll();
        if( isFirstPass )
        {
            renderable.geometry().visitFeatures( [&]( GeometryFeature& gf, unsigned int role )
                {
                    /* Denote that the geometry feature was used.
                     */
                    usedFeatures.insert( &gf );

                    /* Check whether video resources need to be acquired.
                     */
                    if( acquiredFeatures.find( &gf ) == acquiredFeatures.end() )
                    {
                        VideoResource* const vr = gf.acquireVideoResource();
                        acquiredFeatures[ &gf ] = vr;
                    }
                }
            );
        }
        render( renderable );
    }
    if( isFirstPass )
    {
        /* Release unused video resources.
         */
        for( auto itr = acquiredFeatures.begin(); itr != acquiredFeatures.end(); )
        {
            if( usedFeatures.find( itr->first ) == usedFeatures.end() )
            {
                if( itr->second != nullptr )
                {
                    delete itr->second;
                }
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
template< typename GeometryFeatureType >
typename GeometryFeatureType::ManagedInterface& GeometryStage< RenderableCompare >
    ::videoResource( GeometryFeatureType& gf ) const
{
    const auto itr = acquiredFeatures.find( &gf );
    LIBCARNA_ASSERT( itr != acquiredFeatures.end() );
    return static_cast< typename GeometryFeatureType::ManagedInterface& >( *itr->second );
}


template< typename RenderableCompare >
template< typename GeometryFeatureType >
const typename GeometryFeatureType::ManagedInterface& GeometryStage< RenderableCompare >
    ::videoResource( const GeometryFeatureType& gf ) const
{
    return videoResource( const_cast< GeometryFeatureType& >( gf ) );
}



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // GEOMETRYSTAGE_H_6014714286
