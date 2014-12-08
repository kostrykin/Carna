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

#ifndef OBJECT3DUNIFORMFILTER_H_6014714286
#define OBJECT3DUNIFORMFILTER_H_6014714286

/** \file   Object3DUniformFilter.h
  * \brief  Defines \ref Carna::base::model::Object3DUniformFilter.
  */

#include "Object3DFilter.h"

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// Object3DUniformFilter
// ----------------------------------------------------------------------------------

/** \brief  \ref Object3DFilter implementation that either accepts or rejects all
  *         \ref Object3D instances.
  *
  * Before \ref v_2_4 this class was named \c Object3DNullFilter.
  *
  * \author Leonid Kostrykin
  * \date   14.12.2011
  */
class CARNA_EXPORT Object3DUniformFilter : public Object3DFilter
{

public:

    /** \brief  Instantiates.
      */
    Object3DUniformFilter( bool accept )
        : acceptsAll( accept )
    {
    }

    virtual bool accepts( const Object3D& ) const override
    {
        return acceptsAll;
    }


    /** \brief  Holds whether all 3D objects are accepted.
      *
      * If \c false, all 3D objects are rejected.
      */
    const bool acceptsAll;

}; // Object3DUniformFilter



}  // namesapce Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // OBJECT3DUNIFORMFILTER_H_6014714286
