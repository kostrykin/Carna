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

#include <LibCarna/base/BlendFunction.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BlendFunction
// ----------------------------------------------------------------------------------

BlendFunction::BlendFunction( int sourceFactor, int destinationFactor )
    : sourceFactor( sourceFactor )
    , destinationFactor( destinationFactor )
{
}


bool BlendFunction::operator==( const BlendFunction& other ) const
{
    return sourceFactor == other.sourceFactor && destinationFactor == other.destinationFactor;
}



}  // namespace Carna :: base

}  // namespace Carna
