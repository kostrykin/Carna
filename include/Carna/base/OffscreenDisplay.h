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

#ifndef OFFSCREENDISPLAY_H_6014714286
#define OFFSCREENDISPLAY_H_6014714286

/** \file   OffscreenDisplay.h
  * \brief  Defines \ref Carna::base::OffscreenDisplay.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <QObject>

class QImage;

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// OffscreenDisplay
// ----------------------------------------------------------------------------------

/** \brief
  * Performs offscreen rendering to texture.
  *
  * \warning
  * This class wasn't working until \ref v_2_1_5.
  *
  * \see
  * There is also an \ref OffscreenRenderingExample "example on offscreen-rendering".
  *
  * \since  \ref v_2_1_3
  * \author Leonid Kostrykin
  * \date   29.4.13 - 22.5.13
  */
class CARNA_EXPORT OffscreenDisplay : public QObject
{

    Q_OBJECT

    NON_COPYABLE

public:

    /** \brief
      * Initializes the visualization \a module with the scene \a provider and the visualization \a environment,
      * and all buffers according to the \a width and \a height.
      *
      * \post <code>module->isInitialized()</code>
      */
    OffscreenDisplay
        ( OffscreenVisualizationEnvironment& environment
        , unsigned int width, unsigned int height
        , Visualization* module
        , view::SceneProvider& provider );

    /** \brief
      * Releases acquired resources.
      */
    ~OffscreenDisplay();

 // ----------------------------------------------------------------------------------

    /** \brief
      * References the supplied offscreen-rendering environment.
      */
    OffscreenVisualizationEnvironment& environment;

    /** \brief
      * References the mutable composed visualization module.
      */
    Visualization& module();
    
    /** \brief
      * References the immutable composed visualization module.
      */
    const Visualization& module() const;

 // ----------------------------------------------------------------------------------

    /** \brief  Resizes all buffers if necessary.
      *
      * \post   <code>width() == newWidth && height() == newHeight</code>
      *
      * The display becomes invalidated only when the size really got changed.
      */
    void resize( unsigned int newWidth, unsigned int newHeight );

    /** \brief  Tells current target's width.
      */
    unsigned int width() const;
    
    /** \brief  Tells current target's height.
      */
    unsigned int height() const;

 // ----------------------------------------------------------------------------------

    /** \brief  Tells whether repaint is scheduled.
      */
    bool isInvalidated() const;

    /** \brief  References the image grabbed from the framebuffer most lately.
      *
      * If the display \ref isInvalidated "was invalidated", \ref paint "repaint" is performed first.
      *
      * The returned image lives on as long as
      * - neither the \c %OffscreenDisplay instance, it is retrieved from, is destroyed,
      * - nor the framebuffer is \ref resize "resized".
	  *
	  * Nevertheless, even when the image is destroyed because of the framebuffer being resized,
	  * references do stay legal, since the new image is allocated at the same position as the previous:
	  *
	  * \code
	  * const std::unique_ptr< OffscreenDisplay > display( createDisplay() );
	  *
	  * QImage& frame1 = display->framebuffer();
	  *
	  * display->resize( display->width() + 1, display->height() + 1 );
	  *
	  * QImage& frame2 = display->framebuffer();
	  *
	  * CARNA_ASSERT( &frame1 == &frame2 ); // the references do point to the same memory locations
	  * \endcode
      */
    QImage& framebuffer();

    /** \brief  Tells the OpenGL texture object associated to the framebuffer.
      *
      * \post   <code>textureID() > 0</code>
      */
    unsigned int textureID() const;

 // ----------------------------------------------------------------------------------

public slots:

    /** \brief  Schedules repaint for next \ref framebuffer call.
      */
    void invalidate();

    /** \brief  Repaints immediately.
      *
      * \post   <code>isInvalidated() == false</code>
      */
    void paint();

 // ----------------------------------------------------------------------------------

private:

    class Details;
    std::unique_ptr< Details > pimpl;

}; // OffscreenDisplay



}  // namespace Carna :: base

}  // namespace Carna

#endif // OFFSCREENDISPLAY_H_6014714286
