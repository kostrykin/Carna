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
#include <Carna/base/Matrix4f.h>
#include <Carna/base/noncopyable.h>
#include <functional>

/** \file   Spatial.h
  * \brief  Defines \ref Carna::base::view::Spatial.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Spatial
// ----------------------------------------------------------------------------------

class CARNA_LIB Spatial
{

    NON_COPYABLE

    Node* myParent;
    Matrix4f myWorldTransform;

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
    
    Matrix4f localTransform;
    
    virtual void updateWorldTransform();
    
    const Matrix4f& worldTransform() const;

}; // Spatial



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // SPATIAL_H_6014714286
