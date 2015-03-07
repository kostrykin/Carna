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

#ifndef FRAMEBUFFER_H_6014714286
#define FRAMEBUFFER_H_6014714286

/** \file   Framebuffer.h
  * \brief  Defines \ref Carna::base::Framebuffer
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <stack>
#include <set>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Framebuffer
// ----------------------------------------------------------------------------------

/** \brief  Encapsulates framebuffer objects.
  *
  * \see    RenderTexture
  * \see    FramebufferBinding
  * \author Leonid Kostrykin
  * \date   9.3.2011
  */
class CARNA_LIB Framebuffer
{

    NON_COPYABLE

public:

    /** \brief
      * Acquires framebuffer object with depth buffer and attaches \a renderTexture
      * as color attachment.
      */
    explicit Framebuffer( RenderTexture& renderTexture );

    /** \brief  Releases associated framebuffer object and depth buffer.
      */
    ~Framebuffer();

    /** \brief  Identifies the associated framebuffer object.
      */
    const unsigned int id;

    static void copy
        ( unsigned int srcId, unsigned int dstId
        , unsigned int srcX0, unsigned int srcY0
        , unsigned int dstX0, unsigned int dstY0
        , unsigned int width, unsigned int height
        , unsigned int flags );

    static void copy( unsigned int srcId, unsigned int dstId, const Viewport& src, const Viewport& dst, unsigned int flags );

    static unsigned int currentId();

    /** \brief  Invalidates the depth buffer's contents and resizes it.
      */
    void resize( unsigned int w, unsigned int h );

    /** \brief  Returns the texture's width.
      */
    inline unsigned int width() const
    {
        return w;
    }

    /** \brief  Returns the texture's height.
      */
    inline unsigned int height() const
    {
        return h;
    }

    /** \brief  Binds and unbinds framebuffer objects.
      *
      * Does not check given FBO for validity.
      * Allows manipulation of the bound framebuffer object.
      * If you want to render to the framebuffer object, refer to Binding.
      *
      * \see    Binding is what you would use in most cases.
      * \see    Framebuffer
      * \author Leonid Kostrykin
      * \date   24.4.2011
      */
    class CARNA_LIB MinimalBinding
    {

        NON_COPYABLE

    public:

        /** \brief  Binds given framebuffer object.
          */
        explicit MinimalBinding( Framebuffer& fbo );

        /** \brief  Releases the binding.
          *
          * Restores the previous binding.
          */
        virtual ~MinimalBinding();

        /** \brief  Attaches the given texture as the currently bound
          *         framebuffer's color buffer.
          *
          * If there was another attached previously, it is replaced.
          */
        void setColorComponent( RenderTexture&, unsigned int position = 0 );

        /** \brief  Reads and returns a color component's current pixel at the given
          *         location.
          */
        Color readPixel( unsigned int x, unsigned int y, unsigned int color_attachment = 0 ) const;

        const Framebuffer& framebuffer() const;

        /** \brief  Re-performs the binding.
          */
        virtual void refresh() const;

    private:

        /** \brief  Binds the associated framebuffer object.
          */
        void bindFBO() const;

    protected:

        /** \brief  References the bound framebuffer object.
          */
        Framebuffer& fbo;

    }; // MinimalBinding

    /** \brief  Same as MinimalBinding, but checks FBO for validity when binding.
      *
      * Allows rendering to the bound framebuffer object in opposite to the
      * MinimalBinding.
      *
      * \see    Framebuffer
      * \author Leonid Kostrykin
      * \date   24.4.2011
      */
    class CARNA_LIB Binding : public MinimalBinding
    {

    public:

        /** \brief  Delegates to MinimalBinding::MinimalBinding.
          *
          * \throws std::runtime_error      if FBO is incomplete or current configuration
          *                                 is unsupported.
          */
        explicit Binding( Framebuffer& );

    protected:

        /** \brief  Re-performs the binding.
          */
        virtual void refresh();

    }; // Binding

private:

    /** \brief  Keeps track of overwritten framebuffer bindings.
      */
    class BindingStack;

    /** \brief  Identifies the associated depth buffer object.
      */
    const unsigned int depthBuffer;

    /** \property w
      * \brief  The depth buffer's width.
      *
      * \property h
      * \brief  The depth buffer's height.
      */
    unsigned int w, h;

    /** \brief  Holds which color buffer slots from \c GL_COLOR_ATTACHMENT0_EXT to
      *         \c GL_COLOR_ATTACHMENT15_EXT have been bound.
      */
    std::set< unsigned int > boundColorBuffers;

}; // Framebuffer



// ----------------------------------------------------------------------------------
// CARNA_RENDER_TO_FRAMEBUFFER_EX
// ----------------------------------------------------------------------------------

#define CARNA_RENDER_TO_FRAMEBUFFER_EX( fbo, binding, instructions ) \
    { \
        Carna::base::Framebuffer::Binding binding( fbo ); \
        instructions; \
    }



// ----------------------------------------------------------------------------------
// CARNA_RENDER_TO_FRAMEBUFFER
// ----------------------------------------------------------------------------------

#define CARNA_RENDER_TO_FRAMEBUFFER( fbo, instructions ) \
    CARNA_RENDER_TO_FRAMEBUFFER_EX( fbo, fboBinding##__COUNTER__, instructions )



}  // namespace Carna :: base

}  // namespace Carna

#endif // FRAMEBUFFER_H_6014714286
