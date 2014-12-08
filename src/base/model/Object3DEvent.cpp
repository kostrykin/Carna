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

#include <Carna/base/model/Object3DEvent.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// Object3DEvent
// ----------------------------------------------------------------------------------

const base::Flags< Object3DEvent::Feature > Object3DEvent::any =
        base::Flags< Object3DEvent::Feature >( Object3DEvent::existence )
                                          .or( Object3DEvent::name )
                                          .or( Object3DEvent::position )
                                          .or( Object3DEvent::rotation )
                                          .or( Object3DEvent::shape );



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna
