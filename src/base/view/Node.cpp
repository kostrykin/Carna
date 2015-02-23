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

#include <Carna/base/view/Node.h>
#include <algorithm>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Node
// ----------------------------------------------------------------------------------

Node::~Node()
{
    deleteAllChildren();
}


bool Node::hasChild( const Spatial& child ) const
{
    return children.find( const_cast< Spatial* >( &child ) ) != children.end();
}


void Node::attachChild( Spatial* child )
{
    CARNA_ASSERT( child != nullptr );
    if( !child->hasParent() || &child->parent() != this )
    {
        if( child->hasParent() )
        {
            child->detachFromParent();
        }
        children.insert( child );
        child->updateParent( *this );
    }
}


Spatial* Node::detachChild( Spatial& child )
{
    const auto childItr = children.find( &child );
    if( childItr != children.end() )
    {
        CARNA_ASSERT( child.hasParent() && &child.parent() == this );
        children.erase( &child );
        child.detachFromParent();
        return &child;
    }
    else
    {
        return nullptr;
    }
}


void Node::deleteAllChildren()
{
    std::for_each( children.begin(), children.end(), std::default_delete< Spatial >() );
    children.clear();
}


void Node::visitChildren( const MutableVisitor& visit )
{
    for( auto itr = children.begin(); itr != children.end(); ++itr )
    {
        visit( **itr );
    }
}


void Node::visitChildren( const ImmutableVisitor& visit ) const
{
    for( auto itr = children.begin(); itr != children.end(); ++itr )
    {
        visit( **itr );
    }
}


void Node::updateWorldTransform()
{
    Spatial::updateWorldTransform();
    visitChildren( []( Spatial& child )
        {
            child.updateWorldTransform();
        }
    );
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
