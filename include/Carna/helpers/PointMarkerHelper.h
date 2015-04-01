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
  * This provides two methods for the creation of point markers. A point marker is
  * considered a 2D shape of fixed pixel size that is yet positioned in 3D space.
  * Each instance of this class creates point markers of a particular pixel size.
  *
  * The first method takes no arguments and creates point markers from a predefined
  * color palette. This method is a little faster in theory because it recycles
  * materials once they have been instantiated for a particular color. The second
  * method takes the color as an argument and repeats the instantiation of a material
  * every time it is used.
  *
  * The following example creates a sequence of marker points in rotating colors:
  *
  * \snippet ModuleTests/PointMarkerHelperTest.cpp multiple
  *
  * \image html PointMarkerHelperTest/multiple.png "exemplary rendering from code above"
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

    /** \brief
      * Holds the default point marker pixel size.
      */
    const static unsigned int DEFAULT_POINT_SIZE = 6;

    /** \brief
      * Instantiates.
      */
    explicit PointMarkerHelper
        ( unsigned int geometryType
        , unsigned int pointSize = DEFAULT_POINT_SIZE );

    /** \overload
      */
    explicit PointMarkerHelper
        ( unsigned int geometryType
        , unsigned int meshRole
        , unsigned int materialRole
        , unsigned int pointSize = DEFAULT_POINT_SIZE );

    /** \brief
      * \ref releaseGeometryFeatures "Releases all previously acquired materials."
      */
    ~PointMarkerHelper();

    const unsigned int geometryType; ///< Holds the markers' \ref GeometryTypes "geometry type".
    const unsigned int meshRole;     ///< Holds the \ref GeometryTypes "role" used to attach the mesh to the markers.
    const unsigned int materialRole; ///< Holds the \ref GeometryTypes "role" used to attach the material to the markers.
    const unsigned int pointSize;    ///< Holds the pixel size of the markers.
    
    /** \brief
      * Releases all previously acquired materials. Invoke this method when it is sure
      * that no further markers will be created.
      *
      * Invoking this method and than instantiating markers will cause redundant
      * \ref base::Material instantiations.
      */
    void releaseGeometryFeatures();

    /** \brief
      * Creates new point marker colored differently than the last point marker
      * created by any `%PointMarkerHelper` instance.
      */
    base::Geometry* createPointMarker() const;

    /** \brief
      * Creates new point marker with \a color.
      */
    base::Geometry* createPointMarker( const base::Color& color ) const;

    /** \brief
      * Resets the color that the next invocation to the argument-less version of
      * \ref createPointMarker uses.
      */
    static void resetDefaultColor();

}; // PointMarkerHelper



}  // namespace Carna :: helpers

}  // namespace Carna



#endif // POINTMARKERHELPER_H_6014714286
