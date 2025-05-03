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

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/noncopyable.h>
#include <LibCarna/base/math.h>
#include <stack>
#include <set>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Framebuffer
// ----------------------------------------------------------------------------------

/** \brief
  * Maintains a framebuffer object that supports up to 8 color components
  * simultaneously.
  *
  * An OpenGL framebuffer object typically consists of a depth attachment and an
  * ordered set of color attachments. The depth attachment can be realized either by
  * a depth buffer (faster, read-only) or by a depth texture. Color attachments are
  * always textures. We call them *render textures* here.
  *
  * \author Leonid Kostrykin
  * \date   9.3.2011 - 25.3.15
  */
class LIBCARNA Framebuffer
{

    NON_COPYABLE
    
    math::Vector2ui size;

public:

    /** \brief
      * Holds maximum number of simultaneously supported color attachments.
      */
    const static unsigned int MAXIMUM_ALLOWED_COLOR_COMPONENTS = 8;

    /** \brief
      * Acquires framebuffer object with depth buffer and attaches \a renderTexture
      * as the first color attachment. The \a renderTexture is resized automatically
      * s.t. it matches the \a size of the framebuffer.
      */
    Framebuffer( unsigned int width, unsigned int height, Texture< 2 >& renderTexture );
    
    /** \brief
      * Creates render texture.
      *
      * \param floatingPoint
      *     sets whether `float`-based pixels shall be used instead of unsigned byte.
      */
    static Texture< 2 >* createRenderTexture( bool floatingPoint = false );

    /** \brief
      * Deletes the maintained framebuffer object and its depth buffer.
      */
    ~Framebuffer();
    
    /** \brief
      * Tells whether a color component exists at \a location.
      */
    bool hasRenderTexture( unsigned int location ) const;
    
    /** \brief
      * References the render texture at \a location.
      * \pre `hasRenderTexture(location) == true`
      */
    const Texture< 2 >& renderTexture( unsigned int location ) const;

    /** \brief
      * Identifies the maintained framebuffer object.
      */
    const unsigned int id;

    /** \brief
      * Copies data from the depth attachment of the framebuffer identified by
      * \a srcId to the depth attachment of the framebuffer identified by \a dstId.
      * Use `0` for one of these arguments to reference the main framebuffer.
      */
    static void copyDepthAttachment( unsigned int srcId, unsigned int dstId, const Viewport& src, const Viewport& dst );

    /** \brief
      * Copies data from the color attachment \a srcColorAttachment of the
      * framebuffer identified by \a srcId to the color attachment
      * \a dstColorAttachment of the framebuffer identified by \a dstId. Use `0` for
      * one of these arguments to reference the main framebuffer.
      */
    static void copyColorAttachment
        ( unsigned int srcId, unsigned int dstId
        , const Viewport& src, const Viewport& dst
        , unsigned int srcColorAttachment, unsigned int dstColorAttachment );

    /** \brief
      * Tells the ID of the currently bound framebuffer. This is `0` for the main
      * framebuffer.
      */
    static unsigned int currentId();

    /** \brief
      * Resizes depth buffer and color components to \a size.
      */
    void resize( const math::Vector2ui& size );

    /** \overload
      */
    void resize( unsigned int width, unsigned int height );

    /** \brief
      * Returns the framebuffer's width.
      */
    inline unsigned int width() const
    {
        return size.x();
    }

    /** \brief
      * Returns the framebuffer's height.
      */
    inline unsigned int height() const
    {
        return size.y();
    }

    // ------------------------------------------------------------------------------
    // Framebuffer :: MinimalBinding
    // ------------------------------------------------------------------------------

    /** \brief
      * Maintains the binding of a \ref Framebuffer in a RAII-manner.
      *
      * Bound framebuffers are not checked for validity. Allows manipulation of the
      * bound framebuffer, but not rendering. Use a full \ref Binding if you want to
      * render to the framebuffer.
      *
      * \author Leonid Kostrykin
      * \date   24.4.2011 - 25.3.15
      */
    class LIBCARNA MinimalBinding
    {

        NON_COPYABLE

    public:

        /** \brief
          * Binds \a fbo as the current framebuffer.
          */
        explicit MinimalBinding( Framebuffer& fbo );

        /** \brief
          * Retracts this binding and restores the previous one.
          */
        virtual ~MinimalBinding();

        /** \brief
          * Attaches \a renderTexture as the color component at \a location of the
          * bound framebuffer object.
          *
          * \pre This is the latest framebuffer binding.
          *
          * If there was another color component bound to \a location previously, it
          * is replaced.
          */
        void setColorComponent( Texture< 2 >& renderTexture, unsigned int location = 0 );
        
        /** \brief
          * Removes color component at \a location from bound framebuffer object.
          *
          * \pre This is the latest framebuffer binding.
          */
        void removeColorComponent( unsigned int location );

        /** \brief
          * Reads the color of the pixel located at \a x and \a y within the color
          * attachment at \a location.
          *
          * \pre This is the latest framebuffer binding.
          */
        Color readPixel( unsigned int x, unsigned int y, unsigned int location = 0 ) const;

        /** \brief
          * References the bound framebuffer.
          */
        const Framebuffer& framebuffer() const;

        /** \brief
          * Re-performs this binding. This is useful if the currently bound
          * framebuffer has been changed through a different mechanism, e.g. using
          * OpenGL calls directly or by another API.
          *
          * \pre This is the latest framebuffer binding.
          */
        virtual void refresh() const;

    private:

        /** \brief
          * Binds the associated framebuffer.
          */
        void bindFBO() const;

    protected:

        /** \brief
          * References the bound framebuffer.
          */
        Framebuffer& fbo;

    }; // Framebuffer :: MinimalBinding

    // ------------------------------------------------------------------------------
    // Framebuffer :: Binding
    // ------------------------------------------------------------------------------

    /** \brief
      * Acts like \ref MinimalBinding, but checks the bound framebuffer for validity
      * and allows rendering.
      *
      * \author Leonid Kostrykin
      * \date   24.4.2011 - 25.3.2015
      */
    class LIBCARNA Binding : public MinimalBinding
    {

    public:

        /** \brief
          * Binds \a fbo as the current framebuffer.
          *
          * \throws AssertionFailure thrown if the framebuffer is incomplete or its
          *     configuration is unsupported.
          */
        explicit Binding( Framebuffer& );

    protected:

        /** \brief  Re-performs the binding.
          */
        virtual void refresh();

    }; // Framebuffer :: Binding

    // ------------------------------------------------------------------------------

private:

    Texture< 2 >* renderTextures[ MAXIMUM_ALLOWED_COLOR_COMPONENTS ];
    const unsigned int depthBuffer;
    std::set< unsigned int > boundColorBuffers;
    class BindingStack;

    static void copy
        ( unsigned int srcId, unsigned int dstId
        , const Viewport& src, const Viewport& dst
        , unsigned int flags
        , unsigned int srcColorAttachment, unsigned int dstColorAttachment );

    static void copy
        ( unsigned int srcId, unsigned int dstId
        , unsigned int srcX0, unsigned int srcY0
        , unsigned int dstX0, unsigned int dstY0
        , unsigned int srcWidth, unsigned int srcHeight
        , unsigned int dstWidth, unsigned int dstHeight
        , unsigned int flags
        , unsigned int srcColorAttachment, unsigned int dstColorAttachment );

}; // Framebuffer



// ----------------------------------------------------------------------------------
// LIBCARNA_RENDER_TO_FRAMEBUFFER_EX
// ----------------------------------------------------------------------------------

/** \brief
  * Binds \a fbo using a binding named \a binding and performs \a instructions before
  * revoking the \a binding.
  */
#define LIBCARNA_RENDER_TO_FRAMEBUFFER_EX( fbo, binding, instructions ) \
    { \
        Carna::base::Framebuffer::Binding binding( fbo ); \
        instructions; \
    }



// ----------------------------------------------------------------------------------
// LIBCARNA_RENDER_TO_FRAMEBUFFER
// ----------------------------------------------------------------------------------

/** \overload
  */
#define LIBCARNA_RENDER_TO_FRAMEBUFFER( fbo, instructions ) \
    LIBCARNA_RENDER_TO_FRAMEBUFFER_EX( fbo, fboBinding##__COUNTER__, instructions )



}  // namespace Carna :: base

}  // namespace Carna

#endif // FRAMEBUFFER_H_6014714286
