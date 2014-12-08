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

#ifndef SLICEPLANE_H_6014714286
#define SLICEPLANE_H_6014714286

/** \file   SlicePlane.h
  * \brief  Defines \ref Carna::SlicePlanes::SlicePlane.
  */

#include <Carna/Carna.h>
#include <Carna/base/Vector3.h>
#include <Carna/base/Transformation.h>
#include <Carna/base/noncopyable.h>
#include <memory>

class QImage;
class QWidget;

namespace Carna
{

namespace SlicePlanes
{



// ----------------------------------------------------------------------------------
// SlicePlanes :: SlicePlane
// ----------------------------------------------------------------------------------

/** \brief  Definition and GPU-enhanced computation of planar volume intersections.
  *
  * \see
  * There is \ref SlicePlaneRenderer "an adapter of this functionality" to the \ref base::view::Renderer interface.
  *
  * The internal state is set by the \ref compute method.
  * If computation is successful, which means that \ref isValid returns \c true, than the results can be queried by:
  *
  * - \ref SlicePlane::fetchResult "Downloading image from video to system memory?"
  * - \ref SlicePlane::getResultTextureID "Continuing to use the image in video memory?"
  * - \ref SlicePlane::getTangentToVolume "Processing the orientation of the image?"
  *
  * \attention
  * When specifying the intersectional plane, you must use \ref VolumeSpace "volume space coordinates".
  *
  * \note
  * The rotation of the image within the plane's tangent space is a degree of freedom
  * and chosen by the underlying implementation: the chosen rotation 
  * ref Carna::SlicePlane::getTangentToVolume "can be detected easily" and processed
  * in order to provide some fixed orientation.
  *
  * \image html SlicePlane01.png "intersection examples with two different planes"
  *
  * \author Leonid Kostrykin
  * \date   26.5.11 - 7.3.13
  */
class CARNA_EXPORT SlicePlane
{

    NON_COPYABLE

public:

    /** \brief  Instantiates using shared graphical resources.
      */
    SlicePlane( base::view::SceneProvider&, base::VisualizationEnvironment& );

    /** \brief  Instantiates using exclusive graphical resources.
      *
      * \deprecated Use \ref Carna::base::OffscreenDisplay "OffscreenDisplay" and \ref Carna::SlicePlanes::SlicePlaneVisualization instead.
      */
    explicit SlicePlane( const base::model::Scene& );

    /** \brief  Releases resources.
      */
    virtual ~SlicePlane();


    /** \brief
      * Computes intersection for the given parameters.
      *
      * Returns \c true if computation was successful or \c false if something went
      * wrong. The validity state, which can be queried by \ref isValid, is updated to the returned value.
      *
      * \pre
      * <code>Carna::base::Math::isEqual( normal.norm(), 0. ) == false</code>
      *
      * Most common reason for failure is the inability to compute at least three
      * different intersection points of the given plane and the volume's bounding box.
      * Usually this happens when the plane does not intersect the 3D volume but is
      * tangent to some corner of the volume, like the plane
      * \f$ \{ \vec x : \langle \vec x, \frac{1}{\sqrt 3}\big( 1, 1, 1 \big) \rangle = 0 \} \f$
      * does, for example.
      *
      * \param normal
      * Plane normal vector in \ref VolumeSpace "volume space coordinates".
      *
      * \param distance
      * Plane distance to origin in \ref VolumeSpace "volume space coordinates".
      *
      * \param proportions
      * Vector that describes the volume proportions. The longest side of the volume
      * bounding box must be denoted with \c 1, the other two sides proportional to
      * the longest side.
      *
      * \param windowingLevel
      * The HU value \f$w_0\f$ that is to be mapped to RGB color \f$(127,127,127)\f$.
      *
      * \param windowingWidth
      * The \f$\delta w\f$ where
      * -# all values \f$\leq w_0 - \delta w\f$ are mapped to black and all values \f$> \max\{ -1024, w_0 - \delta w \}\f$ not to black,
      * -# all values \f$\geq w_0 + \delta w\f$ are mapped to white and all values \f$< \min\{ w_0 + \delta w, 3071 \}\f$ not to white.
      *
      * \param border
      * The width of the line that is to be drawn along the intersection of the slice
      * plane with the volume's bounding box. No line is drawn if \a border is \c 0.
      */
    bool compute
        ( const base::Vector& normal
        , const double& distance
        , const base::Vector& proportions
        , int windowingLevel
        , unsigned int windowingWidth
        , float border );


    /** \brief  Tells the transformation from tangent space to
      *         \ref CoordinateSystems "volume space".
      */
    const base::Transformation& getTangentToVolume() const;


    /** \brief  Tells the ID of the GL texture object of the computed plane.
      */
    unsigned int getResultTextureID() const;


    /** \brief  Fetches the result from video to system memory.
      */
    void fetchResult( QImage& ) const;


    /** \brief  Tells whether this object represents a valid slice plane, which is
      *         determined by the success or the failure of a prior \ref compute call.
      */
    bool isValid() const;


private:

    class ExclusiveModeDetails;
    class SharedModeDetails;

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // SlicePlanes :: SlicePlane



}  // namespace Carna :: SlicePlanes

}  // namespace Carna

#endif // SLICEPLANE_H_6014714286
