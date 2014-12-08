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

#ifndef SLICEPLANES_SLICEPLANE_DETAILS_H_6014714286
#define SLICEPLANES_SLICEPLANE_DETAILS_H_6014714286

/** \file   SlicePlaneDetails.h
  * \brief  Defines \ref Carna::SlicePlanes::SlicePlane::Details.
  */

#include <Carna/SlicePlanes/SlicePlane.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/model/Volume.h>

class QGLContext;
class QGLFramebufferObject;
class QGLPixelBuffer;

namespace Carna
{

namespace SlicePlanes
{



// ----------------------------------------------------------------------------------
// SlicePlane :: Details
// ----------------------------------------------------------------------------------

class SlicePlane::Details
{

    NON_COPYABLE

protected:

    SlicePlane& self;

public:

    const static double WINDOW_SIZE;


    Details
        ( SlicePlane& self
        , const base::model::Scene& model );

    virtual ~Details();


    virtual void activateContext() = 0;

    virtual unsigned int getVolumeTextureID() const = 0;


    void initialize();


    /** \brief  Holds size of current volumetric 3D texture.
      */
    const base::Vector3ui myTextureSize;


    /** \brief  Holds framebuffer object dimensions.
      */
    const unsigned int fboSizeXY;

    /** \brief  Holds associated framebuffer object.
      */
    std::unique_ptr< QGLFramebufferObject > fbo;


    std::unique_ptr< base::view::ShaderBundle > shader;


    base::Transformation tangent_to_volume;


    bool valid;

}; // SlicePlane :: Details



// ----------------------------------------------------------------------------------
// SlicePlane :: ExclusiveModeDetails
// ----------------------------------------------------------------------------------

class SlicePlane::ExclusiveModeDetails : public SlicePlane::Details
{

public:

    ExclusiveModeDetails
        ( SlicePlane& self
        , const base::model::Scene& model );


    const std::unique_ptr< QGLPixelBuffer > pbuffer;

    const std::unique_ptr< base::model::Volume::Texture > volumeTexture;


    virtual void activateContext() override;

    virtual unsigned int getVolumeTextureID() const override;


private:

    const QGLContext* const glContext;

};  // SlicePlane :: ExclusiveModeDetails



// ----------------------------------------------------------------------------------
// SlicePlane :: SharedModeDetails
// ----------------------------------------------------------------------------------

class SlicePlane::SharedModeDetails : public SlicePlane::Details, public base::view::SceneProvider::ResourceClient
{

public:

    SharedModeDetails
        ( SlicePlane& self
        , base::VisualizationEnvironment& environment
        , base::view::SceneProvider& provider );


    base::VisualizationEnvironment& environment;


    virtual void activateContext() override;

    virtual unsigned int getVolumeTextureID() const override;

};  // SlicePlane :: SharedModeDetails



}  // namespace Carna :: SlicePlanes

}  // namespace Carna

#endif // SLICEPLANES_SLICEPLANE_DETAILS_H_6014714286
