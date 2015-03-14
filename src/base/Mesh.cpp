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
    
    unsigned int acquire( const GLContext& glc );
    void release( const GLContext& glc );
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


unsigned int MeshBase::Details::acquire( const GLContext& glc )
{
    const auto infoItr = acquisitions.find( &glc );
    if( infoItr == acquisitions.end() )
    {
        /* Create new vertex array.
         */
        CARNA_ASSERT( glc.isCurrent() );
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


void MeshBase::Details::release( const GLContext& glc )
{
    const auto infoItr = acquisitions.find( &glc );
    CARNA_ASSERT( infoItr != acquisitions.end() );
    unsigned int& acquisitionCount = infoItr->second->acquisitionCount;
    CARNA_ASSERT( acquisitionCount > 0 );
    if( --acquisitionCount == 0 )
    {
        /* Delete vertex array.
         */
        CARNA_ASSERT( glc.isCurrent() );
        glDeleteVertexArrays( 1, &infoItr->second->id );
        acquisitions.erase( infoItr );
    }
}



// ----------------------------------------------------------------------------------
// MeshBase :: VideoResourceAcquisition
// ----------------------------------------------------------------------------------

MeshBase::VideoResourceAcquisition::VideoResourceAcquisition
        ( GLContext& glc
        , MeshBase& mesh )
    : GeometryFeature::VideoResourceAcquisition( glc, mesh )
    , glContext( glc )
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
    myId = mesh.pimpl->acquire( glContext );
}


MeshBase::VideoResourceAcquisition::~VideoResourceAcquisition()
{
    /* Release vertex array within current context.
     */
    mesh.pimpl->release( glContext );
    
    if( mesh.videoResourceAcquisitionsCount() == 1 )
    {
        CARNA_ASSERT( mesh.pimpl->acquisitions.empty() );
        
        /* Delete buffers from video memory.
         */
        mesh.pimpl->vertexBuffer.reset();
        mesh.pimpl-> indexBuffer.reset();
    }
}


void MeshBase::VideoResourceAcquisition::bind() const
{
    glBindVertexArray( this->id() );
}


void MeshBase::VideoResourceAcquisition::render() const
{
    CARNA_ASSERT_EX( vertexBuffer().isValid(), "Vertex buffer is invalid." );
    CARNA_ASSERT_EX(  indexBuffer().isValid(),  "Index buffer is invalid." );

    this->bind();
    indexBuffer().bind();
    glDrawElements( indexBuffer().primitiveType, indexBuffer().size(), indexBuffer().type, nullptr );
}


const VertexBufferBase& MeshBase::VideoResourceAcquisition::vertexBuffer() const
{
    return *mesh.pimpl->vertexBuffer;
}


const IndexBufferBase& MeshBase::VideoResourceAcquisition::indexBuffer() const
{
    return *mesh.pimpl->indexBuffer;
}


VertexBufferBase& MeshBase::VideoResourceAcquisition::vertexBuffer()
{
    return *mesh.pimpl->vertexBuffer;
}


IndexBufferBase& MeshBase::VideoResourceAcquisition::indexBuffer()
{
    return *mesh.pimpl->indexBuffer;
}


unsigned int MeshBase::VideoResourceAcquisition::id() const
{
    return myId;
}



// ----------------------------------------------------------------------------------
// MeshBase
// ----------------------------------------------------------------------------------

MeshBase::MeshBase( unsigned int primitiveType, const VertexAttributes& va )
    : pimpl( new Details( *this ) )
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


MeshBase::VideoResourceAcquisition* MeshBase::acquireVideoResource( GLContext& glc )
{
    return new VideoResourceAcquisition( glc, *this );
}



}  // namespace Carna :: base

}  // namespace Carna
