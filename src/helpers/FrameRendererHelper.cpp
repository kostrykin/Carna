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

#include <LibCarna/helpers/FrameRendererHelper.hpp>
#include <LibCarna/base/LibCarnaException.hpp>
#include <LibCarna/presets/OpaqueRenderingStage.hpp>
#include <LibCarna/presets/TransparentRenderingStage.hpp>
#include <LibCarna/presets/CuttingPlanesStage.hpp>
#include <LibCarna/presets/DRRStage.hpp>
#include <LibCarna/presets/DVRStage.hpp>
#include <LibCarna/presets/OccludedRenderingStage.hpp>
#include <LibCarna/presets/MeshColorCodingStage.hpp>
#include <LibCarna/presets/ParallaxStage.hpp>

namespace LibCarna
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
    LIBCARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( presets::DRRStage );
    LIBCARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( presets::DVRStage );
    LIBCARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( presets::MIPStage );
    LIBCARNA_DEFAULT_RENDER_STAGE_ORDER_NEXT( presets::MaskRenderingStage );

    LIBCARNA_FAIL( "Unknown render stage." );
}



}  // namespace LibCarna :: helpers

}  // namespace LibCarna
