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

#include <Carna/base/view/glew.h>
#include <Carna/base/view/DefaultObject3DPaintingRoutine.h>
#include <Carna/base/model/Object3D.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// DefaultObject3DPaintingRoutine
// ----------------------------------------------------------------------------------

void DefaultObject3DPaintingRoutine::operator()( const Renderer& renderer, const model::Object3D& object )
{
    glPushMatrix();

    object.paint( renderer );

    glPopMatrix();
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
