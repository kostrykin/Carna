/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#ifndef OBJECT3DSELECTIVEFILTER_H_6014714286
#define OBJECT3DSELECTIVEFILTER_H_6014714286

/** \file   Object3DSelectiveFilter.h
  * \brief  Defines \ref Carna::base::model::Object3DSelectiveFilter.
  */

#include <Carna/base/model/Object3DFilter.h>
#include <set>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// Object3DSelectiveFilter
// ----------------------------------------------------------------------------------

/** \brief  \ref Object3DFilter implementation which
  *         accepts or rejects \ref Object3D "objects" based on a list.
  *
  * \see    \ref Object3DWhitelist, \ref Object3DBlacklist
  * \author Leonid Kostrykin
  * \date   14.12.2011
  */
template< bool accept >
class Object3DSelectiveFilter : public Object3DFilter
{

public:

    /** \brief  Defines a set of 3D objects.
      */
    typedef std::set< const Object3D* > Object3DSet;


    /** \brief  Instantiates from an existing set of 3D objects.
      */
    explicit Object3DSelectiveFilter( const Object3DSet& set )
        : set( set )
    {
    }

    /** \brief  Instantiates from a certain 3D object.
      */
    explicit Object3DSelectiveFilter( const Object3D& obj )
    {
        put( obj );
    }


    virtual bool accepts( const Object3D& obj ) const override
    {
        return ( set.find( &obj ) != set.end() ) == accept;
    }


    /** \brief  Adds supplied 3D object to the set.
      */
    void put( const Object3D& obj )
    {
        set.insert( &obj );
    }

    /** \brief  Removes supplied 3D object from the set.
      */
    void remove( const Object3D& obj )
    {
        set.erase( &obj );
    }


private:

    Object3DSet set;

}; // Object3DSelectiveFilter


/** \brief  Defines a white-list of Object3D instances.
  */
typedef Object3DSelectiveFilter< true  > Object3DWhitelist;


/** \brief  Defines a black-list of Object3D instances.
  */
typedef Object3DSelectiveFilter< false > Object3DBlacklist;



}  // namesapce Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // OBJECT3DSELECTIVEFILTER_H_6014714286
