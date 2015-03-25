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
#include <Carna/base/math.h>
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
  * \author Leonid Kostrykin
  * \date   9.3.2011 - 25.3.15
  */
class CARNA_LIB Framebuffer
{

    NON_COPYABLE
    
    math::Vector2ui size;

public:

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

    /** \brief  Releases associated framebuffer object and depth buffer.
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

    static void copy
        ( unsigned int srcId, unsigned int dstId
        , unsigned int srcX0, unsigned int srcY0
        , unsigned int dstX0, unsigned int dstY0
        , unsigned int srcWidth, unsigned int srcHeight
        , unsigned int dstWidth, unsigned int dstHeight
        , unsigned int flags );

    static void copy( unsigned int srcId, unsigned int dstId, const Viewport& src, const Viewport& dst, unsigned int flags );

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

        /** \brief
          * Attaches \a renderTexture as the color component at \a location of the
          * bound framebuffer object.
          *
          * If there was another color component bound to \a location previously, it
          * is replaced.
          */
        void setColorComponent( Texture< 2 >& renderTexture, unsigned int location = 0 );
        
        /** \brief
          * Removes color component at \a location from bound framebuffer object.
          */
        void removeColorComponent( unsigned int location );

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

    Texture< 2 >* renderTextures[ MAXIMUM_ALLOWED_COLOR_COMPONENTS ];
    const unsigned int depthBuffer;
    std::set< unsigned int > boundColorBuffers;
    class BindingStack;

}; // Framebuffer



// ----------------------------------------------------------------------------------
// CARNA_RENDER_TO_FRAMEBUFFER_EX
// ----------------------------------------------------------------------------------

/** \brief
  * Binds \a fbo using a binding named \a binding and performs \a instructions before
  * revoking the \a binding.
  */
#define CARNA_RENDER_TO_FRAMEBUFFER_EX( fbo, binding, instructions ) \
    { \
        Carna::base::Framebuffer::Binding binding( fbo ); \
        instructions; \
    }



// ----------------------------------------------------------------------------------
// CARNA_RENDER_TO_FRAMEBUFFER
// ----------------------------------------------------------------------------------

/** \overload
  */
#define CARNA_RENDER_TO_FRAMEBUFFER( fbo, instructions ) \
    CARNA_RENDER_TO_FRAMEBUFFER_EX( fbo, fboBinding##__COUNTER__, instructions )



}  // namespace Carna :: base

}  // namespace Carna

#endif // FRAMEBUFFER_H_6014714286
