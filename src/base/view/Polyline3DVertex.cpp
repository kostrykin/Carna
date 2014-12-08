/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/base/view/Polyline3DVertex.h>
#include <Carna/base/view/Polyline.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Polyline3DVertex
// ----------------------------------------------------------------------------------

const float Polyline3DVertex::defaultSize = 4.f;


Polyline3DVertex::Polyline3DVertex( model::Scene& model
                                  , const model::Position& position
                                  , float size )
    : Point3D( model, position, size )
    , dead( false )
{
}


Polyline3DVertex::Polyline3DVertex( model::Scene& model
                                  , const model::Position& position
                                  , const QColor& color
                                  , float size )
    : Point3D( model, position, color, size )
    , dead( false )
{
}


Polyline3DVertex::~Polyline3DVertex()
{
    dead = true;

    for( PolylineSet::iterator it = lines.begin(); it != lines.end(); ++it )
    {
        Polyline& line = **it;

        line.removeVertex( *this );
    }
}


bool Polyline3DVertex::partOfLine( const Polyline& line ) const
{
    return lines.find( const_cast< Polyline* >( &line ) ) != lines.end();
}


void Polyline3DVertex::paint( const Renderer& renderer ) const
{
    for( PolylineSet::iterator it = lines.begin(); it != lines.end(); ++it )
    {
        Polyline& line = **it;

        if( &line.getVertex( 0 ) == this )
        {
            line.paint( renderer );
        }
    }
}


void Polyline3DVertex::invalidateObjects3D( model::Object3DEvent& ev )
{
    BaseObject3D::invalidateObjects3D( ev );
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
