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

class CARNA_LIB Spatial
{

    NON_COPYABLE

    Node* myParent;
    math::Matrix4f myWorldTransform;
    const void* myUserData;
    bool movable;

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    Spatial();

    virtual ~Spatial();
    
    typedef std::function< void( Spatial& ) > MutableVisitor;
    
    typedef std::function< void( const Spatial& ) > ImmutableVisitor;
    
    bool hasParent() const;

    /** \brief
      * Detaches this spatial from it's parent node.
      * The caller takes possession of this spatial.
      *
      * \returns
      * Possessing pointer to this spatial if it has successfully been detached from
      * the parent or \c nullptr if it has no parent.
      */
    Spatial* detachFromParent();

    /** \brief
      * Fixes tree consistency by updating parent of this spatial.
      *
      * This method is only required for internal usage.
      */
    void updateParent( Node& parent );
    
    Node& parent();
    
    const Node& parent() const;
    
    math::Matrix4f localTransform;
    
    /** \brief
      * Computes the transformation to world space for this spatial.
      *
      * The default implementation concatenates the parent's world transformation with
      * the \ref localTransform "local transformation" of this spatial.
      */
    virtual void updateWorldTransform();
    
    /** \brief
      * Tells the transformation to world space for this spatial that was last computed.
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
      * Removes any object that has been linked with this \c %Spatial instance through \ref setUserData previously.
      *
      * \post
      * <code>hasUserData() == false</code>
      *
      * Does nothing if \ref hasUserData is \c false.
      */
    void removeUserData();

    /** \brief
      * Tells whether an object has been linked with this \c %Spatial instance through \ref setUserData previously.
      */
    bool hasUserData() const;

    /** \brief
      * Retrieves the object previously \ref setUserData "linked" with this \c %Spatial instance.
      *
      * \pre
      * <code>hasUserData()</code>
      */
    template< typename UserDataType >
    const UserDataType& userData() const;

    void setMovable( bool movable );
    
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
