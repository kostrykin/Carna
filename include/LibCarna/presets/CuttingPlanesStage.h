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

#ifndef CUTTINGPLANESSTAGE_H_6014714286
#define CUTTINGPLANESSTAGE_H_6014714286

#include <LibCarna/base/GeometryStage.h>
#include <LibCarna/base/Renderable.h>
#include <LibCarna/LibCarna.h>

/** \file   CuttingPlanesStage.h
  * \brief  Defines \ref Carna::presets::CuttingPlanesStage.
  */

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// CuttingPlanesStage
// ----------------------------------------------------------------------------------

/** \brief
  * Renders cutting planes of volume geometries in the scene.
  *
  * The `%CuttingPlanesStage` constructor takes two geometry type parameters:
  *
  * \snippet ModuleTests/CuttingPlanesStageTest.cpp cutting_planes_instantiation
  *
  * The concept of geometry types is explained \ref GeometryTypes "here".
  *
  * The following example code attaches three orthogonal planes to a node named
  * `pivot`:
  *
  * \snippet ModuleTests/CuttingPlanesStageTest.cpp cutting_planes_setup
  *
  * This produces the rendering below.
  *
  * \image html CuttingPlanesStageTest/threePlanes.png "rendering result of example code above"
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA CuttingPlanesStage : public base::GeometryStage< void >
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

    struct VideoResources;
    std::unique_ptr< VideoResources > vr;

public:

    const static float DEFAULT_WINDOWING_WIDTH; ///< Holds the default value for \ref setWindowingWidth.
    const static float DEFAULT_WINDOWING_LEVEL; ///< Holds the default value for \ref setWindowingLevel.
    
    /** \brief
      * Holds the \ref GeometryFeatures "role" that intensity volume data is expected
      * to take when attached to \ref base::Geometry nodes.
      */
    const static unsigned int ROLE_INTENSITIES;

    /** \brief
      * Instantiates.
      */
    CuttingPlanesStage( unsigned int volumeGeometryType, unsigned int planeGeometryType );

    /** \brief
      * Deletes.
      */
    virtual ~CuttingPlanesStage();

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;

    /** \brief
      * Sets windowing level to \a windowingLevel.
      */
    void setWindowingLevel( float windowingLevel );
    
    /** \brief
      * Sets windowing level to \a windowingWidth.
      */
    void setWindowingWidth( float windowingWidth );
    
    /** \overload
      */
    void setWindowingWidth( unsigned int windowingWidth );

    /** \brief
      * Sets whether brightness shall be inversely proportional to the intensity.
      */
    void setRenderingInverse( bool inverse );

    /** \brief
      * Tells the windowing level intensity.
      */
    float windowingLevel() const;
    
    /** \brief
      * Tells the windowing width.
      */
    float windowingWidth() const;

    /** \brief
      * Tells the lower bound of the intensity window.
      */
    float minimumIntensity() const;
    
    /** \brief
      * Tells the upper bound of the intensity window.
      */
    float maximumIntensity() const;
    
    /** \brief
      * Tells whether brightness is inversely proportional to the intensity.
      */
    bool isRenderingInverse() const;

protected:
    
    virtual void buildRenderQueues( base::Node& root, const base::math::Matrix4f& viewTransform ) override;

    virtual void rewindRenderQueues() override;

    virtual void updateRenderQueues( const base::math::Matrix4f& viewTransform ) override;

    virtual void render( const base::Renderable& ) override;

}; // CuttingPlanesStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // CUTTINGPLANESSTAGE_H_6014714286
