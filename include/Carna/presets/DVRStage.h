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

#ifndef DVRSTAGE_H_6014714286
#define DVRSTAGE_H_6014714286

#include <Carna/presets/VolumeRenderingStage.h>
#include <Carna/Carna.h>
#include <memory>

/** \file   DVRStage.h
  * \brief  Defines \ref Carna::presets::DVRStage.
  */

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// DVRStage
// ----------------------------------------------------------------------------------

/** \brief
  * Performs *direct volume renderings* of the volume geometries in the scene.
  *
  * \author Leonid Kostrykin
  * \date   25.3.15
  */
class CARNA_LIB DVRStage : public VolumeRenderingStage
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Holds the \ref GeometryFeatures "role" that volume data is expected to take
      * when attached to \ref base::Geometry nodes.
      */
    const static unsigned int ROLE_HU_VOLUME = 0;

    const static unsigned int ROLE_NORMALS = 1;
    
    const static float DEFAULT_TRANSLUCENCE;
    
    /** \brief
      * Instantiates. The created stage will render such \ref base::Geometry scene
      * graph nodes, whose \ref GeometryTypes "geometry types" equal \a geometryType.
      */
    DVRStage( unsigned int geometryType );

    /** \brief
      * Deletes.
      */
    virtual ~DVRStage();

    virtual void reshape( const base::FrameRenderer& fr, unsigned int width, unsigned int height ) override;

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;
        
    void clearColorMap();
    
    void writeColorMap( const base::math::Span< base::HUV >& huRange, const base::math::Span< base::Color > colorRange );
    
    void setTranslucence( float translucence );
    
    float translucence() const;
    
    /** \overload
      */
    void writeColorMap( base::HUV huFirst, base::HUV huLast, const base::Color& colorFirst, const base::Color& colorLast );

protected:

    virtual unsigned int loadVideoResources() override;

    virtual void createVolumeSamplers( const std::function< void( unsigned int, base::Sampler* ) >& registerSampler ) override;

    /** \brief
      * Acquires the `dvr` shader from the \ref base::ShaderManager.
      */
    virtual const base::ShaderProgram& acquireShader() override;

    /** \brief
      * Maps \ref ROLE_HU_VOLUME to `huVolume`.
      */
    virtual const std::string& uniformName( unsigned int role ) const override;

    virtual void configureShader() override;
    
    /** \brief
      * Computes and uploads the normals transformation matrix.
      */
    virtual void configureShader( const base::Renderable& ) override;

}; // DVRStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // DVRSTAGE_H_6014714286
