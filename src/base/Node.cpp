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

#include <Carna/base/Node.h>
#include <algorithm>

namespace Carna
{

namespace base
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


void Node::visitChildren( bool recursively, const MutableVisitor& visit )
{
    for( auto itr = children.begin(); itr != children.end(); ++itr )
    {
        Spatial* const spatial = *itr;
        visit( *spatial );
        if( recursively && dynamic_cast< Node* >( spatial ) != nullptr )
        {
            Node* const node = static_cast< Node* >( spatial );
            node->visitChildren( true, visit );
        }
    }
}


void Node::visitChildren( bool recursively, const ImmutableVisitor& visit ) const
{
    for( auto itr = children.begin(); itr != children.end(); ++itr )
    {
        const Spatial* const spatial = *itr;
        visit( *spatial );
        if( recursively && dynamic_cast< const Node* >( spatial ) != nullptr )
        {
            const Node* const node = static_cast< const Node* >( spatial );
            node->visitChildren( true, visit );
        }
    }
}


void Node::updateWorldTransform()
{
    Spatial::updateWorldTransform();
    visitChildren( false, []( Spatial& child )
        {
            child.updateWorldTransform();
        }
    );
}



}  // namespace Carna :: base

}  // namespace Carna
