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

#include <Carna/base/ManagedMesh.h>
#include <Carna/base/Aggregation.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ManagedMeshBase :: Details
// ----------------------------------------------------------------------------------

struct ManagedMeshBase::Details
{
    Details( ManagedMeshBase& self );
    ManagedMeshBase& self;

    std::unique_ptr< VertexBufferBase > vertexBuffer;
    std::unique_ptr<  IndexBufferBase >  indexBuffer;
    
    struct GLContextInfo
    {
        std::unique_ptr< MeshBase > mesh;
        unsigned int acquisitionCount;
        GLContextInfo();
    };
    std::map< const GLContext*, GLContextInfo* > acquisitions;
};


ManagedMeshBase::Details::Details( ManagedMeshBase& self )
    : self( self )
{
}


ManagedMeshBase::Details::GLContextInfo::GLContextInfo()
    : acquisitionCount( 1 )
{
}



// ----------------------------------------------------------------------------------
// ManagedMeshBase
// ----------------------------------------------------------------------------------

ManagedMeshBase::ManagedMeshBase( unsigned int primitiveType, const VertexAttributes& va )
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


ManagedMeshBase::~ManagedMeshBase()
{
}


bool ManagedMeshBase::controlsSameVideoResource( const GeometryFeature& ) const
{
    return false;
}


ManagedMeshInterface* ManagedMeshBase::acquireVideoResource()
{
    return new ManagedMeshInterface( *this );
}


void ManagedMeshBase::acquireMesh()
{
    /* Create vertex and index buffers if necessary.
     */
    if( videoResourceAcquisitionsCount() == 1 )
    {
        CARNA_ASSERT( pimpl->acquisitions.empty() );
        pimpl->vertexBuffer.reset( loadVertexBuffer() );
        pimpl-> indexBuffer.reset( loadIndexBuffer () );
    }
    
    /* Acquire the mesh.
     */
    const GLContext& glc = GLContext::current();
    const auto infoItr = pimpl->acquisitions.find( &glc );
    if( infoItr == pimpl->acquisitions.end() )
    {
        /* Create new mesh within the current OpenGL context.
         */
        Details::GLContextInfo* const info = new Details::GLContextInfo();
        pimpl->acquisitions[ &glc ] = info;
        info->mesh.reset( new MeshBase
            ( vertexAttributes
            , new Aggregation< VertexBufferBase >( *pimpl->vertexBuffer )
            , new Aggregation<  IndexBufferBase >( *pimpl-> indexBuffer ) ) );
    }
    else
    {
        ++infoItr->second->acquisitionCount;
    }
}


void ManagedMeshBase::releaseMesh()
{
    /* Release the mesh.
     */
    const GLContext& glc = GLContext::current();
    const auto infoItr = pimpl->acquisitions.find( &glc );
    CARNA_ASSERT( infoItr != pimpl->acquisitions.end() );
    unsigned int& acquisitionCount = infoItr->second->acquisitionCount;
    CARNA_ASSERT( acquisitionCount > 0 );
    if( --acquisitionCount == 0 )
    {
        /* Delete the mesh within the current OpenGL context.
         */
        pimpl->acquisitions.erase( infoItr );
        delete infoItr->second;
    }
    
    /* Delete vertex and index buffers if necessary.
     */
    if( videoResourceAcquisitionsCount() == 1 )
    {
        CARNA_ASSERT( pimpl->acquisitions.empty() );
        pimpl->vertexBuffer.reset();
        pimpl-> indexBuffer.reset();
    }
}


const MeshBase& ManagedMeshBase::mesh() const
{
    const GLContext& glc = GLContext::current();
    const auto infoItr = pimpl->acquisitions.find( &glc );
    CARNA_ASSERT( infoItr != pimpl->acquisitions.end() );
    return *infoItr->second->mesh;
}



}  // namespace Carna :: base

}  // namespace Carna
