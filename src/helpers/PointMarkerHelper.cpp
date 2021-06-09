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

#include <Carna/helpers/PointMarkerHelper.h>
#include <Carna/presets/OpaqueRenderingStage.h>
#include <Carna/base/MeshFactory.h>
#include <Carna/base/Vertex.h>
#include <Carna/base/Geometry.h>
#include <Carna/base/Material.h>
#include <Carna/base/RotatingColor.h>

namespace Carna
{

namespace helpers
{



// ----------------------------------------------------------------------------------
// PointMarkerHelper :: Details
// ----------------------------------------------------------------------------------

struct PointMarkerHelper::Details
{
    Details();

    base::ManagedMeshBase& mesh;
    static base::RotatingColor nextColor;
    
    static unsigned int lastNumber;
    base::Geometry* createGeometry( unsigned int geometryType );

    std::map< base::Color, base::Material* > materials;
    base::Material& nextMaterial( unsigned int pointSize );
};


base::RotatingColor PointMarkerHelper::Details::nextColor;
unsigned int PointMarkerHelper::Details::lastNumber = 0;


PointMarkerHelper::Details::Details()
    : mesh( base::MeshFactory< base::PVertex >::createPoint() )
{
}


base::Material& PointMarkerHelper::Details::nextMaterial( unsigned int pointSize )
{
    const base::Color color = nextColor++;
    const auto materialItr = materials.find( color );
    if( materialItr == materials.end() )
    {
        base::Material& material = base::Material::create( "pointmarker" );
        material.setParameter(     "color", color );
        material.setParameter( "pointSize", static_cast< float >( pointSize ) );
        materials[ color ] = &material;
        return material;
    }
    else
    {
        return *materialItr->second;
    }
}


base::Geometry* PointMarkerHelper::Details::createGeometry( unsigned int geometryType )
{
    std::stringstream tag;
    tag << "Point Marker " << ( ++lastNumber );
    return new base::Geometry( geometryType, tag.str() );
}



// ----------------------------------------------------------------------------------
// PointMarkerHelper
// ----------------------------------------------------------------------------------

PointMarkerHelper::PointMarkerHelper( unsigned int geometryType, unsigned int pointSize )
    : pimpl( new Details() )
    , geometryType( geometryType )
    , meshRole( presets::OpaqueRenderingStage::ROLE_DEFAULT_MESH )
    , materialRole( presets::OpaqueRenderingStage::ROLE_DEFAULT_MATERIAL )
    , pointSize( pointSize )
{
}


PointMarkerHelper::PointMarkerHelper
        ( unsigned int geometryType
        , unsigned int meshRole
        , unsigned int materialRole
        , unsigned int pointSize )
    : pimpl( new Details() )
    , geometryType( geometryType )
    , meshRole( meshRole )
    , materialRole( materialRole )
    , pointSize( pointSize )
{
}


PointMarkerHelper::~PointMarkerHelper()
{
    releaseGeometryFeatures();
}


void PointMarkerHelper::resetDefaultColor()
{
    Details::nextColor.reset();
}


void PointMarkerHelper::releaseGeometryFeatures()
{
    for( auto materialItr = pimpl->materials.begin(); materialItr != pimpl->materials.end(); ++materialItr )
    {
        materialItr->second->release();
    }
    pimpl->materials.clear();
    pimpl->mesh.release();
}


base::Geometry* PointMarkerHelper::createPointMarker() const
{
    base::Geometry* const point = pimpl->createGeometry( geometryType );
    point->putFeature( meshRole, pimpl->mesh );
    point->putFeature( materialRole, pimpl->nextMaterial( pointSize ) );
    return point;
}


base::Geometry* PointMarkerHelper::createPointMarker( const base::Color& color, unsigned int pointSize ) const
{
    base::Material& material = base::Material::create( "pointmarker" );
    material.setParameter(     "color", color );
    material.setParameter( "pointSize", static_cast< float >( pointSize ) );

    base::Geometry* const point = pimpl->createGeometry( geometryType );
    point->putFeature( meshRole, pimpl->mesh );
    point->putFeature( materialRole, material );

    material.release();
    return point;
}


base::Geometry* PointMarkerHelper::createPointMarker( const base::Color& color ) const
{
    return createPointMarker( color, pointSize );
}


base::Geometry* PointMarkerHelper::createPointMarker( unsigned int pointSize ) const
{
    return createPointMarker( pimpl->nextColor++, pointSize );
}



}  // namespace Carna :: helpers

}  // namespace Carna
