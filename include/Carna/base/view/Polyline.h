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

#ifndef POLYLINE_H_6014714286
#define POLYLINE_H_6014714286

/** \file   Polyline.h
  * \brief  Defines \ref Carna::base::view::Polyline
  */

#include <Carna/base/view/Polyline3DVertex.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Polyline
// ----------------------------------------------------------------------------------

/** \brief  Defines a sequence of multiple continuous straight line segments.
  *
  * \section PolylineGettingStarted Getting Started
  *
  * The following example would create a square:
  *
  * \code
  * using namespace Carna::base::view;
  * using namespace Carna::base::model;
  *
  * static Polyline* doPolylineExample1( Scene& model )
  * {
  *     Polyline* const line = new Polyline( model, Polyline::lineLoop );
  *
  *     ( *line ) << Position::fromVolumeUnits( model, 0, 0, 0 )
  *               << Position::fromVolumeUnits( model, 1, 0, 0 )
  *               << Position::fromVolumeUnits( model, 1, 1, 0 )
  *               << Position::fromVolumeUnits( model, 0, 1, 0 );
  *
  *     return line;
  * }
  * \endcode
  *
  * The following example creates a cross with a vertex, shared between multiple lines:
  *
  * \code
  * using namespace Carna::base::view;
  * using namespace Carna::base::model;
  *
  * static Polyline* doPolylineExample2( Scene& model )
  * {
  *     Polyline* const line = new Polyline( model, Polyline::lineLoop );
  *
  *     Polyline3DVertex& sharedVertex = *new Carna::Polyline3DVertex( model,
  *             Position::fromVolumeUnits( context.model, 0.5f, 0.5f, 0.5f ) );
  *
  *     ( *line ) << Position::fromVolumeUnits( model, 0.5f, 0.5f, 0 );
  *     line->appendVertex( sharedVertex );
  *     ( *line ) << Position::fromVolumeUnits( model, 0.5f, 0.5f, 1 );
  *
  *     ( *line ) << Position::fromVolumeUnits( model, 0, 0.5f, 0.5f );
  *     line->appendVertex( sharedVertex );
  *     ( *line ) << Position::fromVolumeUnits( model, 1, 0.5f, 0.5f );
  *
  *     return line;
  * }
  * \endcode
  *
  * You must destroy the acquired \c %Polyline instances at a later time to avoid memory leaks.
  * An easy way of ensuring that the polyline is destroyed safely,
  * is tying it's lifetime to those of it's vertices:
  *
  * \code
  * Carna::Polyline* const line = createLine();
  * QObject::connect( line, SIGNAL( allVerticesRemoved() ), line, SLOT( deleteLater() ) );
  * \endcode
  *
  * \note
  * Usually the polyline is painted with a thin white border - you can turn it off using \ref setBorder.
  *
  * \author Leonid Kostrykin
  * \date   1.12.11 - 19.2.13
  */
class CARNA_EXPORT Polyline : public QObject
{

    NON_COPYABLE

    Q_OBJECT

public:

    /** \brief  Holds the default line width used when no particular width is requested.
      */
    const static float defaultWidth;


    /** \brief	Describes the type of a polyline.
      */
    enum Type
    {
        /** \brief	Line segment is painted from last vertex to first.
          */
        lineLoop,
        
        /** \brief	Usual behavior.
          */
        lineStrip
    };


    /** \brief  Instantiates with default color.
      *
      * \param  model               references the data model.
      * \param  lineType            sets whether the line is looped or not.
      * \param  width               sets the line width.
      * \param  defaultNodeSize     sets the default vertex size.
      */
    Polyline( model::Scene& model
            , Type lineType
            , float width = defaultWidth
            , float defaultNodeSize = Polyline3DVertex::defaultSize );

    /** \brief  Instantiates.
      *
      * \param  model               references the data model.
      * \param  lineType            sets whether the line is looped or not.
      * \param  width               sets the line width.
      * \param  defaultNodeSize     sets the default vertex size.
      * \param  defaultNodeColor    sets the default vertex color.
      */
    Polyline( model::Scene& model
            , Type lineType
            , float width
            , float defaultNodeSize
            , const QColor& defaultNodeColor );

    /** \brief	Releases all objects belonging only to this line.
      */
    ~Polyline();


    /** \brief	Holds the default vertex size.
      */
    const float defaultNodeSize;

    /** \brief	Holds the default vertex color.
      */
    const QColor defaultNodeColor;


    /** \brief  Appends a new vertex, using \ref defaultNodeSize and \ref defaultNodeColor.
      */
    void appendVertex( const model::Position& position )
    {
        appendVertex( position, defaultNodeColor, defaultNodeSize );
    }

    /** \brief  Appends a new vertex, using \ref defaultNodeSize.
      */
    void appendVertex( const model::Position& position, const QColor& nodeColor )
    {
        appendVertex( position, nodeColor, defaultNodeSize );
    }

    /** \brief  Appends a new vertex.
      */
    void appendVertex( const model::Position& position
                     , const QColor& nodeColor
                     , float nodeSize );


    /** \brief  Appends an existing vertex.
      */
    void appendVertex( Polyline3DVertex& );


    /** \brief	Removes the given vertex from that line.
      */
    void removeVertex( const Polyline3DVertex& );


    /** \brief  Tells the number of vertices belonging to this line.
      */
    unsigned int getVertexCount() const;

    /** \brief  References a certain vertex.
      */
    Polyline3DVertex& getVertex( unsigned int index );

    /** \brief  References a certain vertex.
      */
    const Polyline3DVertex& getVertex( unsigned int index ) const
    {
        return const_cast< Polyline* >( this )->getVertex( index );
    }


    /** \brief  References the data model.
      */
    model::Scene& model;

    /** \brief  Holds the line width.
      */
    const float width;


    /** \brief  Draws this polyline.
      */
    void paint( const Renderer& ) const;


    /** \brief	Sets whether this polyline is looped.
      */
    void setLooped( bool looped );

    /** \brief	Makes this polyline non-looped.
      */
    void makeLineStrip();

    /** \brief	Makes this polyline looped.
      */
    void makeLineLoop();

    /** \brief	Tells whether this polyline is looped.
      */
    bool isLooped() const;

    /** \brief  Sets whether this line has a white border.
      */
    void setBorder( bool withBorder );
    
    /** \brief  Tells whether this line has a white border.
      */
    bool hasBorder() const;

    /** \brief  Tells whether this line is visible.
      *
      * \since  \ref v_2_2_2
      */
    bool isVisible() const;
    
    /** \brief  Sets whether this line is visible.
      *
      * \since  \ref v_2_2_2
      */
    void setVisible( bool );


    /** \brief	Removes all vertices from this polyline.
      */
    void clear();


signals:

    /** \brief  Emitted when some vertex has been added to this polyline.
      *
      * Emitted by \ref appendVertex.
      */
    void vertexAppended();
    
    /** \brief  Emitted when a vertex has been removed from this polyline.
      *
      * Emitted by \ref removeVertex.
      */
    void vertexRemoved();
    
    /** \brief  Emitted when a vertex has been removed from this polyline and no more vertices are left.
      *
      * Emitted by \ref removeVertex.
      *
      * The \ref vertexRemoved signal is always emitted before.
      */
    void allVerticesRemoved();


private:

    class Details;
    std::unique_ptr< Details > pimpl;

}; // Polyline


/** \brief  Appends a new vertex to some Polyline, taking the line's default settings.
  *
  * \relates    Polyline
  */
inline Polyline& operator<<( Polyline& polyline, const model::Position& position )
{
    polyline.appendVertex( position );
    return polyline;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // POLYLINE_H_6014714286
