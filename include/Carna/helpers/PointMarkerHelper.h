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

#ifndef POINTMARKERHELPER_H_6014714286
#define POINTMARKERHELPER_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>

/** \file   PointMarkerHelper.h
  * \brief  Defines \ref Carna::helpers::PointMarkerHelper.
  */

namespace Carna
{

namespace helpers
{



// ----------------------------------------------------------------------------------
// PointMarkerHelper
// ----------------------------------------------------------------------------------

/** \brief
  * Simplifies the creation and maintenance of point markers.
  *
  * \todo
  * Finish this.
  *
  * \author Leonid Kostrykin
  * \date   31.3.15
  */
class CARNA_LIB PointMarkerHelper
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    const static unsigned int DEFAULT_POINT_SIZE = 6;

    explicit PointMarkerHelper
        ( unsigned int geometryType
        , unsigned int pointSize = DEFAULT_POINT_SIZE );

    explicit PointMarkerHelper
        ( unsigned int geometryType
        , unsigned int meshRole
        , unsigned int materialRole
        , unsigned int pointSize = DEFAULT_POINT_SIZE );

    ~PointMarkerHelper();

    const unsigned int geometryType;
    const unsigned int meshRole;
    const unsigned int materialRole;
    const unsigned int pointSize;
    
    void releaseGeometryFeatures();

    base::Geometry* createPointMarker() const;

    base::Geometry* createPointMarker( const base::Color& color ) const;

}; // PointMarkerHelper



}  // namespace Carna :: helpers

}  // namespace Carna



#endif // POINTMARKERHELPER_H_6014714286
