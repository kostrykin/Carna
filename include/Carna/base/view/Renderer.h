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

#ifndef RENDERER_H_6014714286
#define RENDERER_H_6014714286

/** \file   Renderer.h
  * \brief  Defines \ref Carna::base::view::Renderer.
  */

#include <Carna/Carna.h>
#include <Carna/base/Transformation.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/Association.h>
#include <QObject>
#include <string>

class QPoint;
class QGLContext;

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Renderer
// ----------------------------------------------------------------------------------

/** \brief
  * Represents the view component of a \ref Visualization "visualization" and implements its rendering logic.
  *
  * \attention
  * You \em \em must not execute one and the same renderer within different GL contexts.
  * You \em must assure that the contexts of all renderers do share their GL objects.
  * The most simple way of accomplishing both requirements is using the \ref qt::Display class.
  *
  * This is an abstract base class. Implementations must provide definitions for
  * \ref Renderer::renderColor "renderColor" and
  * \ref Renderer::renderDepth "renderDepth". Providing a null-implementation for
  * \ref Renderer::renderDepth "renderDepth" might cause certain render modes, such as
  * the \ref stereoscopic::Philips "Philips stereoscopic" one, not to work properly.
  *
  * \section RendererComponents Components
  *
  * Each renderer instance is associated with a set of objects that determine the
  * renderer's behavior. These objects and the supported types of their association
  * are listed by the table below.
  *
  * <table>
  * <tr><th>Object class</th><th>Aggregation supported</th><th>Composition supported</th></tr>
  * <tr><td>\ref Camera</td><td>X</td><td>X</td></tr>
  * <tr><td>\ref RenderMode</td><td> </td><td>X</td></tr>
  * <tr><td>\ref Object3DPainter</td><td>X</td><td>X</td></tr>
  * <tr><td>\ref SceneProvider</td><td>X</td><td> </td></tr>
  * </table>
  *
  * \subsection RendererCameraManagement Camera Management
  *
  * The purpose of a camera is to define the coordinate mapping from scene into screen
  * space. Refer to the illustration below for a better understanding. The camera may
  * be aggregated (using \ref Aggregation) as well as composed
  * (using \ref Composition) and is supplied via \ref setCamera. Each renderer
  * is instantiated with a \ref DefaultCamera instance set initially.
  *
  * \see Refer to the \ref RendererFeatures section for details on the coordinate transformation pipeline.
  *
  * \subsection RendererRenderModes Render Modes
  *
  * The \ref RenderMode "render modes" concept enables 3D displays support. With
  * the simple \ref Monoscopic mode set by default, the 3D display support is disabled.
  * It may be enabled via \ref setRenderMode. %Carna comes with two built-in stereoscopic
  * render modes, which do enable support of certain 3D displays from
  * \ref stereoscopic::Philips "Philips" and \ref stereoscopic::Zalman "Zalman".
  *
  * \subsection RendererObject3DManagement 3D Objects Management
  *
  * The method \ref paintObjects3D is responsible of rendering \ref model::Object3D "3D objects".
  * All it basically does is invoking the \ref Object3DPainter, that can be supplied
  * via \ref setObject3DPainter. By default an instance of \ref DefaultObject3DPainter is
  * being used, that simply invokes the painting routine, that has been supplied to
  * \ref paintObjects3D, for each 3D object of the data model. If no painting routine is
  * supplied explicitly, an instance of \ref DefaultObject3DPaintingRoutine is used,
  * that simply invokes the \ref model::Object3D::paint "paint" method of each object.
  *
  * \subsection RendererDataModelInterface Data Model Interface
  *
  * Finally, a reference to a \ref SceneProvider object must be provided to the renderer's
  * constructor, in order to specify the \ref model::Scene "data model" interface to use.
  * The renderer registers itself as a \ref SceneProvider::ResourceClient "client" of the
  * scene provider.
  *
  * \section RendererFeatures Integrated Logics
  *
  * This class provides a simple shader manager. Refer to it's \ref shader methods
  * documentation for details.
  *
  * \image html 3D-Transformation.png
  *
  * %Renderer implementations are allowed to provide any custom scene matrix, by
  * overriding their \ref fetchSceneMatrix method. The coordinate transformation
  * pipeline is illustrated by the above figure. Refer to the \ref CoordinateSystems
  * page for a more detailed description.
  *
  * \author Leonid Kostrykin
  * \date   5.3.13 - 18.3.13
  */
class CARNA_EXPORT Renderer : public QObject
{

    NON_COPYABLE

    Q_OBJECT

public:

    // ------------------------------------------------------------------------------
    // General
    // ------------------------------------------------------------------------------

    
    /** \brief Instantiates and initializes the \ref Renderer base class.
     *
     * \param module
     * References the module this renderer is the view component of. The \ref module
     * attribute is initialized with the reference supplied here.
     *
     * \param provider
     * References the data model interface to use. The \ref provider attribute is
     * initialized with the reference supplied here.
     *
     * \param parent
     * Makes the instance a child of \a parent if <code>parent != nullptr</code>.
     *
     * Invokes \c glewInit.
     *
     * \since \ref v_2_3
     */
    Renderer
        ( Visualization& module
        , SceneProvider& provider
        , QObject* parent = nullptr );

    /** \brief
      * Activates the GL context provided by the \ref Visualization::environment "supplied visualization enviroment".
      */
    virtual ~Renderer();

    /** \brief
      * References the aggregated data model interface.
      */
    SceneProvider& provider;
    
    /** \brief
      * References the mutable module this renderer is the view component of.
      *
      * \post
      * <code>!module().isInitialized() || &module().renderer() == this</code>
      */
    Visualization& module();
    
    /** \brief
      * References the immutable module this renderer is the view component of.
      *
      * \post
      * <code>!module().isInitialized() || &module().renderer() == this</code>
      */
    const Visualization& module() const;

    // ------------------------------------------------------------------------------
    // Camera Management
    // ------------------------------------------------------------------------------

    /** \brief  Acquires new camera.
      *
      * The camera object, that is acquired at the moment of the invocation of this method, is released.
      *
      * \pre    <code>camera != nullptr && camera->get() != nullptr</code>
      *
      * Example for supplying a camera of type \c MyCamera to some given visualization \c module:
      *
      * \code
      * module->doAfterInitialization( [module]()
      *     {
      *         module->renderer().setCamera( new Carna::Composition< Carna::Camera >( new MyCamera() ) );
      *     }
      * );
      * \endcode
      *
      * Also refer to \ref MinimalExample and \ref LambdaExpressions for more explanations.
      */
    void setCamera( Association< Camera >* camera );

    /** \brief  References the currently acquired immutable camera.
      */
    const Camera& camera() const;

    /** \brief  References the currently acquired mutable camera.
      */
    Camera& camera();

    // ------------------------------------------------------------------------------
    // Transformation Management
    // ------------------------------------------------------------------------------

    /** \brief  Tells the current \ref CoordinateSystems "volume to scene space transformation".
      */
    const Transformation& sceneTransformation() const;
    
    /** \brief  Tells the current \ref CoordinateSystems "scene to view space transformation".
      */
    const Transformation& cameraTransformation() const;
    
    /** \brief  Tells the current \ref CoordinateSystems "volume to view space transformation".
      */
    const Transformation& volumeViewTransformation() const;
    
    /** \brief  Tells the current \ref CoordinateSystems "view to screen space transformation".
      */
    const Transformation& projectionTransformation() const;

    // ------------------------------------------------------------------------------
    // Render Modes
    // ------------------------------------------------------------------------------

    /** \brief
      * Abstract base class for different types of render modes such as simple monoscopic or 3D stereo
      *
      * \ref Renderer has a \f$1-1\f$ composition relation with %RenderMode instances.
      *
      * \author Leonid Kostrykin
      * \date   5.3.13 - 18.3.13
      */
    class CARNA_EXPORT RenderMode
    {

        NON_COPYABLE

    public:

        /** \brief  Instantiates.
          *
          * Associates with the given \ref Renderer.
          */
        RenderMode( Renderer& );

        /** \brief  Does nothing.
          */
        virtual ~RenderMode();


        /** \brief  Invokes \ref initialize(unsigned int,unsigned int) with appropriate arguments.
          */
        virtual void initialize();

        /** \brief
          * Renders the final output to the currently set framebuffer,
          * using \ref renderColorTo and \ref renderDepthTo.
          */
        virtual void paint() = 0;

        /** \brief  Indicates that the rendering target might have been resized.
          */
        virtual void resizeBuffers
            ( unsigned int width
            , unsigned int height );


    protected:

        /** \brief  Associated renderer
          */
        Renderer& renderer;


        /** \brief  Draws result of \ref Renderer::renderColor to given texture.
          */
        void renderColorTo( RenderTexture& target );

        /** \brief  Draws result of \ref Renderer::renderDepth to given texture.
          */
        void renderDepthTo( RenderTexture& target );


        /** \brief  Initializes framebuffer object and invokes \ref RenderMode::postInitialize.
          */
        void initialize
            ( unsigned int width
            , unsigned int height );

        /** \brief  Perform optional initialization routines.
          *
          * Invoked when framebuffer object is initialized.
          */
        virtual void postInitialize() = 0;


    private:

        class Details;
        std::unique_ptr< Details > pimpl;

    }; // Renderer::RenderMode

    // ------------------------------------------------------------------------------
    // Render Mode Management
    // ------------------------------------------------------------------------------

    /** \brief  Acquires new render mode.
      *
      * Previously acquired render mode is released.
      *
      * \pre    <code>renderMode != nullptr</code>
      * \post   <code>renderMode == &renderMode()</code>
      */
    void setRenderMode( RenderMode* renderMode );

    /** \brief  References the currently acquired render mode.
      */
    const RenderMode& renderMode() const;

    // ------------------------------------------------------------------------------
    // Shader Management
    // ------------------------------------------------------------------------------
    
    /** \brief  References the shader program, identified by the given \a id.
      *
      * The shaders are loaded and identified by the given \a id, which is passed
      * to \ref ShaderBundle constructor. Refer to its documentation for further
      * information.
      *
      * Acquiring each shader on a per-renderer level would be a waste of resources.
      * Since all renderers are assumed to be sharing their GL objects, the underlying
      * implementation only acquires the shaders if they yet haven't been acquired by
      * any other renderer. The shaders do stay acquired until the last renderer is
      * destroyed - the size of shader objects is magnitudes smaller than the size of
      * the volume data's 3D texture, therefore this weakness in memory management is
      * negligible.
      */
    ShaderProgram& shader( const std::string& id ) const;

    /** \brief  References the shader program, composed by specified vertex and fragment shaders.
      *
      * Acquiring each shader on a per-renderer level would be a waste of resources.
      * Since all renderers are assumed to be sharing their GL objects, the underlying
      * implementation only acquires the shaders if they yet haven't been acquired by
      * any other renderer. The shaders do stay acquired until the last renderer is
      * destroyed - the size of shader objects is magnitudes smaller than the size of
      * the volume data's 3D texture, therefore this weakness in memory management is
      * negligible.
      */
    ShaderProgram& shader
        ( const std::string& vertex_shader_file
        , const std::string& fragment_shader_file ) const;

    // ------------------------------------------------------------------------------
    // Object3D Management
    // ------------------------------------------------------------------------------

    /** \brief  Acquires new object painter.
      *
      * Previously acquired object painter is released.
      *
      * \pre    <code>painter != nullptr && painter->get() != nullptr</code>
      *
      * \see
      * The method \ref paintObjects3D is responsible of rendering \ref model::Object3D "3D objects".
      * By default an instance of \ref DefaultObject3DPainter is being used, that simply
      * invokes the painting routine, that is being supplied to \ref paintObjects3D.
      */
    void setObject3DPainter( Association< Object3DPainter >* painter );

    /** \brief
      * Paints 3D objects through invocation of the \ref setObject3DPainter "previously supplied Object3DPainter" with the given painting routine.
      *
      * By default an instance of \ref DefaultObject3DPainter is being used, that simply
      * invokes the given painting routine on each 3D object of the data model.
      */
    void paintObjects3D( Object3DPaintingRoutine& ) const;

    /** \brief
      * Paints 3D objects through invocation of the \ref setObject3DPainter "previously supplied Object3DPainter".
      *
      * By default an instance of \ref DefaultObject3DPainter is being used, that simply
      * invokes the \ref model::Object3D::paint "paint" operation on each 3D object of the data model.
      */
    void paintObjects3D() const;

    /** \brief  Returns object painted most lately at given point in render target space or \c nullptr.
      *
      * \todo   Enhancement: only redraw the picking buffer if the scene has been repainted meanwhile.
      */
    model::Object3D* pickObject3D( const QPoint& ) const;

    // ------------------------------------------------------------------------------
    // Rendering
    // ------------------------------------------------------------------------------
    
    /** \brief  Runs all drawing operations.
      *
      * \see
      * The \ref invalidated signal is emitted whenever the result, produced by this
      * method, may have changed in relation to the result produced the last time the
      * method was invoked.
      *
      * The basic program flow is shown below:
      *
      * \code
      * camera().openShutter( *this );
      *
      * renderMode().paint();
      *
      * camera().closeShutter( *this );
      * \endcode
      *
      * Nothing is drawn when the \em rendering \em lock is non zero. The rendering lock
      * can be manually controlled via \ref addRenderingLock and \ref removeRenderingLock,
      * which basically do increment and decrement a counter. The rendering lock is
      * incremented implicitly when the \c paint routine catches an exception. So, if you
      * want to proceed rendering, even if an exceptional state was raised, you would
      * have to do something like this:
      *
      * \code
      * try
      * {
      *     renderer.paint();
      * }
      * catch( ... )
      * {
      *     renderer.removeRenderingLock();
      *     throw;
      * }
      * \endcode
      */
    virtual void paint();
    
    /** \brief  Updates dimensions of the rendering buffers.
      *
      * The arguments \a width and \a height are allowed to be \c 0.
      */
    virtual void setTargetSize
        ( unsigned int width
        , unsigned int height );

    /** \brief  Paints a 2D rectangle, covering the whole viewport by default.
      *
      * \pre
      * <code>x0 < x1 && y0 < y1</code>
      *
      * \image html Renderer.drawRect.png "render target coordinate system"
      *
      * Corresponding texture coordinates are assigned to the four vertices,
      * that do not depend on the arguments \a x0, \a y0, \a x1 and \a y1:
      *
      * - Bottom left vertex: \f$( 0, 0 )\f$
      * - Bottom right vertex: \f$( 1, 0 )\f$
      * - Top left vertex: \f$( 0, 1 )\f$
      * - Top right vertex: \f$( 1, 1 )\f$
      *
      * Useful for processing the results of render-to-texture procedures.
      */
    void paintRectangle
        ( float x0 = 0
        , float y0 = 0
        , float x1 = 1
        , float y1 = 1 );

    /** \brief  Returns the width or the height of the widget, what ever is shorter.
      */
    unsigned int shortestTargetSide() const;

    /** \brief  Sets the viewport to be an as large as possible square.
      *
      * The square is centered within the render target.
      *
      * The render target's width and height are assumed to be \a modWidth and
      * \a modHeight times larger respectively than they actually are.
      *
      * \pre
      * This widget's context is active.
      *
      * \pre
      * <code>modWidth > 0 && modHeightmodHeight > 0</code>
      */
    unsigned int setSquareViewport
        ( float modWidth = 1.f
        , float modHeight = 1.f );

    /** \brief  Tells the currently set render target width.
      *
      * The result is determined by the argument passed to \ref setTargetSize previously.
      */
    unsigned int targetWidth() const;

    /** \brief  Tells the currently set render target height.
      *
      * The result is determined by the argument passed to \ref setTargetSize previously.
      */
    unsigned int targetHeight() const;

    /** \brief
      * Tells the ID of the OpenGL texture object that represents the volume data of the
      * interfaced data model.
      *
      * \post
      * <code>volumeTextureID() > 0</code>
      */
    unsigned int volumeTextureID() const;

    /** \brief
      * Tells the ID of the OpenGL texture object that represents the mask data of the
      * interfaced data model.
      *
      * \post
      * <code>maskTextureID() > 0</code>
      */
    unsigned int maskTextureID() const;


public slots:

    /** \brief Emits the \ref invalidated signal.
      *
      * The \ref qt::Display widget repaints itself when it catches this signal.
      */
    void invalidate();

    /** \brief  Disables the \ref paint method.
      *
      * Refer to \ref paint documentation for details.
      */
    void addRenderingLock();
    
    /** \brief  Enables the \ref paint method.
      *
      * Refer to \ref paint documentation for details.
      */
    void removeRenderingLock();


protected:

    /** \brief  Paints the scene to the currently set framebuffer.
      */
    virtual void renderColor() = 0;

    /** \brief  Paints scene's depth buffer to currently set framebuffer.
      */
    virtual void renderDepth() = 0;


    /** \brief  Queries the scene matrix.
      *
      * The default \ref Renderer implementation gives the
      * \ref SceneSpace "default transformation from volume space into scene space",
      * hence \a sceneMatrix is set to \f$S_0 \cdot M_0\f$.
      */
    virtual void fetchSceneMatrix
        ( Transformation& sceneMatrix
        , const Transformation& cameraMatrix
        , const Transformation& projectionMatrix ) const;


signals:

    /** \brief
      * Indicates that an invocation of \ref paint may produce a different result it produced
      * the last time it was invoked.
      *
      * The \ref qt::Display widget repaints itself when it catches this signal.
      */
    void invalidated();

    /** \brief  Emitted when the scene's camera has been exchanged via \ref setCamera.
      */
    void cameraExchanged();


private:

    class Details;
    std::unique_ptr< Details > pimpl;

}; // Renderer



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERER_H_6014714286
