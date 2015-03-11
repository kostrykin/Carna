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

#ifndef SPATIAL_H_6014714286
#define SPATIAL_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/math.h>
#include <Carna/base/noncopyable.h>
#include <functional>

/** \file   Spatial.h
  * \brief  Defines \ref Carna::base::Spatial.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Spatial
// ----------------------------------------------------------------------------------

/** \brief
  * Represents a spatial scene element.
  * It's location is determined relatively to another spatial, called \em %parent.
  *
  * \section SceneGraph Scene Graph
  *
  *     The parent-child relationship induces a tree structure, that is commonly
  *     referred to as \em scene \em graph. Such a scene graph represents a scene.
  *     This has two implications: First, that each scene contains exactly one node
  *     that has no parent, namely the tree's root. Second, that it is sufficient to
  *     specify only the root node in order to reach each spatial element of the
  *     scene.
  *
  *     The specific type of a spatial decides upon whether the spatial is an inner
  *     node or a leaf of the scene graph: If it is \em allowed to have children, the
  *     spatial will be realized by an instance of the \ref Node class, even if it
  *     has no children in a particular situation. In contrast, visible scene
  *     elements, i.e. such that can be rendered, must always be leafs. They will be
  *     realized by instances of the \ref Geometry class usually.
  *
  *     It should be clear from the above why the root of a scene graph always is a
  *     \ref Node instance.
  *
  * \author Leonid Kostrykin
  * \date   21.2.15 - 6.3.15
  */
class CARNA_LIB Spatial
{

    NON_COPYABLE

    Node* myParent;
    math::Matrix4f myWorldTransform;
    const void* myUserData;
    bool movable;

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    /** \brief
      * Instantiates.
      */
    Spatial();
    
    /** \brief
      * Does nothing.
      */
    virtual ~Spatial();
    
    /** \brief
      * Declares an entity that visits mutable \ref Spatial instances.
      */
    typedef std::function< void( Spatial& ) > MutableVisitor;
    
    /** \brief
      * Declares an entity that visits mutable \ref Spatial instances.
      */
    typedef std::function< void( const Spatial& ) > ImmutableVisitor;
    
    /** \brief
      * Tells whether this spatial has a parent node.
      */
    bool hasParent() const;

    /** \brief
      * Detaches this spatial from it's parent node.
      * The caller takes possession of this spatial.
      *
      * \pre
      * <code>hasParent() == true</code>
      *
      * \returns
      * Possessing pointer to this spatial if it has successfully been detached from
      * the parent or \c nullptr if it has no parent.
      */
    Spatial* detachFromParent();

    /** \brief
      * Fixes tree consistency by updating parent of this spatial.
      *
      * \note
      * This method is for internal usage only.
      */
    void updateParent( Node& parent );
    
    /** \brief
      * References the parent node.
      *
      * \pre
      * <code>hasParent() == true</code>
      */
    Node& parent();
    
    /** \brief
      * References the parent node.
      *
      * \pre
      * <code>hasParent() == true</code>
      */
    const Node& parent() const;

    /** \brief
      * References the root node this spatial belongs to.
      */
    Node& findRoot();
    
    /** \brief
      * References the root node this spatial belongs to.
      */
    const Node& findRoot() const;
    
    /** \brief
      * Defines the location, rotation and scale of this spatial in relation to it's
      * parent. If this spatial has no parent, the value has no meaning.
      *
      * The default value is \ref math::identity4f.
      */
    math::Matrix4f localTransform;
    
    /** \brief
      * Computes the transformation to world space for this spatial.
      *
      * The default implementation concatenates the parent's world transformation
      * with the \ref localTransform "local transformation" of this spatial.
      */
    virtual void updateWorldTransform();
    
    /** \brief
      * Tells the transformation to world space for this spatial that was last
      * computed.
      */
    const math::Matrix4f& worldTransform() const;

    /** \brief
      * Links an arbitrary object with this \c %Spatial instance.
      *
      * \post
      * <code>hasUserData() == true</code>
      */
    template< typename UserDataType >
    void setUserData( const UserDataType& userData );
    
    /** \brief
      * Removes any object that has been linked with this \c %Spatial instance
      * through \ref setUserData previously.
      *
      * \post
      * <code>hasUserData() == false</code>
      *
      * Does nothing if \ref hasUserData is \c false.
      */
    void removeUserData();

    /** \brief
      * Tells whether an object has been linked with this \c %Spatial instance
      * through \ref setUserData previously.
      */
    bool hasUserData() const;

    /** \brief
      * Retrieves the object previously \ref setUserData "linked" with this
      * \c %Spatial instance.
      *
      * \pre
      * <code>hasUserData()</code>
      */
    template< typename UserDataType >
    const UserDataType& userData() const;

    /** \brief
      * Sets whether this spatial may be displaced w.r.t. it's parent through user
      * interaction. Usually this will be \c false when this spatial represents a
      * \em component of it's parent, like the shaft of an arrow.
      *
      * Each spatial is movable by default.
      */
    void setMovable( bool movable );
    
    /** \brief
      * Tells whether this spatial may be displaced w.r.t. it's parent through user
      * interaction. Usually this will be \c false when this spatial represents a
      * \em component of it's parent, like the shaft of an arrow.
      *
      * Each spatial is movable by default.
      */
    bool isMovable() const;

}; // Spatial


template< typename UserDataType >
void Spatial::setUserData( const UserDataType& userData )
{
    myUserData = &userData;
    CARNA_ASSERT( hasUserData() );
}


template< typename UserDataType >
const UserDataType& Spatial::userData() const
{
    CARNA_ASSERT( hasUserData() );
    return *static_cast< const UserDataType* >( myUserData );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // SPATIAL_H_6014714286
