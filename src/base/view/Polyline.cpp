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

#include <Carna/base/view/glew.h>
#include <Carna/base/view/PolylineDetails.h>
#include <Carna/base/Composition.h>
#include <Carna/base/Aggregation.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Polyline :: Details
// ----------------------------------------------------------------------------------

qt::RotatingColor Polyline::Details::nextColor;


Polyline::Details::Details( Polyline& self, Polyline::Type lineType )
    : self( self )
    , looped( lineType == lineLoop )
    , withBorder( true )
    , visible( true )
{
}



// ----------------------------------------------------------------------------------
// Polyline
// ----------------------------------------------------------------------------------

const float Polyline::defaultWidth = Polyline3DVertex::defaultSize / 2;


Polyline::Polyline( model::Scene& model
                  , Type lineType
                  , float width
                  , float nodeSize )
    : model( model )
    , width( width )
    , defaultNodeSize( nodeSize )
    , defaultNodeColor( Details::nextColor++ )
    , pimpl( new Details( *this, lineType ) )
{
}


Polyline::Polyline( model::Scene& model
                  , Type lineType
                  , float width
                  , float nodeSize
                  , const QColor& defaultNodeColor )
    : model( model )
    , width( width )
    , defaultNodeSize( nodeSize )
    , defaultNodeColor( defaultNodeColor )
    , pimpl( new Details( *this, lineType ) )
{
}


Polyline::~Polyline()
{
    clear();
}


void Polyline::appendVertex( const model::Position& position, const QColor& nodeColor, float nodeSize )
{
    Polyline3DVertex* const vertex = new Polyline3DVertex( model, position, nodeColor, nodeSize );

    pimpl->nodes.push_back( new Composition< Polyline3DVertex >( vertex ) );

    vertex->lines.insert( this );

    emit vertexAppended();
}


void Polyline::appendVertex( Polyline3DVertex& vertex )
{
    pimpl->nodes.push_back( new Aggregation< Polyline3DVertex >( vertex ) );

    vertex.lines.insert( this );

    emit vertexAppended();
}


void Polyline::paint( const Renderer& renderer ) const
{
    if( !isVisible() )
    {
        return;
    }

    const static int borderPass = 0;
    const static int fillingPass = 1;

    for( int pass = 0; pass < 2; ++pass )
    {
        switch( pass )
        {

        case borderPass:    // paint border
            {
                if( !hasBorder() )
                {
                    continue;
                }

                glLineWidth( width + 2 );
                glColor4f( 1, 1, 1, 1 );;
                break;
            }

        case fillingPass:   // paint filling
            {
                glLineWidth( width );
                break;
            }

        }

     // emit line geometry

        glBegin( pimpl->looped ? GL_LINE_LOOP : GL_LINE_STRIP );

        for( Details::Nodes::const_iterator it = pimpl->nodes.begin(); it != pimpl->nodes.end(); ++it )
        {
            const Polyline3DVertex& node = *( ( *it )->get() );

            if( pass == fillingPass )
            {
                glColor3f( node.color().redF(), node.color().greenF(), node.color().blueF() );
            }

            glVertex3f( node.position().toVolumeUnits().x()
                      , node.position().toVolumeUnits().y()
                      , node.position().toVolumeUnits().z() );
        }

        glEnd();
    }

 // paint vertices

    for( Details::Nodes::const_iterator it = pimpl->nodes.begin(); it != pimpl->nodes.end(); ++it )
    {
        const Polyline3DVertex& node = *( ( *it )->get() );

        if( node.size() > 0.f )
        {
            node.paintPoint( renderer );
        }
    }
}


void Polyline::removeVertex( const Polyline3DVertex& node )
{
    for( Details::Nodes::iterator it = pimpl->nodes.begin(); it != pimpl->nodes.end(); ++it )
    {
        if( &node == ( *it )->get() )
        {
            if( node.dead )
            {
                ( *it )->forget();
            }

            const Association< Polyline3DVertex >* const association = *it;

            pimpl->nodes.erase( it );

            delete association;

            emit vertexRemoved();

            if( pimpl->nodes.empty() )
            {
                emit allVerticesRemoved();
            }

            return;
        }
    }
}


unsigned int Polyline::getVertexCount() const
{
    return pimpl->nodes.size();
}


Polyline3DVertex& Polyline::getVertex( unsigned int index )
{
    return *pimpl->nodes[ index ]->get();
}


void Polyline::setLooped( bool looped )
{
    pimpl->looped = looped;
}


void Polyline::makeLineStrip()
{
    setLooped( false );
}


void Polyline::makeLineLoop()
{
    setLooped( true );
}


bool Polyline::isLooped() const
{
    return pimpl->looped;
}


void Polyline::setBorder( bool withBorder )
{
    pimpl->withBorder = withBorder;
}
    

bool Polyline::hasBorder() const
{
    return pimpl->withBorder;
}


void Polyline::clear()
{
    /* The nodes are removed from list by the
     * removeVertex callback from their destructor.
     */

    while( !pimpl->nodes.empty() )
    {
        const Association< Polyline3DVertex >* const association = pimpl->nodes[ 0 ];
        pimpl->nodes.erase( pimpl->nodes.begin() );

        delete association;
    }
}


bool Polyline::isVisible() const
{
    return pimpl->visible;
}


void Polyline::setVisible( bool visible )
{
    if( pimpl->visible != visible )
    {
        pimpl->visible = visible;

        if( getVertexCount() > 0 )
        {
            getVertex( 0 ).invalidateObjects3D( model::Object3DEvent( model::Object3DEvent::shape ) );
        }
    }
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
