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

#include <Carna/base/controller/MovementIntent.h>

namespace Carna
{

namespace base
{

namespace controller
{



// ----------------------------------------------------------------------------------
// MovementIntent
// ----------------------------------------------------------------------------------

MovementIntent::MovementIntent( const QPoint& screenspaceDestination )
    : screenspaceDestination( screenspaceDestination )
{
}



}  // namespace Carna :: base :: controller

}  // namespace Carna :: base

}  // namespace Carna
