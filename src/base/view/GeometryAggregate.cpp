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

#include <Carna/base/view/GeometryAggregate.h>
#include <Carna/base/view/VideoResourceManager.h>
#include <Carna/base/Log.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// GeometryAggregate
// ----------------------------------------------------------------------------------

GeometryAggregate::GeometryAggregate()
    : myVideoResourceAcquisitions( 0 )
    , released( false )
{
}


GeometryAggregate::~GeometryAggregate()
{
    if( myVideoResourceAcquisitions != 0 )
    {
        Log::instance().record( Log::error, "GeometryAggregate deleted while video resources still acquired!" );
    }
}


unsigned int GeometryAggregate::videoResourceAcquisitionsCount() const
{
    return myVideoResourceAcquisitions;
}


void GeometryAggregate::acquireVideoResource()
{
    ++myVideoResourceAcquisitions;
}


void GeometryAggregate::releaseVideoResource()
{
    CARNA_ASSERT( myVideoResourceAcquisitions > 0 );
    if( --myVideoResourceAcquisitions == 0 )
    {
        if( released )
        {
            deleteIfAllowed();
        }
    }
}


bool GeometryAggregate::deleteIfAllowed()
{
    if( myVideoResourceAcquisitions == 0 && referencingSceneGraphNodes.empty() )
    {
        delete this;
        return true;
    }
    else
    {
        return false;
    }
}


void GeometryAggregate::release()
{
    CARNA_ASSERT( !released );
    if( !deleteIfAllowed() )
    {
        released = true;
    }
}


void GeometryAggregate::addTo( Geometry& sceneGraphNode, unsigned int role )
{
    const std::size_t size0 = referencingSceneGraphNodes.size();
    referencingSceneGraphNodes.insert( &sceneGraphNode );
    if( size0 != referencingSceneGraphNodes.size() )
    {
        sceneGraphNode.putAggregate( *this, role );
    }
}


void GeometryAggregate::removeFrom( Geometry& sceneGraphNode )
{
    const std::size_t size0 = referencingSceneGraphNodes.size();
    referencingSceneGraphNodes.erase( &sceneGraphNode );
    if( size0 != referencingSceneGraphNodes.size() )
    {
        sceneGraphNode.removeAggregate( *this );
    }
    if( released )
    {
        deleteIfAllowed();
    }
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
