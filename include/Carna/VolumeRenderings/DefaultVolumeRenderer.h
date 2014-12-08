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

#ifndef DEFAULTVOLUMERENDERER_H_6014714286
#define DEFAULTVOLUMERENDERER_H_6014714286

/** \file   DefaultVolumeRenderer.h
  * \brief  Defines \ref Carna::VolumeRenderings::DefaultVolumeRenderer.
  */

#include <Carna/VolumeRenderings/VolumeRenderer.h>
#include <Carna/VolumeRenderings/RayMarching.h>
#include <Carna/VolumeRenderings/VolumeVisualization.h>
#include <memory>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: DefaultVolumeRenderer
// ----------------------------------------------------------------------------------

/** \brief  \ref Renderer that implements \em ray \em marching.
  *         This is a technique widely used in 3D graphics for rendering volume data.
  *
  * \section VolumeRendererRayMarching Ray Marching
  *
  * The renderer implementation supplies the volume data as a 3D texture to the
  * OpenGL API. It also computes the front and back face coordinates of each pixel in
  * \ref VolumeSpace "texture space", as illustrated by the picture below: both
  * coordinate mappings, \f$ front(x, y) \f$ and \f$ back(x, y) \f$, are implemented
  * as a RGB 2D texture each, whose color channels do carry the corresponding
  * texture space coordinates for any particular texel.
  *
  * \image html VolumeRenderer1.png "Illustration of the two functions \em front and \em back"
  *
  * \note
  * Notice that in the above image the two exemplary texture space points are mapped to \em different screen space coordinates.
  * The algorithm is interested in such points that are mapped to the \em same screen space coordinates.
  * But illustrating such two points, identified by \f$x_1=x_2\f$ and \f$y_1=y_2\f$, would make them indistinguishable in the image above.
  *
  * Both coordinate mapping textures are bound to an exclusive texture sampler each
  * by the renderer. The actual ray marching rendering algorithm is implemented
  * diversely, dependent on the specific \ref VolumeRendererMode currently used -
  * regardless of this, it receives the texture samplers to use for sampling the
  * coordinate textures as a \ref RayMarching "configuration object" from the
  * renderer.
  *
  * Usually the implementation will compute a \em ray in texture space for each
  * screen space pixel \f$ (x, y) \f$, that intersects the texture volume: the ray
  * will start in \f$ front( x, y ) \f$ and end in \f$ back( x, y ) \f$. This ray
  * will be evaluated in equidistant steps, the distance between two successive ray
  * samples being determined by the current \em sample \em count setting:
  *
  * \f[ \text{sample distance} = \frac{ \sqrt 3 }{ \text{sample count} - 1 } \f]
  *
  * The default sample count setting is:
  *
  * \f[ \text{sample count} = \sqrt{ (\text{voxels along } x_t)^2 + (\text{voxels along } y_t)^2 + (\text{voxels along } z_t)^2 } \f]
  *
  * \see
  * Refer to <a href="http://http.developer.nvidia.com/GPUGems3/gpugems3_ch30.html">
  * ''NVIDIA GPU Gems 3'', chapter 30.3, ''Rendering''</a> for deeper understanding
  * of the technique.
  *
  * \section VolumeRendererMiscellaneous Miscellaneous
  *
  * When the ray marching is finished by the supplied \ref VolumeRendererMode, a
  * further rendering pass is performed in order to apply a set of optional \em post
  * \em processing steps. These include:
  *
  * - \ref setColorPostProcessing "inverting colors"
  * - \ref setMask "overlaying a black circular mask"
  * - \ref setRotation2D "rotating the image"
  * - flipping the image \ref setFlippedHorizontal "horizontally" and/or \ref setFlippedVertical "vertically"
  *
  * \subsection VolumeRendererUsage Usage
  *
  * There is an example on how to use the volume renderer
  * \ref VolumeRenderingExample "here".
  *
  * The \ref VolumeControllerUI class offers an easy-to-use user interface with the
  * purpose of configuring this renderer.
  *
  * \author Leonid Kostrykin
  * \date   8.8.11 - 16.8.12
  */
class CARNA_EXPORT DefaultVolumeRenderer : public VolumeRenderer
{

    Q_OBJECT

public:

    /** \brief  Holds the number of the first texture sampler, \ref VolumeRendererMode
      *         implementations are allowed to use if they need to provide additional
      *         textures to their shaders.
      */
    const static unsigned int FIRST_GENERAL_PURPOSE_TEXTURE_SAMPLER;


    /** \brief  Instantiates.
      */
    DefaultVolumeRenderer
        ( base::Visualization& module
        , base::view::SceneProvider& provider
        , QObject* parent = nullptr );

    /** \brief  Releases acquired GL resources.
      */
    virtual ~DefaultVolumeRenderer();


    virtual unsigned int firstGeneralPurposeTextureSampler() override
    {
        return FIRST_GENERAL_PURPOSE_TEXTURE_SAMPLER;
    }

    virtual int sampleCount() const override;

    virtual ColorOperation colorPostProcessing() const override;

    virtual bool hasMask() const override;

    virtual bool hasMode() const override;

    virtual const VolumeRendererModes& modes() const override;

    virtual VolumeRendererMode& mode() override;


    virtual void installMode( VolumeRendererMode* mode ) override;

    virtual bool hasModeInstalled( VolumeRendererMode& ) const override;

    virtual bool hasModeInstalled( const std::string& ) const override;


    virtual const base::Transformation& getObjectsModelView() const override;

    
    virtual double minX() const override;
    
    virtual double maxX() const override;
    
    virtual double minY() const override;
    
    virtual double maxY() const override;
    
    virtual double minZ() const override;
    
    virtual double maxZ() const override;


    virtual bool isFlippedHorizontal() const override;
    
    virtual bool isFlippedVertical() const override;

    
    virtual unsigned int rotation2D() const override;


public slots:

    virtual void setMode( Carna::VolumeRenderings::VolumeRendererMode& newMode ) override;

    virtual void setMode( const std::string& modeName ) override;

    virtual void setNoMode() override;


    virtual void setRotationOffset( const Carna::base::Transformation& ) override;

    virtual void setRotationCenter( const Carna::base::Vector& ) override;


    virtual void setSampleCount( int sample_count ) override;

    virtual void setColorPostProcessing( Carna::VolumeRenderings::VolumeRenderer::ColorOperation ) override;

    virtual void setMask( bool ) override;
    
    virtual void setFlippedHorizontal( bool ) override;
    
    virtual void setFlippedVertical( bool ) override;
    
    virtual void setRotation2D( int ) override;


    virtual void setMinX( double minX ) override;
    
    virtual void setMaxX( double maxX ) override;
    
    virtual void setMinY( double minY ) override;
    
    virtual void setMaxY( double maxY ) override;
    
    virtual void setMinZ( double minZ ) override;
    
    virtual void setMaxZ( double maxZ ) override;


    virtual void setTargetSize
        ( unsigned int width
        , unsigned int height ) override;


protected:

    virtual void renderColor() override;

    virtual void renderDepth() override;
    
    /** \brief  Queries the \ref CoordinateSystems "scene matrix".
      *
      * This view's scene matrix is defined by:
      *
      * \f[ S = S_0 \cdot M_0 \cdot M_R \f]
      *
      * with
      *
      * \f[
      * M_R =
      * \begin{pmatrix}
      * 1 & 0 & 0 & r_x - 0.5 \\
      * 0 & 1 & 0 & r_y - 0.5 \\
      * 0 & 0 & 1 & r_z - 0.5 \\
      * 0 & 0 & 0 & 0 \\
      * \end{pmatrix}
      * \cdot
      * R
      * \cdot
      * \begin{pmatrix}
      * 1 & 0 & 0 & -r_x \\
      * 0 & 1 & 0 & -r_y \\
      * 0 & 0 & 1 & -r_z \\
      * 0 & 0 & 0 & 0 \\
      * \end{pmatrix}
      * \f]
      *
      * and \f$R\f$ being the \ref setRotationOffset "rotation offset" and
      * \f$\vec{r}\f$ the \ref setRotationCenter "rotation center" in volume units.
      */
    virtual void fetchSceneMatrix
        ( base::Transformation& sceneMatrix
        , const base::Transformation& cameraMatrix
        , const base::Transformation& projectionMatrix ) const override;


private:

    class Details;
    std::unique_ptr< Details > pimpl;


    /** \brief  Activates the given mode.
      *
      * Also \ref Renderer::invalidate "invalidates the renderer".
      *
      * \pre    <code>textureID != nullptr</code> which means that \ref initializeGL
      *                                           has been invoked priorly.
      * \pre    <code>hasMode( newMode ) == true</code>
      * \post   <code>&mode() == &newMode</code>
      *
      * \todo   Move to DefaultVolumeRenderer::Details
      */
    void activateMode( VolumeRendererMode& newMode );

    /** \todo   Move to DefaultVolumeRenderer::Details
      */
    void emitVolumeBoundingBox() const;


signals:

    /** \brief  Another \ref VolumeRendererMode has been set.
      */
    void modeChanged();

    /** \brief  Another \ref VolumeRendererMode has been set.
      */
    void modeChanged( Carna::VolumeRenderings::VolumeRendererMode& newMode );

    /** \brief  \ref setNoMode was invoked and now no mode is active.
      */
    void modeChangedToNone();


    /** \brief  %Volume cropping settings have been changed.
      */
    void xMaxChanged( double );
    
    /** \brief  %Volume cropping settings have been changed.
      */
    void xMinChanged( double );
    
    /** \brief  %Volume cropping settings have been changed.
      */
    void yMaxChanged( double );
    
    /** \brief  %Volume cropping settings have been changed.
      */
    void yMinChanged( double );
    
    /** \brief  %Volume cropping settings have been changed.
      */
    void zMaxChanged( double );

    /** \brief  %Volume cropping settings have been changed.
      */
    void zMinChanged( double );

}; // VolumeRenderings :: DefaultVolumeRenderer



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // DEFAULTVOLUMERENDERER_H_6014714286
