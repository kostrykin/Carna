/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include <LibCarna/base/glew.h>
#include <LibCarna/base/glError.h>
#include <LibCarna/base/GLContext.h>
#include <LibCarna/base/Framebuffer.h>
#include <LibCarna/base/Viewport.h>
#include <LibCarna/base/LibCarnaException.h>
#include <LibCarna/base/Texture.h>
#include <LibCarna/base/text.h>
#include <stdexcept>
#include <sstream>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// createGlFramebuffer
// ----------------------------------------------------------------------------------

static inline unsigned int createGlFramebuffer()
{
    unsigned int n;
    glGenFramebuffersEXT( 1, &n );
    return n;
}



// ----------------------------------------------------------------------------------
// createGlDepthbufferObject
// ----------------------------------------------------------------------------------

static inline unsigned int createGlDepthbufferObject()
{
    unsigned int n;
    glGenRenderbuffersEXT( 1, &n );
    return n;
}



// ----------------------------------------------------------------------------------
// bindSystemFramebuffer
// ----------------------------------------------------------------------------------

static void bindSystemFramebuffer()
{
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
    if( GLContext::current().isDoubleBuffered )
    {
        glDrawBuffer( GL_BACK );
    }
    REPORT_GL_ERROR;
}



// ----------------------------------------------------------------------------------
// Framebuffer :: BindingStack
// ----------------------------------------------------------------------------------

class Framebuffer::BindingStack
{

    NON_COPYABLE

private:

    typedef std::map< const GLContext*, BindingStack* > InstanceByContext;

    static InstanceByContext instances;

    static InstanceByContext::iterator current();

    std::stack< MinimalBinding* > bindings;

public:

    static void refresh();

    static MinimalBinding& top();

    static void pop();

    static void push( MinimalBinding* );

    static bool empty();

}; // Framebuffer :: BindingStack


Framebuffer::BindingStack::InstanceByContext Framebuffer::BindingStack::instances;


Framebuffer::BindingStack::InstanceByContext::iterator Framebuffer::BindingStack::current()
{
    GLContext& currentContext = GLContext::current();

    const auto it = instances.find( &currentContext );
    if( it != instances.end() )
    {
        return it;
    }
    else
    {
        instances[ &currentContext ] = new BindingStack();

#ifndef _NO_FRAMEBUFFER_DEBUG
        qDebug() << "Framebuffer now manages" << instances.size() << "contexts (+1)";
#endif

        return instances.find( &currentContext );
    }
}


void Framebuffer::BindingStack::refresh()
{
    if( BindingStack::empty() )
    {
        bindSystemFramebuffer();
    }
    else
    {
        BindingStack::top().refresh();
    }
}


Framebuffer::MinimalBinding& Framebuffer::BindingStack::top()
{
    const auto& bindings = current()->second->bindings;

    LIBCARNA_ASSERT( !bindings.empty() );

    return *bindings.top();
}


void Framebuffer::BindingStack::pop()
{
    const auto it = current();
    auto& bindings = it->second->bindings;

    LIBCARNA_ASSERT( !bindings.empty() );

    bindings.pop();

    if( bindings.empty() )
    {
        delete it->second;
        instances.erase( it );

#ifndef _NO_FRAMEBUFFER_DEBUG
        qDebug() << "Framebuffer now manages" << instances.size() << "contexts (-1)";
#endif
    }
}


void Framebuffer::BindingStack::push( MinimalBinding* binding )
{
    const auto it = current();
    auto& bindings = it->second->bindings;

    bindings.push( binding );
}


bool Framebuffer::BindingStack::empty()
{
    const auto& bindings = current()->second->bindings;

    return bindings.empty();
}



// ----------------------------------------------------------------------------------
// Framebuffer
// ----------------------------------------------------------------------------------

Framebuffer::Framebuffer( unsigned int width, unsigned int height, Texture< 2 >& renderTexture )
    : size( math::Vector2ui( width + 1, height + 1 ) )
    , id( createGlFramebuffer() )
    , depthBuffer( createGlDepthbufferObject() )
{
    for( unsigned int i = 0; i < MAXIMUM_ALLOWED_COLOR_COMPONENTS; ++i )
    {
        renderTextures[ i ] = nullptr;
    }

    /* Initialize the depth buffer.
     */
    resize( math::Vector2ui( width, height ) );

    /* Bind the framebuffer.
     */
    MinimalBinding binding( *this );

    /* Bind the depth buffer to the framebuffer object.
     */
    glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT
                                , GL_DEPTH_ATTACHMENT_EXT
                                , GL_RENDERBUFFER_EXT
                                , depthBuffer );

    /* Bind the 'renderTexture' to the framebuffer object and resize it.
     */
    binding.setColorComponent( renderTexture );

    /* Check for errors.
     */
    REPORT_GL_ERROR;
}


Framebuffer::~Framebuffer()
{
    /* Delete the framebuffer object.
     */
    glDeleteFramebuffersEXT( 1, &id );

    /* Delete the depth buffer.
     */
    glDeleteRenderbuffersEXT( 1, &depthBuffer );
}


Texture< 2 >* Framebuffer::createRenderTexture( bool floatingPoint )
{
    const unsigned int internalFormat = floatingPoint ? GL_RGBA16F : GL_RGBA8;
    const unsigned int    pixelFormat = GL_RGBA;
    return new Texture< 2 >( internalFormat, pixelFormat );
}


void Framebuffer::resize( const math::Vector2ui& size )
{
    resize( size.x(), size.y() );
}


void Framebuffer::resize( unsigned int w, unsigned int h )
{
    LIBCARNA_ASSERT_EX( w > 0, "Framebuffer width must be positive!" );
    LIBCARNA_ASSERT_EX( h > 0, "Framebuffer height must be positive!" );

    /* Update the size.
     */
    if( size.x() != w || size.y() != h )
    {
        size.x() = w;
        size.y() = h;

        /* Bind the framebuffer.
         */
        MinimalBinding binding( *this );

        /* Update the depth buffer.
         */
        glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, depthBuffer );
        glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w, h );

        /* Update the color components.
         */
        for( unsigned int i = 0; i < MAXIMUM_ALLOWED_COLOR_COMPONENTS; ++i )
        {
            if( renderTextures[ i ] != nullptr )
            {
                renderTextures[ i ]->update( size );
            }
        }

        /* Check for errors.
         */
        REPORT_GL_ERROR;
    }
}


unsigned int Framebuffer::currentId()
{
    if( BindingStack::empty() )
    {
        return 0;
    }
    else
    {
        return BindingStack::top().framebuffer().id;
    }
}


void Framebuffer::copyColorAttachment
    ( unsigned int srcId, unsigned int dstId
    , const Viewport& src, const Viewport& dst
    , unsigned int srcColorAttachment, unsigned int dstColorAttachment )
{
    copy( srcId, dstId, src, dst, GL_COLOR_BUFFER_BIT, srcColorAttachment, dstColorAttachment );
}


void Framebuffer::copyDepthAttachment( unsigned int srcId, unsigned int dstId, const Viewport& src, const Viewport& dst )
{
    copy( srcId, dstId, src, dst, GL_DEPTH_BUFFER_BIT, 0, 0 );
}


void Framebuffer::copy
    ( unsigned int srcId, unsigned int dstId
    , const Viewport& src, const Viewport& dst
    , unsigned int flags
    , unsigned int srcColorAttachment, unsigned int dstColorAttachment )
{
    const unsigned int  width = std::min( src.width() , dst.width()  );
    const unsigned int height = std::min( src.height(), dst.height() );
    copy( srcId, dstId
        , src.marginLeft(), src.marginTop()
        , dst.marginLeft(), dst.marginTop()
        , src.width(), src.height()
        , dst.width(), dst.height()
        , flags
        , srcColorAttachment, dstColorAttachment );
}


void Framebuffer::copy
    ( unsigned int idFrom, unsigned int idTo
    , unsigned int srcX0, unsigned int srcY0
    , unsigned int dstX0, unsigned int dstY0
    , unsigned int srcWidth, unsigned int srcHeight
    , unsigned int dstWidth, unsigned int dstHeight
    , unsigned int flags
    , unsigned int srcColorAttachment, unsigned int dstColorAttachment )
{
    REPORT_GL_ERROR;

    /* Bind framebuffers.
     */
    glBindFramebufferEXT( GL_READ_FRAMEBUFFER, idFrom );
    glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER, idTo   );
    
    /* Setup color attachments to be copied in case 'flags' wants us to do so.
     */
    if( flags & GL_COLOR_BUFFER_BIT )
    {
        glReadBuffer( GL_COLOR_ATTACHMENT0_EXT + srcColorAttachment );
        glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT + dstColorAttachment );
    }

    /* Do the copying. The lower bounds of the rectangles passed to
     * 'glBlitFramebuffer' are inclusive, while the upper bounds are exclusive.
     */
    glBlitFramebufferEXT
        ( srcX0, srcY0, srcX0 + srcWidth, srcY0 + srcHeight
        , dstX0, dstY0, dstX0 + dstWidth, dstY0 + dstHeight
        , flags
        , GL_NEAREST );

    REPORT_GL_ERROR;

    /* Restore previous bindings.
     */
    BindingStack::refresh();
}



// ----------------------------------------------------------------------------------
// Framebuffer::MinimalBinding
// ----------------------------------------------------------------------------------

Framebuffer::MinimalBinding::MinimalBinding( Framebuffer& fbo )
    : fbo( fbo )
{
    Framebuffer* const currentFBO = BindingStack::empty() ? 0 : &( BindingStack::top().fbo );
    BindingStack::push( this );
    if( currentFBO != &fbo )
    {
        bindFBO();
    }
}


Framebuffer::MinimalBinding::~MinimalBinding()
{
    Framebuffer* const currentFBO = &( BindingStack::top().fbo );
    BindingStack::pop();

    if( BindingStack::empty() )
    {
        bindSystemFramebuffer();
    }
    else
    if( currentFBO != &( BindingStack::top().fbo ) )
    {
        BindingStack::top().refresh();
    }
}


void Framebuffer::MinimalBinding::bindFBO() const
{
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, fbo.id );
    REPORT_GL_ERROR;
}


void Framebuffer::MinimalBinding::setColorComponent( Texture< 2 >& renderTexture, unsigned int location )
{
    LIBCARNA_ASSERT( &BindingStack::top() == this );
    LIBCARNA_ASSERT( location < MAXIMUM_ALLOWED_COLOR_COMPONENTS );

    /* Denote that 'renderTexture' is a color attachment now.
     */
    fbo.renderTextures[ location ] = &renderTexture;
    fbo.boundColorBuffers.insert( location );
    
    /* Update the size of the new color component.
     */
    if( !renderTexture.isValid() || renderTexture.size().x() != fbo.width() || renderTexture.size().y() != fbo.height() )
    {
        renderTexture.update( fbo.size );
    }

    /* Bind the 'renderTexture' to the framebuffer object.
     */
    glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT
                             , GL_COLOR_ATTACHMENT0_EXT + location
                             , GL_TEXTURE_2D
                             , renderTexture.id
                             , 0 );

    /* Check for errors.
     */
    REPORT_GL_ERROR;
}


void Framebuffer::MinimalBinding::removeColorComponent( unsigned int location )
{
    LIBCARNA_ASSERT( &BindingStack::top() == this );
    const auto itr = fbo.boundColorBuffers.find( location );
    if( itr != fbo.boundColorBuffers.end() )
    {
        /* Unbind the texture from the framebuffer object.
         */
        glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT
                                 , GL_COLOR_ATTACHMENT0_EXT + location
                                 , GL_TEXTURE_2D
                                 , 0
                                 , 0 );

        /* Denote that the color component was removed.
         */
        fbo.renderTextures[ location ] = nullptr;
        fbo.boundColorBuffers.erase( itr );

        /* Check for errors.
         */
        REPORT_GL_ERROR;
    }
}


const Framebuffer& Framebuffer::MinimalBinding::framebuffer() const
{
    return fbo;
}


Color Framebuffer::MinimalBinding::readPixel( unsigned int x, unsigned int y, unsigned int location ) const
{
    LIBCARNA_ASSERT( &BindingStack::top() == this );
    LIBCARNA_ASSERT( location < MAXIMUM_ALLOWED_COLOR_COMPONENTS );
    
    unsigned char data[ 4 ];
    glReadBuffer( GL_COLOR_ATTACHMENT0_EXT + location );
    glReadPixels( x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data );
    return Color( data[ 0 ], data[ 1 ], data[ 2 ], data[ 3 ] );
}


void Framebuffer::MinimalBinding::refresh() const
{
    LIBCARNA_ASSERT( &BindingStack::top() == this );
    bindFBO();
}



// ----------------------------------------------------------------------------------
// Framebuffer::Binding
// ----------------------------------------------------------------------------------

Framebuffer::Binding::Binding( Framebuffer& fbo )
    : MinimalBinding( fbo )
{
    Binding::refresh();
}


void Framebuffer::Binding::refresh()
{
    MinimalBinding::refresh();

    /* Validate the framebuffer.
     */
    const GLenum error = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
    switch( error )
    {

    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
        LIBCARNA_FAIL( "Framebuffer configuration not supported!" );

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
        LIBCARNA_FAIL( "Framebuffer misses color attachment!" );

    case GL_FRAMEBUFFER_COMPLETE_EXT:
        break;

    default:
        LIBCARNA_FAIL( "Unknown Framebuffer error! Error code: " + text::lexical_cast< std::string >( error ) );

    }

    /* Setup for rendering.
     */
    if( !fbo.boundColorBuffers.empty() )
    {
        std::vector< GLenum > buffers( fbo.boundColorBuffers.size() );
        unsigned int index = 0;
        for( std::set< unsigned int >::const_iterator it  = fbo.boundColorBuffers.begin();
                                                      it != fbo.boundColorBuffers.end();
                                                    ++it, ++index )
        {
            LIBCARNA_ASSERT_EX( index < buffers.size(), "Color attachments are not bound continuously!" );
            buffers[ index ] = GL_COLOR_ATTACHMENT0_EXT + index;
        }
        glDrawBuffers( buffers.size(), &buffers[ 0 ] );
    }

    /* Check for errors.
     */
    REPORT_GL_ERROR;
}



}  // namespace LibCarna :: base

}  // namespace LibCarna
