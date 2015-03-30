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

#include <Carna/base/glew.h>
#include <Carna/presets/MIPLayer.h>

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// MIP :: MIPLayer
// ----------------------------------------------------------------------------------

const base::BlendFunction MIPLayer::LAYER_FUNCTION_REPLACE( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
const base::BlendFunction MIPLayer::LAYER_FUNCTION_ADD    ( GL_SRC_ALPHA, GL_ONE );


MIPLayer::MIPLayer
    ( const base::math::Span< base::HUV >& huRange
    , const base::math::Vector4f& color
    , const base::BlendFunction& function )

    : huRange( huRange )
    , color( color )
    , myFunction( &function )
{
}


MIPLayer::MIPLayer
    ( base::HUV firstHuv, base::HUV lastHuv
    , const base::math::Vector4f& color
    , const base::BlendFunction& function )

    : huRange( firstHuv, lastHuv )
    , color( color )
    , myFunction( &function )
{
}


void MIPLayer::setFunction( const base::BlendFunction& layerFunction )
{
    myFunction = &layerFunction;
}


const base::BlendFunction& MIPLayer::function() const
{
    return *myFunction;
}



}  // namespace Carna :: presets

}  // namespace Carna
