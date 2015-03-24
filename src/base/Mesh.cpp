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

#include <Carna/base/glew.h>
#include <Carna/base/Mesh.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// computeVertexStride
// ----------------------------------------------------------------------------------

static std::size_t computeVertexStride( const VertexAttributes& vertexAttributes )
{
    std::size_t componentsCount = 0;
    for( auto itr = vertexAttributes.begin(); itr != vertexAttributes.end(); ++itr )
    {
        componentsCount += itr->componentsCount;
    }
    return componentsCount * sizeof( float );
}


// ----------------------------------------------------------------------------------
// MeshBase :: Details
// ----------------------------------------------------------------------------------

struct MeshBase::Details
{
    Details( MeshBase& self );
    MeshBase& self;

    std::unique_ptr< VertexBufferBase > vertexBuffer;
    std::unique_ptr< IndexBufferBase > indexBuffer;
    
    struct GLContextInfo
    {
        unsigned int id;
        unsigned int acquisitionCount;
        GLContextInfo();
    };
    std::map< const GLContext*, GLContextInfo* > acquisitions;
    
    unsigned int acquire();
    void release();
};


MeshBase::Details::Details( MeshBase& self )
    : self( self )
{
}


MeshBase::Details::GLContextInfo::GLContextInfo()
    : id( 0 )
    , acquisitionCount( 1 )
{
}


unsigned int MeshBase::Details::acquire()
{
    const GLContext& glc = GLContext::current();
    const auto infoItr = acquisitions.find( &glc );
    if( infoItr == acquisitions.end() )
    {
        /* Create new vertex array.
         */
        GLContextInfo* const info = new GLContextInfo();
        acquisitions[ &glc ] = info;
        glGenVertexArrays( 1, &info->id );
        
        /* Configure vertex array.
         */
        glBindVertexArray( info->id );
        vertexBuffer->bind();
        std::size_t offset = 0;
        const std::size_t stride = computeVertexStride( self.vertexAttributes );
        for( auto itr = self.vertexAttributes.begin(); itr != self.vertexAttributes.end(); ++itr )
        {
            glEnableVertexAttribArray( itr->position );
            glVertexAttribPointer
                ( itr->position
                , itr->componentsCount
                , itr->type
                , false
                , stride
                , static_cast< float* >( nullptr ) + offset );
            offset += itr->componentsCount;
        }
        glBindVertexArray( 0 );
        return info->id;
    }
    else
    {
        ++infoItr->second->acquisitionCount;
        return infoItr->second->id;
    }
}


void MeshBase::Details::release()
{
    const GLContext& glc = GLContext::current();
    const auto infoItr = acquisitions.find( &glc );
    CARNA_ASSERT( infoItr != acquisitions.end() );
    unsigned int& acquisitionCount = infoItr->second->acquisitionCount;
    CARNA_ASSERT( acquisitionCount > 0 );
    if( --acquisitionCount == 0 )
    {
        /* Delete vertex array.
         */
        glDeleteVertexArrays( 1, &infoItr->second->id );
        acquisitions.erase( infoItr );
    }
}



// ----------------------------------------------------------------------------------
// MeshBase :: VideoResourceAcquisition
// ----------------------------------------------------------------------------------

MeshBase::ManagedInterface::ManagedInterface( MeshBase& mesh )
    : GeometryFeature::ManagedInterface( mesh )
    , mesh( mesh )
{
    if( mesh.videoResourceAcquisitionsCount() == 1 )
    {
        CARNA_ASSERT( mesh.pimpl->acquisitions.empty() );
        
        /* Upload the vertices and indices to video memory.
         */
        mesh.pimpl->vertexBuffer.reset( mesh.loadVertexBuffer() );
        mesh.pimpl-> indexBuffer.reset( mesh.loadIndexBuffer () );
    }
    
    /* Acquire vertex array within current context.
     */
    myId = mesh.pimpl->acquire();
}


MeshBase::ManagedInterface::~ManagedInterface()
{
    /* Release vertex array within current context.
     */
    mesh.pimpl->release();
    
    if( mesh.videoResourceAcquisitionsCount() == 1 )
    {
        CARNA_ASSERT( mesh.pimpl->acquisitions.empty() );
        
        /* Delete buffers from video memory.
         */
        mesh.pimpl->vertexBuffer.reset();
        mesh.pimpl-> indexBuffer.reset();
    }
}


void MeshBase::ManagedInterface::bind() const
{
    glBindVertexArray( this->id() );
}


void MeshBase::ManagedInterface::render() const
{
    CARNA_ASSERT_EX( vertexBuffer().isValid(), "Vertex buffer is invalid." );
    CARNA_ASSERT_EX(  indexBuffer().isValid(),  "Index buffer is invalid." );

    this->bind();
    indexBuffer().bind();
    glDrawElements( indexBuffer().primitiveType, indexBuffer().size(), indexBuffer().type, nullptr );
}


const VertexBufferBase& MeshBase::ManagedInterface::vertexBuffer() const
{
    return *mesh.pimpl->vertexBuffer;
}


const IndexBufferBase& MeshBase::ManagedInterface::indexBuffer() const
{
    return *mesh.pimpl->indexBuffer;
}


VertexBufferBase& MeshBase::ManagedInterface::vertexBuffer()
{
    return *mesh.pimpl->vertexBuffer;
}


IndexBufferBase& MeshBase::ManagedInterface::indexBuffer()
{
    return *mesh.pimpl->indexBuffer;
}


unsigned int MeshBase::ManagedInterface::id() const
{
    return myId;
}



// ----------------------------------------------------------------------------------
// MeshBase
// ----------------------------------------------------------------------------------

MeshBase::MeshBase( unsigned int primitiveType, const VertexAttributes& va )
#pragma warning( push )
#pragma warning( disable:4355 )
    /* It is okay to use 'this' in class initialization list, as long as it is not
     * used to access any members that may not have been initialized yet.
     */
    : pimpl( new Details( *this ) )
#pragma warning( pop )
    , primitiveType( primitiveType )
    , vertexAttributes( va )
{
}


MeshBase::~MeshBase()
{
}


bool MeshBase::controlsSameVideoResource( const GeometryFeature& ) const
{
    return false;
}


MeshBase::ManagedInterface* MeshBase::acquireVideoResource()
{
    return new ManagedInterface( *this );
}



}  // namespace Carna :: base

}  // namespace Carna
