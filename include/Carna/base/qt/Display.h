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

#ifndef DISPLAY_H_6014714286
#define DISPLAY_H_6014714286

/** \file   Display.h
  * \brief  Defines \ref Carna::base::qt::Display.
  */

#include <QGLWidget>
#include <stack>
#include <queue>
#include <Carna/base/noncopyable.h>
#include <Carna/base/Transformation.h>
#include <Carna/base/view/FramebufferObject.h>
#include <Carna/base/view/Camera.h>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// Display
// ----------------------------------------------------------------------------------

/** \brief
  * Simple widget that's only purpose is embedding any given \ref Visualization "visualization" into a Qt based user interface.
  *
  * All displays do share GL objects - such as textures or shaders - among their contexts.
  *
  * If you need to grab the rendering from the display, you must ensure that the rendering is finished by the moment you call \c grabFrameBuffer:
  *
  * \code
  * QApplication::processEvents();
  * display->updateGL();
  * QImage framebuffer = display->grabFrameBuffer();
  * \endcode
  *
  * All displays do share GL objects - such as textures or shaders - among their contexts.
  *
  * \author Leonid Kostrykin
  * \date   8.6.11 - 15.3.13
  */
class CARNA_EXPORT Display : public QGLWidget
{

    Q_OBJECT

    NON_COPYABLE

public:

    /** \brief
      * Instantiates and sets the visualization \a module to be shown within the instantiated display.
      *
      * Takes ownership of the supplied visualization module.
      *
      * \pre
      * <code>module->isInitialized() == false</code>
      *
      * The visualization \a module will be \ref Visualization::initialize "initialized" with
      * the given scene \a provider, hence the visualization's renderer will be
      * \ref Visualization::createRenderer "created" with the same. The initialization
      * will be performed when the display becomes visible for the first time:
      * speaking strictly, it will be the moment that \ref initializeGL is invoked.
      */
    Display
        ( Visualization* module
        , view::SceneProvider& provider
        , QWidget* parent = nullptr );
    
    /** \brief
      * Instantiates.
      *
      * All displays do share GL objects - such as textures or shaders - among their contexts.
      *
      * Use \ref setModule to determine which visualization is to be shown within the newly instantiated display.
      *
      * \since \ref v_2_5
      */
    explicit Display
        ( view::SceneProvider& provider
        , QWidget* parent = nullptr );


    /** \brief  Releases acquired resources.
      */
    virtual ~Display();

// ----------------------------------------------------------------------------------
    
    /** \brief
      * Tells whether any module has been set yet, either through \ref setModule or the constructor.
      *
      * \since \ref v_2_5
      */
    bool hasModule() const;

    /** \brief
      * References the mutable composed visualization module.
      */
    Visualization& module();
    
    /** \brief
      * References the immutable composed visualization module.
      */
    const Visualization& module() const;

    /** \brief
      * Sets the visualization \a module that is to be shown within this display.
      *
      * \pre
      * <code>module != nullptr</code>
      * <code>module->isInitialized() == false</code>
      *
      * If there was another module set previously, either through this method or the
      * constructor, it is destroyed before the method finishes.
      *
      * The visualization \a module will be \ref Visualization::initialize "initialized"
      * with the \ref view::SceneProvider "scene provider" that was set by the constructor,
      * hence the visualization's renderer will be \ref Visualization::createRenderer "created"
      * with the same. The initialization will be performed immediately if and only if this
      * display already has become visible once, or when the display becomes visible for the
      * first time: this will be the moment that \ref initializeGL is invoked.
      *
      * \since \ref v_2_5
      */
    void setModule( Visualization* module );

// ----------------------------------------------------------------------------------

protected:

    /** \brief
      * Initializes the GL context and the visualization module that was provided to
      * the constructor. As a consequence of the module's initialization, a specific
      * renderer is created.
      *
      * \post
      * <code>module()->isInitialized() == true</code>
      */
    virtual void initializeGL() override;

    /** \brief  Triggers the renderer and presents the results.
      *
      * Invokes \ref view::Renderer::paint particularily.
      *
      * \pre
      * <code>module()->isInitialized() == true</code>
      */
    virtual void paintGL() override;

    /** \brief  Triggers \ref view::Renderer::setTargetSize.
      *
      * \pre
      * <code>module()->isInitialized() == true</code>
      */
    virtual void resizeGL( int w, int h ) override;

    /** \brief  Delegates the event to the visualization's controller.
      */
    virtual void wheelEvent( QWheelEvent* ev ) override;
    
    /** \brief  Delegates the event to the visualization's controller.
      */
    virtual void mousePressEvent( QMouseEvent* ev ) override;
    
    /** \brief  Delegates the event to the visualization's controller.
      */
    virtual void mouseMoveEvent( QMouseEvent* ev ) override;
    
    /** \brief  Delegates the event to the visualization's controller.
      */
    virtual void mouseReleaseEvent( QMouseEvent* ev ) override;

// ----------------------------------------------------------------------------------

private:

    class Details;
    std::unique_ptr< Details > pimpl;

}; // Display



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // DISPLAY_H_6014714286
