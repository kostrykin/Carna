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

#ifndef RELEASABLE_H_6014714286
#define RELEASABLE_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>

/** \file   Renderable.h
  * \brief  Defines \ref Carna::base::Renderable.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Releasable
// ----------------------------------------------------------------------------------

/** \brief
  * Defines callback that is invoked when the last remained
  * \ref GLContext "OpenGL context" of the application is about to shut down,
  * indicating that all still remained \ref GeometryFeature "geometry features" must
  * be \ref GeometryFeature::release "released" now.
  *
  * \date   15.03.2015
  * \author Leonid Kostrykin
  */
class CARNA_LIB Releasable
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    Releasable();
    
    virtual ~Releasable();
    
protected:

    /** \brief
      * Releases any previously acquired \ref GeometryFeature "geometry features".
      * The exact behavior depends on the implementation. This method is invoked when
      * the last remained \ref GLContext "OpenGL context" of the application is about
      * to shut down.
      */
    virtual void release() = 0;

}; // Releasable



}  // namespace Carna :: base

}  // namespace Carna

#endif // RELEASABLE_H_6014714286

