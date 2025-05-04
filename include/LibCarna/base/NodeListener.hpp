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

#ifndef NODELISTENER_H_6014714286
#define NODELISTENER_H_6014714286

#include <LibCarna/LibCarna.hpp>

/** \file   NodeListener.h
  * \brief  Defines \ref LibCarna::base::NodeListener.
  */

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// NodeListener
// ----------------------------------------------------------------------------------

/** \brief
  * Notified by \ref Node objects.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA NodeListener
{

public:

    /** \brief
      * Deletes.
      */
    virtual ~NodeListener();
    
    /** \brief
      * Indicates that \a node is about to be deleted. Dying nodes never notify
      * \ref onTreeChange.
      */
    virtual void onNodeDelete( const Node& node ) = 0;
    
    /** \brief
      * Indicates that the scene's tree structure has changed. The tree structure of
      * \a node only has changed if \a inThisSubtree is `true`.
      */
    virtual void onTreeChange( Node& node, bool inThisSubtree ) = 0;
    
    /** \brief
      * Indicates that \a subtree has been invalidated. This may include changes of
      * the tree structure as well as updated materials and suchlike.
      */
    virtual void onTreeInvalidated( Node& subtree ) = 0;

}; // NodeListener



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // NODELISTENER_H_6014714286
