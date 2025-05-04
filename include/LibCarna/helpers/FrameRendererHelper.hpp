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

#ifndef FRAMERENDERERHELPER_H_6014714286
#define FRAMERENDERERHELPER_H_6014714286

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/Log.hpp>
#include <LibCarna/base/text.hpp>
#include <LibCarna/base/RenderStageSequence.hpp>
#include <LibCarna/base/RenderStage.hpp>
#include <LibCarna/base/GLContext.hpp>
#include <LibCarna/base/MeshRenderingStage.hpp>
#include <LibCarna/presets/MeshColorCodingStage.hpp>
#include <LibCarna/presets/OccludedRenderingStage.hpp>
#include <set>

/** \file   FrameRendererHelper.h
  * \brief  Defines \ref LibCarna::helpers::FrameRendererHelper.
  */

namespace LibCarna
{

namespace helpers
{



// ----------------------------------------------------------------------------------
// DefaultRenderStageOrder
// ----------------------------------------------------------------------------------

/** \brief
  * Defines default \ref QuickStart_FrameRenderer "rendering stages order" used by
  * \ref FrameRendererHelper.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA DefaultRenderStageOrder
{

public:

    /** \brief
      * Compares \a x to \a y.
      */
    bool operator()( const base::RenderStage* x, const base::RenderStage* y ) const;

}; // DefaultRenderStageOrder



// ----------------------------------------------------------------------------------
// FrameRendererHelper
// ----------------------------------------------------------------------------------

/** \brief
  * Adds \ref base::RenderStage "rendering stages" to a \ref base::FrameRenderer
  * object according to \a RenderStageOrder.
  *
  * \todo
  * Finish this.
  *
  * \author Leonid Kostrykin
  */
template< typename RenderStageOrder >
class FrameRendererHelper
{

    NON_COPYABLE

    std::set< base::RenderStage*, RenderStageOrder > stages;

public:

    /** \brief
      * Instantiates helper for \a renderer.
      */
    explicit FrameRendererHelper( base::RenderStageSequence& renderer );

    /** \brief
      * Deletes.
      */
    ~FrameRendererHelper();
    
    /** \brief
      * Takes possession of \a rs and adds it to the \ref renderer when \ref commit
      * is invoked.
      */
    FrameRendererHelper< RenderStageOrder >& operator<<( base::RenderStage* rs );

    /** \brief
      * References the configured %renderer.
      */
    base::RenderStageSequence& renderer;

    /** \brief
      * Resets changes that have been made since the last invocation of \ref commit.
      */
    void reset();

    /** \brief
      * Commits recorded changes to the associated \ref renderer. Removes all
      * previosly committed stages if \a clear is `true`.
      */
    void commit( bool clear = true );

}; // FrameRendererHelper


template< typename RenderStageOrder >
FrameRendererHelper< RenderStageOrder >::FrameRendererHelper( base::RenderStageSequence& renderer )
    : renderer( renderer )
{
}


template< typename RenderStageOrder >
FrameRendererHelper< RenderStageOrder >::~FrameRendererHelper()
{
    reset();
}


template< typename RenderStageOrder >
void FrameRendererHelper< RenderStageOrder >::reset()
{
    std::for_each( stages.begin(), stages.end(), std::default_delete< base::RenderStage >() );
    stages.clear();
}


template< typename RenderStageOrder >
void FrameRendererHelper< RenderStageOrder >::commit( bool clear )
{
    if( clear )
    {
        renderer.clearStages();
    }
    presets::MeshColorCodingStage* mccs = nullptr;
    presets::OccludedRenderingStage* occluded = nullptr;
    unsigned int registeredMeshColorCodingStages = 0;
    unsigned int enabledOccludedStages = 0;
    for( auto stageItr = stages.begin(); stageItr != stages.end(); ++stageItr )
    {
        base::RenderStage* const rs = *stageItr;
        renderer.appendStage( rs );

        if( mccs == nullptr )
        {
            mccs = dynamic_cast< presets::MeshColorCodingStage* >( rs );
        }
        if( occluded == nullptr )
        {
            occluded = dynamic_cast< presets::OccludedRenderingStage* >( rs );
        }

        base::MeshRenderingMixin* const meshRenderer = dynamic_cast< base::MeshRenderingMixin* >( rs );
        if( meshRenderer != nullptr && mccs != nullptr )
        {
            mccs->putGeometryType( meshRenderer->geometryType, meshRenderer->ROLE_DEFAULT_MESH );
            mccs->enableMaterials( meshRenderer->geometryType, meshRenderer->ROLE_DEFAULT_MATERIAL );
            ++registeredMeshColorCodingStages;
        }
        if( meshRenderer != nullptr && occluded != nullptr )
        {
            occluded->enableStage( *rs );
            ++enabledOccludedStages;
        }
    }
    base::Log::instance().record( base::Log::debug
        , "FrameRendererHelper registered "
        + base::text::lexical_cast< std::string >( registeredMeshColorCodingStages )
        + " stage(s) for color coding." );
    base::Log::instance().record( base::Log::debug
        , "FrameRendererHelper enabled "
        + base::text::lexical_cast< std::string >( enabledOccludedStages )
        + " stage(s) for occluded rendering." );
    stages.clear();
}


template< typename RenderStageOrder >
LibCarna::helpers::FrameRendererHelper< RenderStageOrder >& FrameRendererHelper< RenderStageOrder >::operator<<( LibCarna::base::RenderStage* rs )
{
    stages.insert( rs );
    return *this;
}



}  // namespace LibCarna :: helpers

}  // namespace LibCarna



#endif // FRAMERENDERERHELPER_H_6014714286
