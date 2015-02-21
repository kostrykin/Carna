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

#include <Carna/base/view/Spatial.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Spatial
// ----------------------------------------------------------------------------------

Spatial::Spatial()
	: myParent( nullptr )
{
}


Spatial::~Spatial()
{
}


bool Spatial::hasParent() const
{
	return myParent != nullptr;
}


Spatial* Spatial::detachFromParent()
{
	CARNA_ASSERT( hasParent() );
	return myParent->detachChild( *this );
}

	
Node& Spatial::parent()
{
	CARNA_ASSERT( hasParent() );
	return *myParent;
}


const Node& Spatial::parent() const
{
	CARNA_ASSERT( hasParent() );
	return *myParent;
}


virtual void Spatial::updateWorldTransform()
{
	if( hasParent() )
	{
		myWorldTransform = myParent->worldTransform() * localTransform;
	}
	else
	{
		myWorldTransform = localTransform;
	}
}


const Matrix4f& Spatial::worldTransform() const
{
	return myWorldTransform;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
