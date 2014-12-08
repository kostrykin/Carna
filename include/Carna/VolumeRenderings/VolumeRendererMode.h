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

#ifndef VOLUMERENDERERMODE_H_6014714286
#define VOLUMERENDERERMODE_H_6014714286

/** \file   VolumeRendererMode.h
  * \brief  Defines \ref Carna::VolumeRenderings::VolumeRendererMode.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <QObject>

class QWidget;

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: VolumeRendererMode
// ----------------------------------------------------------------------------------

/** \brief
  * Abstract base class for \ref VolumeRendererRayMarching "ray marching" implementations.
  *
  * %Carna comes with several ''out-of-the-box'' implementations:
  *
  * - \ref DRR::XRay "Digital Radiograph Reconstruction (DRR)"
  * - \ref MIP::MaximumIntensityProjection "Multi-Channel Maximum Intensity Projection (MIP)"
  * - \ref DVR::DirectVolumeRendering "Direct Volume Rendering (DVR)"
  *
  * \section VolumeRendererMode_Lifecycle VolumeRendererMode Lifecycle
  * 
  * Each instance of this class runs through a sequence of states as illustrated by
  * the state diagram below.
  *
  * \image html VolumeRendererModeStates.png "VolumeRendererMode states and legal state transitions"
  *
  * The state transitions are documented in detail below. As you can take from the
  * diagram, each instance can only be associated once with a particular renderer.
  *
  * \par Installation
  *
  * Method \ref prepareForRenderer is called by the renderer during
  * \ref VolumeRenderer::installMode "installation". The instance is associated
  * with the given renderer.
  *
  * The implementation acquires all permanently required GL objects, such as
  * textures or framebuffers. This is \em not the right moment for GL state setup
  * to take place.
  *
  * \par Activation / Deactivation
  *
  * Method \ref start is called by the renderer when the mode is activated
  * through \ref VolumeRenderer::setMode. The GL state is set up properly. If
  * there are any timers to start than this is where it should be done.
  *
  * Method \ref stop is called by the renderer when the mode is deactivated
  * through \ref VolumeRenderer::setMode. If there were any timers started
  * priorly, this is when they should be stopped.
  *
  * \section VolumeRendererModeImplementing How to Implement
  *
  * %Volume rendering is usually accomplished by ray marching. Refer to
  * <a href="http://http.developer.nvidia.com/GPUGems3/gpugems3_ch30.html">''NVIDIA
  * GPU Gems 3'', chapter 30.3, ''Rendering''</a> for deeper understanding of the
  * technique.
  *
  * Eventually, you will have to implement \ref renderColor when deriving from this class.
  * You might also want to implement \ref renderDepth, for certain \ref stereoscopic
  * "stereoscopic rendering modes" to work properly, though it isn't always senseful.
  * You will also have to pick a unique mode name, that you will pass to the
  * \ref VolumeRendererMode::VolumeRendererMode "VolumeRendererMode constructor". For
  * convenience reasons you should also create a static public field \c NAME inside
  * your mode implementation class, that will hold that particular name.
  *
  * Consider the code below as a tiny implementation example, that performs a single
  * rendering pass with some custom shader:
  *
  * \code
  * #include <glError.h>
  * #include <RayMarching.h>
  * #include <VolumeRenderer.h>
  *
  * using namespace Carna;
  *
  * void MyVolumeRendererMode::renderColor( RayMarching& rendering )
  * {
  *     REPORT_GL_ERROR;
  *
  *  // try to load the resource files ':/shaders/shader.frag' and
  *  // ':/shaders/shader.vert' and build a shader program from them
  *
  *     ShaderProgram::Binding shader( rendering.renderer.shader( "shader" ) );
  *
  *     REPORT_GL_ERROR;
  *
  *  // setup shader
  *
  *     shader.putUniform1i( "volumeTexture", rendering.volumeTextureSampler );
  *     shader.putUniform1i( "sampleCount", rendering.samples );
  *
  *     REPORT_GL_ERROR;
  *
  *  // invoke the ray marching shader
  *
  *     rendering.renderer.paintRectangle();
  *
  *     REPORT_GL_ERROR;
  * }
  * \endcode
  *
  * The \ref REPORT_GL_ERROR macros are optional.
  *
  * \see
  * The example above illustrates the basic functionality that
  * \ref VolumeRendererSinglePassMode provides, except for the fact that it does
  * allow setting any additional shader arguments by implementing
  * \ref VolumeRendererSinglePassMode::configureShader "configureShader".
  *
  * \author Leonid Kostrykin
  * \date   8.8.11 - 20.3.13
  */
class CARNA_EXPORT VolumeRendererMode : public QObject
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      *
      * \param
      * name
      * names this mode uniquely - pick your clasS' name if in doubt.
      */
    explicit VolumeRendererMode( const std::string& name );


    /** \brief  Releases acquired GL resources.
      */
    virtual ~VolumeRendererMode();


    /** \brief  Holds the name of this mode.
      */
    const std::string name;


    /** \brief  Instantiates and returns this mode's controller UI element or \c nullptr if it has none.
      */
    virtual QWidget* createControllerUI() = 0;

    /** \brief  Performs the rendering.
      */
    virtual void renderColor( RayMarching& ) = 0;

    /** \brief
      * Reproduces the depth buffer belonging to the result of a
      * \ref renderColor invocation as a gray-scale image, that is close in time.
      *
      * The brighter the color, the closer the pixel.
      */
    virtual void renderDepth( RayMarching& ) = 0;


    /** \brief  Associates this mode with a particular renderer.
      *
      * Shall be overridden when any GL objects need to be acquired.
      *
      * \pre    <code>isInitialized() == false</code>
      */
    virtual void prepareForRenderer( VolumeRenderer& renderer );

    /** \brief  Invoked when this mode is start.
      *
      * \pre    <code>isInitialized()</code>
      *
      * This is the right place e.g. for starting timers or setting up the GL state.
      */
    virtual void start();

    /** \brief  Invoked when this mode is stop.
      *
      * \pre    <code>isInitialized()</code>
      *
      * This is the right place e.g. for stopping timers.
      */
    virtual void stop();


    /** \brief  Must be invoked when associated renderer is resized.
      *         The \ref VolumeRenderer implementations does so.
      *
      * \pre    <code>isInitialized()</code>
      *
      * Shall be reimplemented when any buffers need to be resized.
      */
    virtual void resizeBuffers( int width, int height );


    /** \brief  References the \ref VolumeRenderer this mode was associated with.
      *
      * \pre    <code>isInitialized()</code>
      */
    VolumeRenderer& renderer();

    /** \brief  References the \ref VolumeRenderer this mode was associated with.
      *
      * \pre    <code>isInitialized()</code>
      */
    const VolumeRenderer& renderer() const
    {
        return const_cast< VolumeRendererMode* >( this )->renderer();
    }


    /** \brief
      * Tells whether \ref prepareForRenderer already has been invoked.
      *
      * This is usually done through \ref VolumeRenderer::installMode.
      * Also see the \ref VolumeRendererMode_Lifecycle "lifecycle documentation".
      */
    bool isInitialized() const
    {
        return currentRenderer != 0;
    }


public slots:

    /** \brief
      * Invalidates the \ref renderer this mode \ref VolumeRenderer::installMode "was installed to".
      *
      * If this mode hasn't been installed yet, nothing happens.
      *
      * \since \ref v_2_1
      */
    void invalidate();


protected:

    /** \brief
      * Paints 3D objects with respect to the renderer's current \ref VolumeRenderer::setRotationOffset "rotation offset setting".
      *
      * \pre    <code>isInitialized()</code>
      *
      * When deriving from this class, one should always prefer this method for drawing
      * \ref base::model::Object3D "3D objects" over <code>renderer().provider.paintObjects3D()</code>.
      */
    void paintObjects3D() const;


private: // TODO: implement PIMPL

    /** \brief  References the \ref VolumeRenderer this \c %VolumeRendererMode was associated
      *         with \ref prepareForRenderer "when initialized".
      */
    VolumeRenderer* currentRenderer;

}; // VolumeRenderings :: VolumeRendererMode



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // VOLUMERENDERERMODE_H_6014714286
