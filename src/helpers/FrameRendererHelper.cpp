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

#include <LibCarna/helpers/FrameRendererHelper.h>
#include <LibCarna/base/CarnaException.h>
#include <LibCarna/presets/OpaqueRenderingStage.h>
#include <LibCarna/presets/TransparentRenderingStage.h>
#include <LibCarna/presets/CuttingPlanesStage.h>
#include <LibCarna/presets/DRRStage.h>
#include <LibCarna/presets/DVRStage.h>
#include <LibCarna/presets/OccludedRenderingStage.h>
#include <LibCarna/presets/MeshColorCodingStage.h>
#include <LibCarna/presets/ParallaxStage.h>

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
// LIBCARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT
// ----------------------------------------------------------------------------------

#define LIBCARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( RenderStage ) \
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

    LIBCARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( presets::MeshColorCodingStage );
    LIBCARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( presets::ParallaxStage );
    LIBCARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( presets::CuttingPlanesStage );
    LIBCARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( presets::OccludedRenderingStage );
    LIBCARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( presets::OpaqueRenderingStage );
    LIBCARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( presets::TransparentRenderingStage );
    LIBCARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( presets::VolumeRenderingStage );

    LIBCARNA_FAIL( "Unknown render stage." );
}



}  // namespace Carna :: helpers

}  // namespace Carna
