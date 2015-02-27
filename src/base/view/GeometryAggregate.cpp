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
#include <Carna/base/view/VideoResourcesControl.h>
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

GeometryAggregate::GeometryAggregate( VideoResourcesControl* vrc )
    : videoResourcesAcquisitions( 0 )
    , vrc( vrc )
    , released( false )
{
    CARNA_ASSERT( vrc != nullptr );
}


GeometryAggregate& GeometryAggregate::create( VideoResourcesControl* vrc )
{
    return *new GeometryAggregate( vrc );
}


GeometryAggregate::~GeometryAggregate()
{
    if( videoResourcesAcquisitions != 0 )
    {
        Log::instance().record( Log::error, "GeometryAggregate deleted while video resources still acquired!" );
        vrc->deleteResources();
    }
}


void GeometryAggregate::acquireVideoResources()
{
    if( ++videoResourcesAcquisitions == 1 )
    {
        vrc->uploadResources();
    }
}


void GeometryAggregate::releaseVideoResources()
{
    CARNA_ASSERT( videoResourcesAcquisitions > 0 );
    if( --videoResourcesAcquisitions == 0 )
    {
        vrc->deleteResources();
        if( released )
        {
            deleteIfAllowed();
        }
    }
}


const VideoResourcesControl& GeometryAggregate::videoResources() const
{
    return *vrc;
}


bool GeometryAggregate::deleteIfAllowed()
{
    if( videoResourcesAcquisitions == 0 && referencingSceneGraphNodes.empty() )
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
