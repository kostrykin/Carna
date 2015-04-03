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
#include <Carna/base/NodeListener.h>
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
    
    /* We can iterate over 'listeners' directly, because we pass ourselves an an
     * immutable reference to the listener, s.t. it cannot alter that list anyway.
     */
    for( auto itr = listeners.begin(); itr != listeners.end(); ++itr )
    {
        NodeListener& listener = **itr;
        listener.onNodeDelete( *this );
    }
}


void Node::notifyTreeChanges()
{
    /* Create copy of 'listeners' s.t. each listener is free to remove itself from
     * the list.
     */
    const std::set< NodeListener* > listeners( this->listeners.begin(), this->listeners.end() );
    for( auto itr = listeners.begin(); itr != listeners.end(); ++itr )
    {
        NodeListener& listener = **itr;
        listener.onTreeChange( *this );
    }
    
    /* Notify the parent that is has changed too.
     */
    if( hasParent() )
    {
        parent().notifyTreeChanges();
    }
}


void Node::addNodeListener( NodeListener& listener )
{
    listeners.insert( &listener );
}


void Node::removeNodeListener( NodeListener& listener )
{
    listeners.erase( &listener );
}


void Node::invalidate()
{
    /* Create copy of 'listeners' s.t. each listener is free to remove itself from
     * the list.
     */
    const std::set< NodeListener* > listeners( this->listeners.begin(), this->listeners.end() );
    for( auto itr = listeners.begin(); itr != listeners.end(); ++itr )
    {
        NodeListener& listener = **itr;
        listener.onTreeInvalidated( *this );
    }
    
    /* Also invalidate all parent subtrees.
     */
    Spatial::invalidate();
}


bool Node::hasChild( const Spatial& child ) const
{
    return children.find( const_cast< Spatial* >( &child ) ) != children.end();
}


void Node::attachChild( Spatial* child )
{
    CARNA_ASSERT( child != nullptr );
    CARNA_ASSERT( child != this );
    if( !child->hasParent() || &child->parent() != this )
    {
        if( child->hasParent() )
        {
            child->detachFromParent();
        }
        children.insert( child );
        child->updateParent( *this );
    }
    notifyTreeChanges();
}


Spatial* Node::detachChild( Spatial& child )
{
    const auto childItr = children.find( &child );
    if( childItr != children.end() )
    {
        CARNA_ASSERT( child.hasParent() && &child.parent() == this );
        children.erase( &child );
        child.detachFromParent();
        notifyTreeChanges();
        return &child;
    }
    else
    {
        return nullptr;
    }
}


void Node::deleteAllChildren()
{
    if( !children.empty() )
    {
        std::for_each( children.begin(), children.end(), std::default_delete< Spatial >() );
        children.clear();
        notifyTreeChanges();
    }
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
