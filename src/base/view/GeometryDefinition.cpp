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

#include <Carna/base/view/GeometryDefinition.h>
#include <Carna/base/view/VideoResourcesManager.h>
#include <Carna/base/Log.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// GeometryDefinition
// ----------------------------------------------------------------------------------

GeometryDefinition::GeometryDefinition( VideoResourcesManager* vrm )
    : videoResourcesAcquisitions( 0 )
    , vrm( vrm )
    , released( false )
{
    CARNA_ASSERT( vrm != nullptr );
}


GeometryDefinition& GeometryDefinition::create( VideoResourcesManager* vrm )
{
    return *new GeometryDefinition( vrm );
}


GeometryDefinition::~GeometryDefinition()
{
    if( videoResourcesAcquisitions != 0 )
    {
        Log::instance().record( Log::error, "GeometryDefinition deleted while video resources still acquired!" );
        vrm->deleteResources();
    }
}


void GeometryDefinition::acquireVideoResources()
{
    if( ++videoResourcesAcquisitions == 1 )
    {
        vrm->uploadResources();
    }
}


void GeometryDefinition::releaseVideoResources()
{
    CARNA_ASSERT( videoResourcesAcquisitions > 0 );
    if( --videoResourcesAcquisitions == 0 )
    {
        vrm->deleteResources();
        if( released )
        {
            deleteIfAllowed();
        }
    }
}


const VideoResourcesManager& GeometryDefinition::videoResources() const
{
    return *vrm;
}


bool GeometryDefinition::deleteIfAllowed()
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


void GeometryDefinition::release()
{
    CARNA_ASSERT( !released );
    if( !deleteIfAllowed() )
    {
        released = true;
    }
}


void GeometryDefinition::applyTo( Geometry& sceneGraphNode )
{
    referencingSceneGraphNodes.insert( &sceneGraphNode );
}


void GeometryDefinition::removeFrom( Geometry& sceneGraphNode )
{
    referencingSceneGraphNodes.erase( &sceneGraphNode );
    if( released )
    {
        deleteIfAllowed();
    }
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
