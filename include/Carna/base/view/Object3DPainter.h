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

#ifndef OBJECT3DPAINTER_H_6014714286
#define OBJECT3DPAINTER_H_6014714286

/** \file   Object3DPainter.h
  * \brief  Defines \ref Carna::base::view::Object3DPainter.
  */

#include <Carna/Carna.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Object3DPainter
// ----------------------------------------------------------------------------------

/** \brief  Provides interface for drawing Object3D instances.
  *
  * \author Leonid Kostrykin
  * \date   14.12.11 - 6.3.13
  */
class CARNA_EXPORT Object3DPainter
{

public:

    /** \brief  Does nothing.
      */
    virtual ~Object3DPainter()
    {
    }


    /** \brief  Paints 3D objects using the given painting routine.
      */
    virtual void paint( const Renderer&, Object3DPaintingRoutine& ) const = 0;

}; // Object3DPainter



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // OBJECT3DPAINTER_H_6014714286
