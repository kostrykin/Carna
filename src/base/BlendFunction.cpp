/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/base/BlendFunction.h>

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



}  // namespace Carna :: base

}  // namespace Carna
