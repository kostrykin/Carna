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

#ifndef OBJECT3DFILTER_H_6014714286
#define OBJECT3DFILTER_H_6014714286

/** \file   Object3DFilter.h
  * \brief  Defines \ref Carna::base::model::Object3DFilter.
  */

#include <Carna/Carna.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// Object3DFilter
// ----------------------------------------------------------------------------------

/** \brief  Accepts or rejects certain Object3D instances.
  *
  * \see    \ref Object3DWhitelist, \ref Object3DBlacklist
  * \author Leonid Kostrykin
  * \date   14.12.2011
  */
class CARNA_EXPORT Object3DFilter
{

public:

    /** \brief  Tests some 3D object.
      */
    virtual bool accepts( const Object3D& ) const = 0;

    /** \brief	Does nothing.
      */
    virtual ~Object3DFilter()
    {
    }

}; // Object3DFilter



}  // namesapce Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // OBJECT3DFILTER_H_6014714286
