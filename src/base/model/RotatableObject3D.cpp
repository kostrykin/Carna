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

#include <Carna/base/model/RotatableObject3D.h>
#include <Carna/base/model/RotatableObject3DDetails.h>
#include <Carna/base/model/Object3DEvent.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// RotatableObject3D :: Details
// ----------------------------------------------------------------------------------

RotatableObject3D::Details::Details()
{
}



// ----------------------------------------------------------------------------------
// RotatableObject3D
// ----------------------------------------------------------------------------------

RotatableObject3D::RotatableObject3D( Scene& model, const std::string& name )
    : Object3D( model, name )
    , pimpl( new Details() )
{
}


RotatableObject3D::RotatableObject3D( Scene& model, const Position& position, const std::string& name )
    : Object3D( model, position, name )
    , pimpl( new Details() )
{
}


RotatableObject3D::~RotatableObject3D()
{
}


const Transformation& RotatableObject3D::rotation() const
{
    return pimpl->rotation;
}


void RotatableObject3D::setRotation( const Transformation& newRotation )
{
    pimpl->rotation = newRotation;

    invalidateObjects3D( Object3DEvent( Object3DEvent::rotation ) );

    emit rotated();
}



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna
