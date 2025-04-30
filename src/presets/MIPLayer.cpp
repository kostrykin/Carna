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
// MIPLayer :: Details
// ----------------------------------------------------------------------------------

struct MIPLayer::Details
{
    Details( const base::BlendFunction& function );

    const base::BlendFunction* function;
};


MIPLayer::Details::Details( const base::BlendFunction& function )
    : function( &LAYER_FUNCTION_REPLACE )
{
}



// ----------------------------------------------------------------------------------
// MIPLayer
// ----------------------------------------------------------------------------------

const base::BlendFunction MIPLayer::LAYER_FUNCTION_REPLACE( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
const base::BlendFunction MIPLayer::LAYER_FUNCTION_ADD    ( GL_SRC_ALPHA, GL_ONE );


MIPLayer::MIPLayer( const base::BlendFunction& function, unsigned int colorMapResolution )
    : pimpl( new Details( function ) )
    , colorMap( colorMapResolution )
{
}


MIPLayer::~MIPLayer()
{
}


void MIPLayer::setFunction( const base::BlendFunction& function )
{
    pimpl->function = &function;
}


const base::BlendFunction& MIPLayer::function() const
{
    return *pimpl->function;
}



}  // namespace Carna :: presets

}  // namespace Carna
