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

#ifndef OBJECT3DPAINTINGROUTINE_H_6014714286
#define OBJECT3DPAINTINGROUTINE_H_6014714286

/** \file   Object3DPaintingRoutine.h
  * \brief  Defines \ref Carna::base::view::Object3DPaintingRoutine.
  */

#include <Carna/Carna.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Object3DPaintingRoutine
// ----------------------------------------------------------------------------------

/** \brief  Routine for painting an \ref model::Object3D "Object3D" instance.
  *
  * The concept of painting 3D objects is documented \ref Object3D_Rendering "here".
  *
  * \see
  * You might be looking for the
  * \ref DefaultObject3DPaintingRoutine "default implementation of this interface".
  *
  * \author Leonid Kostrykin
  * \date   26.9.11 - 6.3.12
  */
class CARNA_EXPORT Object3DPaintingRoutine
{

public:

    /** \brief  Does nothing.
      */
    virtual ~Object3DPaintingRoutine()
    {
    }

    /** \brief  Paints the given object.
      */
    virtual void operator()( const Renderer&, const model::Object3D& ) = 0;

}; // Object3DPaintingRoutine



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // OBJECT3DPAINTINGROUTINE_H_6014714286
