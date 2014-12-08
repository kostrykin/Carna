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

#include <Carna/base/view/DefaultObject3DPainter.h>
#include <Carna/base/view/Object3DPaintingRoutine.h>
#include <Carna/base/model/Scene.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// DefaultObject3DPainter
// ----------------------------------------------------------------------------------

void DefaultObject3DPainter::paint( const Renderer& renderer, Object3DPaintingRoutine& paintObject ) const
{
    for( unsigned int i = 0; i < model.objectsCount(); ++i )
    {
        const model::Object3D& object = model.objectByIndex( i );

        if( filter->accepts( object ) )
        {
            paintObject( renderer, object );
        }
    }
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
