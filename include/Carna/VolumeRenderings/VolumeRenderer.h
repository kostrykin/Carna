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

#ifndef VOLUMERENDERER_H_6014714286
#define VOLUMERENDERER_H_6014714286

/** \file   VolumeRenderer.h
  * \brief  Defines \ref Carna::VolumeRenderings::VolumeRenderer.
  */

#include <Carna/Carna.h>
#include <Carna/base/view/Renderer.h>
#include <memory>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: VolumeRenderer
// ----------------------------------------------------------------------------------

/** \brief  Abstract \ref Renderer specialization that is supposed to utilize \ref VolumeRendererRayMarching as it's rendering technique.
  *         This is a generalization of the \ref DefaultVolumeRenderer class.
  *
  * When the ray marching is finished, a further rendering pass is performed in order
  * to apply a set of optional \em post \em processing steps. These steps include:
  *
  * - \ref setColorPostProcessing "inverting colors"
  * - \ref setMask "overlaying a black circular mask"
  * - \ref setRotation2D "rotating the image"
  * - flipping the image \ref setFlippedHorizontal "horizontally" and/or \ref setFlippedVertical "vertically"
  *
  * The \ref VolumeControllerUI class offers an easy-to-use user interface with the purpose of configuring this renderer.
  *
  * \since  \ref v_2_3
  * \author Leonid Kostrykin
  * \date   18.11.2013
  */
class CARNA_EXPORT VolumeRenderer : public base::view::Renderer
{

    Q_OBJECT

public:

    /** \brief  Lists supported color post processing modes.
      *
      * These values are used by \ref setColorPostProcessing and \ref colorPostProcessing.
      */
    enum ColorOperation
    {
        noColorOperation = 0,
        invertBlackAndWhite = 1,
        invertAllColors = 2
    };

    /** \brief  Ordered list of volume rendering modes.
      */
    typedef std::vector< VolumeRendererMode* > VolumeRendererModes;


    /** \brief  Instantiates.
      */
    VolumeRenderer
        ( base::Visualization& module
        , base::view::SceneProvider& provider
        , QObject* parent = nullptr );
            
    /** \brief  Does nothing.
      */    
    virtual~ VolumeRenderer();

    

    /** \brief  Tells the number of the first texture sampler, implementations
      *         are allowed to use if they need to provide additional textures
      *         to their shaders.
      */
    virtual unsigned int firstGeneralPurposeTextureSampler() = 0;

    /** \brief  Tells maximum number of ray samples made per pixel during \ref VolumeRendererRayMarching "ray marching".
      *
      * \post
      * <code>sampleCount() >= 2</code>
      */
    virtual int sampleCount() const = 0;

    /** \brief  Tells currently set color post processing mode.
      */
    virtual ColorOperation colorPostProcessing() const = 0;

    /** \brief  Tells whether a circular mask is blended after usual rendering is done.
      */
    virtual bool hasMask() const = 0;
    
    /** \brief  References ordered list of associated volume rendering modes.
      */
    virtual const VolumeRendererModes& modes() const = 0;

    /** \brief  Tells whether some \ref VolumeRendererMode is currently activated.
      */
    virtual bool hasMode() const = 0;

    /** \brief  References the currently set mutable volume rendering mode.
      *
      * \pre    <code>hasMode()</code>
      */
    virtual VolumeRendererMode& mode() = 0;

    /** \brief  References the currently set immutable volume rendering mode.
      *
      * \pre    <code>hasMode()</code>
      */
    const VolumeRendererMode& mode() const
    {
        return const_cast< VolumeRenderer* >( this )->mode();
    }


    /** \brief  Associates mode with this renderer, but does not activate it.
      *
      * \pre
      * <code>mode != nullptr</code>
      *
      * \post
      * <code>hasModeInstalled(*mode)</code>
      * <code>hasModeInstalled(mode->name)</code>
      */
    virtual void installMode( VolumeRendererMode* mode ) = 0;

    /** \brief
      * Tells whether the given mode is associated with this renderer and can be activated.
      */
    virtual bool hasModeInstalled( VolumeRendererMode& ) const = 0;

    /** \brief
      * Tells whether a mode named \a modeName is associated with this view and can be activated.
      */
    virtual bool hasModeInstalled( const std::string& ) const = 0;


    /** \brief
      * Tells the model view transformation that is being used currently for painting \ref base::model::Object3D "3D objects".
      */
    virtual const base::Transformation& getObjectsModelView() const = 0;

    
    /** \brief  Tells current volume cropping settings.
      *
      * \post   <code>0. <= minX() && minX() <= 1.</code>
      *
      * The default value is \c 0.
      */
    virtual double minX() const = 0;
    
    /** \brief  Tells current volume cropping settings.
      *
      * \post   <code>0. <= maxX() && maxX() <= 1.</code>
      *
      * The default value is \c 1.
      */
    virtual double maxX() const = 0;
    
    /** \brief  Tells current volume cropping settings.
      *
      * \post   <code>0. <= minY() && minY() <= 1.</code>
      *
      * The default value is \c 0.
      */
    virtual double minY() const = 0;
    
    /** \brief  Tells current volume cropping settings.
      *
      * \post   <code>0. <= maxY() && maxY() <= 1.</code>
      *
      * The default value is \c 1.
      */
    virtual double maxY() const = 0;
    
    /** \brief  Tells current volume cropping settings.
      *
      * \post   <code>0. <= minZ() && minZ() <= 1.</code>
      *
      * The default value is \c 0.
      */
    virtual double minZ() const = 0;
    
    /** \brief  Tells current volume cropping settings.
      *
      * \post   <code>0. <= maxZ() && maxZ() <= 1.</code>
      *
      * The default value is \c 1.
      */
    virtual double maxZ() const = 0;


    /** \brief  Tells whether the finished rendering is flipped along x-axis before it is presented.
      */
    virtual bool isFlippedHorizontal() const = 0;
    
    /** \brief  Tells whether the finished rendering is flipped along y-axis before it is presented.
      */
    virtual bool isFlippedVertical() const = 0;

    
    /** \brief  Tells the rotation angle of the finished rendering before it is presented.
      *
      * \post
      * <code>rotation2D() < 360</code>
      *
      * The angle is told in degrees.
      */
    virtual unsigned int rotation2D() const = 0;


public slots:

    /** \brief  Sets current \ref VolumeRendererMode.
      *
      * Also \ref base::view::Renderer::invalidate "invalidates the renderer".
      *
      * \pre    <code>hasModeInstalled( newMode ) == true</code>
      * \post   <code>hasMode() && &mode() == &newMode</code>
      */
    virtual void setMode( Carna::VolumeRenderings::VolumeRendererMode& newMode ) = 0;

    /** \brief  Searches for the first declared mode named \a modeName and activates it.
      *
      * Also \ref base::view::Renderer::invalidate "invalidates the renderer".
      *
      * \pre    <code>hasModeInstalled( modeName ) == true</code>
      * \post   <code>hasMode() && mode().name == modeName</code>
      */
    virtual void setMode( const std::string& modeName ) = 0;

    /** \brief  Searches for the first declared mode named \a modeName and activates it.
      *
      * Prefer the \c std::string version of this method.
      *
      * Also \ref base::view::Renderer::invalidate "invalidates the renderer".
      *
      * \pre    <code>hasModeInstalled( modeName.toStdString() ) == true</code>
      * \post   <code>hasMode() && mode().name == modeName.toStdString()</code>
      */
    void setMode( const QString& modeName )
    {
        setMode( modeName.toStdString() );
    }

    /** \brief  Deactivates currently set \ref VolumeRendererMode.
      *
      * Does \em not emit \ref modeChanged signals but the \ref modeChangedToNone signal instead.
      *
      * \post   <code>hasMode() == false</code>
      */
    virtual void setNoMode() = 0;


    /** \brief  Configures the rotation offset between the volume rendering and the 3D objects.
      *
      * Also \ref base::view::Renderer::invalidate "invalidates the renderer".
      */
    virtual void setRotationOffset( const Carna::base::Transformation& ) = 0;

    /** \brief  Configures the rotation offset's center in volume units.
      *
      * Also \ref base::view::Renderer::invalidate "invalidates the renderer".
      */
    virtual void setRotationCenter( const Carna::base::Vector& ) = 0;


    /** \brief  Sets maximum number of ray samples to make per pixel during \ref VolumeRendererRayMarching "ray marching".
      *
      * Also \ref base::view::Renderer::invalidate "invalidates the renderer".
      *
      * If a value less than \c 2 is passed for \a sample_count, the sample count is set to \c 2 instead.
      */
    virtual void setSampleCount( int sample_count ) = 0;

    /** \brief  Sets color post processing mode.
      *
      * Also \ref base::view::Renderer::invalidate "invalidates the renderer".
      */
    virtual void setColorPostProcessing( Carna::VolumeRenderings::VolumeRenderer::ColorOperation ) = 0;

    /** \brief  Sets whether a circular mask is blended after rendering is done.
      *
      * Also \ref base::view::Renderer::invalidate "invalidates the renderer".
      */
    virtual void setMask( bool ) = 0;
    
    /** \brief  Sets whether the finished rendering is flipped along x-axis before it is presented.
      */
    virtual void setFlippedHorizontal( bool ) = 0;
    
    /** \brief  Sets whether the finished rendering is flipped along y-axis before it is presented.
      */
    virtual void setFlippedVertical( bool ) = 0;
    
    /** \brief  Sets the rotation angle of the finished rendering before it is presented.
      *
      * The angle is to be supplied in degrees.
      */
    virtual void setRotation2D( int ) = 0;


    /** \brief  Configures volume cropping.
      *
      * \pre    <code>0. <= minX && minX <= 1.</code>
      *
      * Emits the \ref xMinChanged signal.
      * The default value is \c 0.
      */
    virtual void setMinX( double minX ) = 0;
    
    /** \brief  Configures volume cropping.
      *
      * \pre    <code>0. <= maxX && maxX <= 1.</code>
      *
      * Emits the \ref xMaxChanged signal.
      * The default value is \c 1.
      */
    virtual void setMaxX( double maxX ) = 0;
    
    /** \brief  Configures volume cropping. The argument must be in \f$[0,1]\f$.
      *
      * \pre    <code>0. <= minY && minY <= 1.</code>
      *
      * Emits the \ref yMinChanged signal.
      * The default value is \c 0.
      */
    virtual void setMinY( double minY ) = 0;
    
    /** \brief  Configures volume cropping. The argument must be in \f$[0,1]\f$.
      *
      * \pre    <code>0. <= maxY && maxY <= 1.</code>
      *
      * Emits the \ref yMaxChanged signal.
      * The default value is \c 1.
      */
    virtual void setMaxY( double maxY ) = 0;
    
    /** \brief  Configures volume cropping. The argument must be in \f$[0,1]\f$.
      *
      * \pre    <code>0. <= minZ && minZ <= 1.</code>
      *
      * Emits the \ref zMinChanged signal.
      * The default value is \c 0.
      */
    virtual void setMinZ( double minZ ) = 0;
    
    /** \brief  Configures volume cropping. The argument must be in \f$[0,1]\f$.
      *
      * \pre    <code>0. <= maxZ && maxZ <= 1.</code>
      *
      * Emits the \ref zMaxChanged signal.
      * The default value is \c 1.
      */
    virtual void setMaxZ( double maxZ ) = 0;


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

}; // VolumeRenderings :: VolumeRenderer



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // VOLUMERENDERER_H_6014714286
