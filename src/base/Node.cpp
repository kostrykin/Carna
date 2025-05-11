/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include <LibCarna/base/Node.hpp>
#include <LibCarna/base/NodeListener.hpp>
#include <algorithm>
#include <set>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Node :: Details
// ----------------------------------------------------------------------------------

struct Node::Details
{
    Details( Node& self );
    Node& self;

    std::set< Spatial* > children;
    std::set< NodeListener* > listeners;
    
    bool isDying;
    bool isTreeChangeNotified;
    bool isDeletingChildren;
    void notifyTreeChanges( bool inThisSubtree );
};


Node::Details::Details( Node& self )
    : self( self )
    , isDying( false )
    , isTreeChangeNotified( false )
    , isDeletingChildren( false )
{
}


void Node::Details::notifyTreeChanges( bool inThisSubtree )
{
    /* Do not propagate notifications as long as this node is deleting children
     * itself. It will fire a notifications when it is done anyway. Also do not
     * propagate notifications if this node is being deleted.
     */
    if( isDeletingChildren || isDying )
    {
        return;
    }

    /* Notify each node only once.
     */
    if( !isTreeChangeNotified )
    {
        isTreeChangeNotified = true;
        
        /* Create copy of 'listeners' s.t. each listener is free to remove itself from
         * the list.
         */
        const std::set< NodeListener* > listeners( this->listeners.begin(), this->listeners.end() );
        for( auto itr = listeners.begin(); itr != listeners.end(); ++itr )
        {
            NodeListener& listener = **itr;
            listener.onTreeChange( self, inThisSubtree );
        }
        
        /* Notify the parent that is has changed too.
         */
        if( self.hasParent() )
        {
            self.parent().pimpl->notifyTreeChanges( true );
        }
        
        /* Notify the children that the scene's tree structure has changed. Create copy
         * of 'children' s.t. each listener is free to remove its child from the list.
         */
        const std::set< Spatial* > children( this->children.begin(), this->children.end() );
        for( auto itr = children.begin(); itr != children.end(); ++itr )
        {
            Node* const child = dynamic_cast< Node* >( *itr );
            if( child != nullptr )
            {
                /* Denote that this change affects a different subtree.
                 */
                child->pimpl->notifyTreeChanges( false );
            }
        }
        
        isTreeChangeNotified = false;
    }
}



// ----------------------------------------------------------------------------------
// Node
// ----------------------------------------------------------------------------------

Node::Node( const std::string& tag )
    : Spatial( tag )
#pragma warning( push )
#pragma warning( disable:4355 )
    /* It is okay to use 'this' in class initialization list, as long as it is not
     * used to access any members that may not have been initialized yet.
     */
    , pimpl( new Details( *this ) )
#pragma warning( pop )
{
}


Node::~Node()
{
    pimpl->isDying = true;

    /* We can iterate over 'listeners' directly, because we pass ourselves an an
     * immutable reference to the listener, s.t. it cannot alter that list anyway.
     */
    for( auto itr = pimpl->listeners.begin(); itr != pimpl->listeners.end(); ++itr )
    {
        NodeListener& listener = **itr;
        listener.onNodeDelete( *this );
    }
    
    /* We delete the children after notifying the listeners s.t. each listener has a
     * chance to 'save' particular children if it desires to.
     */
    deleteAllChildren();
}


void Node::addNodeListener( NodeListener& listener )
{
    pimpl->listeners.insert( &listener );
}


void Node::removeNodeListener( NodeListener& listener )
{
    pimpl->listeners.erase( &listener );
}


void Node::invalidate()
{
    /* Create copy of 'listeners' s.t. each listener is free to remove itself from
     * the list.
     */
    const std::set< NodeListener* > listeners( pimpl->listeners.begin(), pimpl->listeners.end() );
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
    return pimpl->children.find( const_cast< Spatial* >( &child ) ) != pimpl->children.end();
}


void Node::attachChild( Spatial* child )
{
    LIBCARNA_ASSERT( child != nullptr );
    LIBCARNA_ASSERT( child != this );
    if( !child->hasParent() || &child->parent() != this )
    {
        if( child->hasParent() )
        {
            child->detachFromParent();
        }
        pimpl->children.insert( child );
        child->updateParent( *this );
    }
    pimpl->notifyTreeChanges( true );
}


Spatial* Node::detachChild( Spatial& child )
{
    const auto childItr = pimpl->children.find( &child );
    if( childItr != pimpl->children.end() )
    {
        LIBCARNA_ASSERT( child.hasParent() && &child.parent() == this );
        pimpl->children.erase( &child );
        child.detachFromParent();
        pimpl->notifyTreeChanges( true );
        return &child;
    }
    else
    {
        return nullptr;
    }
}


void Node::deleteAllChildren()
{
    if( !pimpl->children.empty() )
    {
        pimpl->isDeletingChildren = true;
        std::for_each( pimpl->children.begin(), pimpl->children.end(), std::default_delete< Spatial >() );
        pimpl->children.clear();
        pimpl->isDeletingChildren = false;
        pimpl->notifyTreeChanges( true );
    }
}


void Node::visitChildren( bool recursively, const MutableVisitor& visit )
{
    for( auto itr = pimpl->children.begin(); itr != pimpl->children.end(); ++itr )
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
    for( auto itr = pimpl->children.begin(); itr != pimpl->children.end(); ++itr )
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


std::size_t Node::children() const
{
    return pimpl->children.size();
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



}  // namespace LibCarna :: base

}  // namespace LibCarna
