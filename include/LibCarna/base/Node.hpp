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

#ifndef NODE_H_6014714286
#define NODE_H_6014714286

#include <LibCarna/base/Spatial.hpp>
#include <memory>

/** \file
  * \brief
  * Defines \ref LibCarna::base::Node.
  */

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Node
// ----------------------------------------------------------------------------------

/** \brief
  * Defines the inner node of a \ref SceneGraph "scene graph". Implements a
  * \ref Spatial "spatial scene element" that is allowed to have children, but cannot
  * be rendered.
  *
  * Each node possesses it's children.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA Node : public Spatial
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Instantiates.
      *
      * \param tag is an arbitrary string that may be used to identify this node.
      */
    explicit Node( const std::string& tag = "" );

    /** \brief
      * Deletes.
      */
    virtual ~Node();
    
    /** \brief
      * Notifies \a listener of changes related to this node in
      * \f$\mathcal O\left(\log n\right)\f$.
      */
    void addNodeListener( NodeListener& listener );
    
    /** \brief
      * Removes \a listener from being notified of changes related to this node in
      * \f$\mathcal O\left(\log n\right)\f$.
      */
    void removeNodeListener( NodeListener& listener );
    
    /** \brief
      * Notifies \ref addNodeListener "all its listeners" that this subtree has
      * changed. This may include changes of the tree structure as well as updated
      * materials and suchlike. Also invalidates all its parent subtrees.
      */
    void invalidate() override;

    /** \brief
      * Attaches \a child to this node in \f$\mathcal O\left(\log n\right)\f$ and
      * takes it's possession.
      *
      * If \a child already has another parent, it is first detached from that one.
      */
    void attachChild( Spatial* child );
    
    /** \brief
      * Detaches \a child from this node in \f$\mathcal O\left(\log n\right)\f$. The
      * caller takes possession of the child.
      *
      * \returns
      * Possessing pointer to the child if it has successfully been detached or \c nullptr
      * if something went wrong, e.g. this node is not the \a child's parent.
      */
    Spatial* detachChild( Spatial& child );

    /** \brief
      * Tells whether \a child is among the children of this node in
      * \f$\mathcal O\left(\log n\right)\f$.
      */
    bool hasChild( const Spatial& child ) const;
    
    /** \brief
      * Deletes all children of this node.
      */
    void deleteAllChildren();
    
    /** \brief
      * Tells number of children.
      */
    std::size_t children() const;
    
    /** \brief
      * Invokes \a visit once on each child of this node \a recursively.
      */
    void visitChildren( bool recursively, const MutableVisitor& visit );
    
    /** \overload
      */
    void visitChildren( bool recursively, const ImmutableVisitor& visit ) const;
    
    /** \brief
      * Computes the transformation to world space for this node just
      * \ref Spatial::updateWorldTransform "like the base class does",
      * than orders it's children to do the same.
      */
    virtual void updateWorldTransform() override;

}; // Node



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // NODE_H_6014714286
