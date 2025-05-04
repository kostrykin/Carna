/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#ifndef CAMERASHOWCASECONTROL_H_6014714286
#define CAMERASHOWCASECONTROL_H_6014714286

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/CameraControl.h>
#include <memory>

/** \file   CameraShowcaseControl.h
  * \brief  Defines \ref Carna::presets::CameraShowcaseControl.
  */

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// CameraShowcaseControl
// ----------------------------------------------------------------------------------

/** \brief
  * Implements \ref base::CameraControl, rotates the camera around its parent node.
  * The camera movement is bounded to the rail from its parent to its current
  * location.
  *
  * \todo Implement `CameraShowcaseControl::%moveLaterally`.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA CameraShowcaseControl : public base::CameraControl
{

    NON_COPYABLE
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Holds default minimum allowed distance of the camera to its origin.
      */
    const static float DEFAULT_MIN_DISTANCE;
    
    /** \brief
      * Holds default maximum allowed distance of the camera to its origin.
      */
    const static float DEFAULT_MAX_DISTANCE;

    /** \brief
      * Instantiates.
      */
    CameraShowcaseControl();
    
    /** \brief
      * Deletes.
      */
    virtual ~CameraShowcaseControl();
    
    /** \brief
      * Disallows the camera to move closer than \a minDistance to its origin. If the
      * camera currently is closer than \a minDistance, it will be moved s.t. it will
      * be \a minDistance units away from its origin.
      *
      * \pre `minDistance >= 0`
      */
    void setMinDistance( float minDistance );
    
    /** \brief
      * Disallows the camera to move further away than \a maxDistance from its
      * origin.  If the camera currently is further away than \a maxDistance, it will
      * be moved s.t. it will be \a maxDistance units away from its origin.
      *
      * \pre `minDistance >= 0`
      */
    void setMaxDistance( float maxDistance );
    
    /** \brief
      * Tells minimum allowed distance of camera to its origin.
      */
    float minDistance() const;
    
    /** \brief
      * Tells maximum allowed distance of camera to its origin.
      */
    float maxDistance() const;

    /** \brief
      * Sets the node that is to be controlled by this instance.
      */
    virtual void setCamera( base::Spatial& ) override;
    
    /** \brief
      * Rotates the camera around the \f$\left(0, 1, 0\right)^\mathrm T\f$ vector of
      * its origin by \a radians.
      */
    virtual void rotateHorizontally( float radians ) override;
    
    /** \brief
      * Rotates the camera around its local \f$\left(1, 0, 0\right)^\mathrm T\f$
      * vector, shifted to its origin, by \a radians.
      */
    virtual void rotateVertically( float radians ) override;
    
    /** \brief
      * Moves the camera along its local \f$\left(0, 0, 1\right)^\mathrm T\f$ vector
      * by \a units. The location is bounded by the current values for
      * \ref minDistance and \ref maxDistance.
      */
    virtual void moveAxially( float units ) override;
    
    virtual void moveLaterally( float unitsX, float unitsY ) override;

}; // CameraShowcaseControl



}  // namespace Carna :: presets

}  // namespace Carna



#endif // CAMERASHOWCASECONTROL_H_6014714286
