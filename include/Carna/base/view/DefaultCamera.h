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

#ifndef DEFAULTCAMERA_H_6014714286
#define DEFAULTCAMERA_H_6014714286

/** \file   DefaultCamera.h
  * \brief  Defines \ref Carna::base::view::DefaultCamera.
  */

#include <Carna/base/view/Camera.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// DefaultCamera
// ----------------------------------------------------------------------------------

/** \brief  \ref Camera implementation that can be controlled programmatically or
  *         obtain it's view data from user interaction via
  *         \ref controller::DefaultCameraController.
  *
  * Also see the documentation on the produced \ref fetchCameraMatrix "camera matrices"
  * and \ref fetchProjectionMatrix "projection matrices".
  *
  * \author Leonid Kostrykin
  * \date   8.6.11 - 25.3.13
  */
class CARNA_EXPORT DefaultCamera : public Camera
{

public:

    /** \brief  Instantiates.
      *
      * \param model
      * References the data model this camera is associated with.
      *
      * \param perspectiveProjection
      * Sets whether a perspective or an orthogonal projection (default) shall be used.
      */
    explicit DefaultCamera
        ( const model::Scene& model
        , bool perspectiveProjection = false );

    /** \brief  Releases acquired resources.
      */
    virtual ~DefaultCamera();

    // ------------------------------------------------------------------------------
    // Camera Interface
    // ------------------------------------------------------------------------------
    
    /** \brief  Obtains the \em camera \em matrix.
      *
      * \see
      * Refer to \ref CoordinateSystems for more information on vertex transformation.
      *
      * The produced matrix spans the \em camera \em space coordinate system,
      * so that the \em scene \em space is mapped -
      * only using proportional scaling -
      * and the whole volume data becomes subset of
      *
      * \f[ \big\{ \vec p = ( x, y, z )^\mathrm T : x, y, z \in [0, 1] \big\} \f]
      *
      * Hence, the length of one unit in camera space is chosen equal to the length of the volume data's longest edge.
      * The scene is translated two units into the deep.
      *
      * The produced transformations are illustrated below.
      *
      * \image html DefaultCamera1.png
      */
    virtual void fetchCameraMatrix( Renderer&, Transformation& ) override;
    
    /** \brief  Obtains the \em projection \em matrix.
      *
      * Refer to \ref fetchCameraMatrix documentation for details.
      */
    virtual void fetchProjectionMatrix( Renderer&, Transformation& ) override;


    /** \brief  Does nothing.
      */
    virtual void openShutter( Renderer& renderer ) override
    {
    }

    /** \brief  Does nothing.
      */
    virtual void closeShutter( Renderer& renderer ) override
    {
    }

    // ------------------------------------------------------------------------------
    // Orthogonal / Perspective
    // ------------------------------------------------------------------------------

    /** \brief  Returns whether a perspective or an orthogonal projection is being used.
      */
    bool isPerspectiveProjection() const;

    /** \brief  Sets whether a perspective or an orthogonal projection shall be used.
      */
    void setPerspectiveProjection( bool enabled = true );

    // ------------------------------------------------------------------------------
    // Zoom Management
    // ------------------------------------------------------------------------------

    /** \brief  Tells the current zoom factor.
      */
    float zoom() const;

    /** \brief  Adjusts the zoom.
      *
      * The zoom center must be given in pixels relatively to the render targets center.
      * It is ignored for negative \a zoomDelta.
      */
    void zoom( float zoomDelta, float zoomCenterX, float zoomCenterY );

    // ------------------------------------------------------------------------------
    // Shift & Rotation
    // ------------------------------------------------------------------------------

    /** \brief
      * Shifts the camera within it's x-y-plane by \a pixelsOffsetX and \a pixelsOffsetY pixels respectively.
      */
    void shift( float pixelsOffsetX, float pixelsOffsetY );

    /** \brief
      * Applies the given transformation to the camera's rotation around the scene's center.
      */
    void rotate( const Transformation& );

    /** \brief
      * Overrides the camera's rotation around the scene's center by the given transformation.
      */
    void setRotation( const Transformation& );


private:

    struct Details;

    std::unique_ptr< Details > pimpl;

}; // DefaultCamera



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // DEFAULTCAMERA_H_6014714286
