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

#ifndef NODE_H_6014714286
#define NODE_H_6014714286

#include <Carna/base/view/Spatial.h>
#include <set>

/** \file   Node.h
  * \brief  Defines \ref Carna::base::view::Node.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Node
// ----------------------------------------------------------------------------------

class CARNA_LIB Node : public Spatial
{

    std::set< Spatial* > children;

public:

    virtual ~Node();

    void attachChild( Spatial* );
    
    Spatial* detachChild( Spatial& );
    
    void deleteAllChildren();
    
    void visitChildren( const MutableVisitor& );
    
    void visitChildren( const ImmutableVisitor& ) const;
    
    virtual void updateWorldTransform() override;

}; // Node



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // NODE_H_6014714286
