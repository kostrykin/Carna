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

#include <Carna/helpers/FrameRendererHelper.h>
#include <Carna/base/CarnaException.h>
#include <Carna/presets/OpaqueRenderingStage.h>
#include <Carna/presets/TransparentRenderingStage.h>
#include <Carna/presets/CuttingPlanesStage.h>
#include <Carna/presets/DRRStage.h>
#include <Carna/presets/OccludedRenderingStage.h>
#include <Carna/presets/MeshColorCodingStage.h>
#include <Carna/presets/ParallaxStage.h>

namespace Carna
{

namespace helpers
{



// ----------------------------------------------------------------------------------
// is
// ----------------------------------------------------------------------------------

template< typename RenderStage >
static bool is( const base::RenderStage* rs )
{
    return dynamic_cast< const RenderStage* >( rs ) != nullptr;
}



// ----------------------------------------------------------------------------------
// CARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT
// ----------------------------------------------------------------------------------

#define CARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( RenderStage ) \
    if( is< RenderStage >( x ) ) \
    { \
        return PREFER_X; \
    } \
    if( is< RenderStage >( y ) ) \
    { \
        return PREFER_Y; \
    }



// ----------------------------------------------------------------------------------
// DefaultRenderStageOrder
// ----------------------------------------------------------------------------------

bool DefaultRenderStageOrder::operator()( const base::RenderStage* x, const base::RenderStage* y ) const
{
    const static bool PREFER_X = true;
    const static bool PREFER_Y = false;

    CARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( presets::MeshColorCodingStage );
    CARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( presets::ParallaxStage );
    CARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( presets::CuttingPlanesStage );
    CARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( presets::OccludedRenderingStage );
    CARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( presets::OpaqueRenderingStage );
    CARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( presets::TransparentRenderingStage );
    CARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( presets::RayMarchingStage );

    CARNA_FAIL( "Unknown render stage." );
}



}  // namespace Carna :: helpers

}  // namespace Carna
