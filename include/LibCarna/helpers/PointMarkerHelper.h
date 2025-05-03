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

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/noncopyable.h>

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
  * A point marker is considered a 2D shape of fixed pixel size that is yet
  * positioned in 3D space. Point markers are rendered as
  * \ref base::IndexBufferBase::PRIMITIVE_TYPE_POINTS "point-primitives" and using
  * the `pointmarker` material shader. That material shader produces the white edge
  * around the markers.
  *
  * Each instance of this class creates point markers of a preferred pixel size. It
  * provides basically two methods for the creation of point markers. The first type
  * takes no arguments and creates point markers from a predefined color palette and
  * using the preferred pixel size. This method is a little faster in theory because
  * it recycles materials once they have been instantiated for a particular color.
  * The other methods take the color, the pixel size, or both as arguments and repeat
  * the instantiation of a material every time they are used.
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
class LIBCARNA PointMarkerHelper
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
    const unsigned int pointSize;    ///< Holds the preferred pixel size of the markers.
    
    /** \brief
      * Releases all previously acquired materials. Invoke this method when it is sure
      * that no further markers will be created.
      *
      * Invoking this method and than instantiating markers will cause redundant
      * \ref base::Material instantiations.
      */
    void releaseGeometryFeatures();

    /** \brief
      * Creates new point marker of preferred pixel size, colored differently than
      * the last point marker created by any `%PointMarkerHelper` instance.
      *
      * This method cycles through a predefined palette of colors. It recycles the
      * materials instantiates once per color.
      */
    base::Geometry* createPointMarker() const;

    /** \brief
      * Creates new point marker of \a pointSize colored differently than the last
      * point marker created by any `%PointMarkerHelper` instance. This method
      * creates a new material each time it is used.
      */
    base::Geometry* createPointMarker( unsigned int pointSize ) const;

    /** \brief
      * Creates new point marker of preferred pixel size with \a color. This method
      * creates a new material each time it is used.
      */
    base::Geometry* createPointMarker( const base::Color& color ) const;

    /** \brief
      * Creates new point marker of \a pointSize with \a color. This method creates
      * a new material each time it is used.
      */
    base::Geometry* createPointMarker( const base::Color& color, unsigned int pointSize ) const;

    /** \brief
      * Resets the color that the next invocation to the argument-less version of
      * \ref createPointMarker uses.
      */
    static void resetDefaultColor();

}; // PointMarkerHelper



}  // namespace Carna :: helpers

}  // namespace Carna



#endif // POINTMARKERHELPER_H_6014714286
