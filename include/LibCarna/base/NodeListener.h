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

#ifndef NODELISTENER_H_6014714286
#define NODELISTENER_H_6014714286

#include <LibCarna/LibCarna.h>

/** \file   NodeListener.h
  * \brief  Defines \ref Carna::base::NodeListener.
  */

namespace Carna
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
  * \date   3.4.15
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



}  // namespace Carna :: base

}  // namespace Carna

#endif // NODELISTENER_H_6014714286
