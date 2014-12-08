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
#include <Carna/base/view/IncrementalColorPaintingRoutine.h>
#include <Carna/base/model/Object3D.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// IncrementalColorPaintingRoutine
// ----------------------------------------------------------------------------------

IncrementalColorPaintingRoutine::IncrementalColorPaintingRoutine()
    : nextID( 1 )
{
}


void IncrementalColorPaintingRoutine::operator()( const Renderer& renderer, const model::Object3D& obj )
{
    const unsigned int id = nextID++;

    idByObject[ &obj ] = id;
    objectById[ id ] = &obj;

    Vector3ui color;
    color.x = (id >>  0 ) & 0x000000FF; //< bits 0-7
    color.y = (id >>  8 ) & 0x000000FF; //< bits 8-15
    color.z = (id >> 16 ) & 0x000000FF; //< bits 16-23

    glPushMatrix();

    obj.paintFalseColor( renderer, color );

    glPopMatrix();
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
