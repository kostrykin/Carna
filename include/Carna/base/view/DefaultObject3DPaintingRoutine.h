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

#ifndef DEFAULTOBJECT3DPAINTINGROUTINE_H_6014714286
#define DEFAULTOBJECT3DPAINTINGROUTINE_H_6014714286

/** \file   DefaultObject3DPaintingRoutine.h
  * \brief  Defines \ref Carna::base::view::DefaultObject3DPaintingRoutine.
  */

#include <Carna/base/view/Object3DPaintingRoutine.h>
#include <Carna/base/noncopyable.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// DefaultObject3DPaintingRoutine
// ----------------------------------------------------------------------------------

/** \brief  Simple Object3D painting routing, based on \ref model::Object3D::paint invocation.
  *
  * \author Leonid Kostrykin
  * \date   26.9.11 - 21.3.13
  */
class CARNA_EXPORT DefaultObject3DPaintingRoutine : public Object3DPaintingRoutine
{

    NON_COPYABLE

public:

    /** \brief  Paints the given object by invoking \ref model::Object3D::paint.
      */
    virtual void operator()( const Renderer&, const model::Object3D& ) override;

}; // DefaultObject3DPaintingRoutine



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // DEFAULTOBJECT3DPAINTINGROUTINE_H_6014714286
