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

#ifndef PROJECTIONCONTROL_H_6014714286
#define PROJECTIONCONTROL_H_6014714286

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/math.hpp>
#include <memory>

/** \file   ProjectionControl.h
  * \brief  Defines \ref LibCarna::base::ProjectionControl.
  */

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ProjectionControl
// ----------------------------------------------------------------------------------

/** \brief
  * Controls projection matrices.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA ProjectionControl
{

    NON_COPYABLE
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Holds default near clipping plane distance that should be used by
      * \ref updateProjection.
      */
    const static float DEFAULT_MINIMUM_VISIBLE_DISTANCE;

    /** \brief
      * Holds default far clipping plane distance that should be used by
      * \ref updateProjection.
      */
    const static float DEFAULT_MAXIMUM_VISIBLE_DISTANCE;

    /** \brief
      * Instantiates.
      */
    ProjectionControl();
    
    /** \brief
      * Deletes.
      */
    virtual ~ProjectionControl();
    
    /** \brief
      * Sets the width of the viewport \ref updateProjection should create projection
      * matrices for.
      *
      * \pre `width > 0`
      */
    void setViewportWidth( unsigned int width );
    
    /** \brief
      * Sets the height of the viewport \ref updateProjection should create
      * projection matrices for.
      *
      * \pre `height > 0`
      */
    void setViewportHeight( unsigned int height );
    
    /** \brief
      * Tells the width of the viewport \ref updateProjection should create
      * projection matrices for.
      */
    unsigned int viewportWidth() const;
    
    /** \brief
      * Tells the height of the viewport \ref updateProjection should create
      * projection matrices for.
      */
    unsigned int viewportHeight() const;
    
    /** \brief
      * Sets near clipping plane distance that should be used by
      * \ref updateProjection.
      *
      * \pre `minimumVisibleDistance >= 0`
      */
    void setMinimumVisibleDistance( float minimumVisibleDistance );
    
    /** \brief
      * Sets far clipping plane distance that should be used by
      * \ref updateProjection.
      *
      * \pre `maximumVisibleDistance > 0`
      */
    void setMaximumVisibleDistance( float maximumVisibleDistance );
    
    /** \brief
      * Tells near clipping plane distance that is used by \ref updateProjection.
      */
    float minimumVisibleDistance() const;
    
    /** \brief
      * Tells far clipping plane distance that is used by \ref updateProjection.
      */
    float maximumVisibleDistance() const;
    
    /** \brief
      * Creates \a projectionMatrix based on previously set parameters.
      */
    virtual void updateProjection( math::Matrix4f& projectionMatrix ) const = 0;
    
    /** \brief
      * Tells `true` when probably a new \ref updateProjection "projection matrix" is
      * available and `false` when \ref updateProjection is likely to return the same
      * result as the last time.
      */
    bool isUpdateAvailable() const;
    
protected:

    /** \brief
      * Denotes that \ref isUpdateAvailable "an update is available".
      */
    void invalidateProjection();
    
    /** \brief
      * Denotes that \ref isUpdateAvailable "no update is available".
      */
    void setProjectionValidated() const;

}; // ProjectionControl



}  // namespace LibCarna :: base

}  // namespace LibCarna



#endif // PROJECTIONCONTROL_H_6014714286
