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

#include <Carna/base/GeometryFeature.h>
#include <Carna/base/Log.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// GeometryFeature :: VideoResourceAcquisition
// ----------------------------------------------------------------------------------

GeometryFeature::VideoResourceAcquisition::VideoResourceAcquisition( GLContext& glc, GeometryFeature& gf )
    : geometryFeature( gf )
{
    ++geometryFeature.myVideoResourceAcquisitions;
    glc.makeActive();
}


GeometryFeature::VideoResourceAcquisition::~VideoResourceAcquisition()
{
    CARNA_ASSERT( geometryFeature.myVideoResourceAcquisitions > 0 );
    if( --geometryFeature.myVideoResourceAcquisitions == 0 )
    {
        if( geometryFeature.released )
        {
            geometryFeature.deleteIfAllowed();
        }
    }
}



// ----------------------------------------------------------------------------------
// GeometryFeature
// ----------------------------------------------------------------------------------

GeometryFeature::GeometryFeature()
    : myVideoResourceAcquisitions( 0 )
    , released( false )
{
}


GeometryFeature::~GeometryFeature()
{
    if( myVideoResourceAcquisitions != 0 )
    {
        Log::instance().record( Log::error, "GeometryFeature deleted while video resources still acquired!" );
    }
}


unsigned int GeometryFeature::videoResourceAcquisitionsCount() const
{
    return myVideoResourceAcquisitions;
}


bool GeometryFeature::deleteIfAllowed()
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


void GeometryFeature::release()
{
    CARNA_ASSERT( !released );
    if( !deleteIfAllowed() )
    {
        released = true;
    }
}


void GeometryFeature::addTo( Geometry& sceneGraphNode, unsigned int role )
{
    const std::size_t size0 = referencingSceneGraphNodes.size();
    referencingSceneGraphNodes.insert( &sceneGraphNode );
    if( size0 != referencingSceneGraphNodes.size() )
    {
        sceneGraphNode.putFeature( role, *this );
    }
}


void GeometryFeature::removeFrom( Geometry& sceneGraphNode )
{
    const std::size_t size0 = referencingSceneGraphNodes.size();
    referencingSceneGraphNodes.erase( &sceneGraphNode );
    if( size0 != referencingSceneGraphNodes.size() )
    {
        sceneGraphNode.removeFeature( *this );
    }
    if( released )
    {
        deleteIfAllowed();
    }
}



}  // namespace Carna :: base

}  // namespace Carna
