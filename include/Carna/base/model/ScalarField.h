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

#ifndef SCALARFIELD_H_6014714286
#define SCALARFIELD_H_6014714286

/** \file   ScalarField.h
  *
  * \brief  Defines \ref Carna::base::model::ScalarField.
  *
  * \author Leonid Kostrykin
  * \date   21.2.13
  */

#include <Carna/base/Vector3.h>
#include <Carna/Carna.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// ScalarField
// ----------------------------------------------------------------------------------

/** \brief  Abstract definition of an \f$\mathbb Z_0^3 \to \mathrm T \subset \mathbb R\f$ scalar field.
  *
  * \author Leonid Kostrykin
  * \date   21.2.13
  */
template< typename ValueType >
class ScalarField
{

public:

    /** \brief  Holds the codomain type of the scalar field.
      *
      * \since  \ref v_2_2_2
      */
    typedef ValueType ValueType;

    /** \brief  Does nothing.
      */
    virtual ~ScalarField()
    {
    }

    
    /** \brief  Returns value of specified voxel.
      */
    virtual ValueType operator()
        ( unsigned int x
        , unsigned int y
        , unsigned int z ) const = 0;

    /** \brief  Returns value of specified voxel.
      */
    virtual ValueType operator()( const Vector3ui& at ) const;

}; // ScalarField


template< typename ValueType >
ValueType ScalarField< ValueType >::operator()( const Vector3ui& at ) const
{
    return ( *this )( at.x, at.y, at.z );
}



}  // namesapce Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // SCALARFIELD_H_6014714286
