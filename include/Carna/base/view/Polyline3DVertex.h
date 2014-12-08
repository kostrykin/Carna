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

#ifndef POLYLINE3DVERTEX_H_6014714286
#define POLYLINE3DVERTEX_H_6014714286

/** \file   Polyline3DVertex.h
  * \brief  Defines \ref Carna::base::view::Polyline3DVertex
  */

#include <Carna/base/view/Point3D.h>
#include <set>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Polyline3DVertex
// ----------------------------------------------------------------------------------

/** \brief  Represents a \ref Polyline "polyline" vertex in 3D space.
  *
  * \author Leonid Kostrykin
  * \date   30.11.11 - 6.3.12
  */
class CARNA_EXPORT Polyline3DVertex : public Point3D
{

public:

    /** \brief  Determines the size to use when size argument is not specified.
      */
    static const float defaultSize;


    /** \brief  Creates a new vertex.
      *
      * \param  model       references the data model.
      * \param  position    tells whether the vertex is to be placed.
      * \param  size        specifies the size of this vertex.
      */
    Polyline3DVertex( model::Scene& model
                    , const model::Position& position
                    , float size = Polyline3DVertex::defaultSize );

    /** \brief  Creates a new vertex.
      *
      * \param  model       references the data model.
      * \param  position    tells whether the vertex is to be placed.
      * \param  color       specifies the color of this vertex.
      * \param  size        specifies the size of this vertex.
      */
    Polyline3DVertex( model::Scene& model
                    , const model::Position& position
                    , const QColor& color
                    , float size = Polyline3DVertex::defaultSize );

    /** \brief  Removes the first from the lines it is associated with.
      */
    virtual ~Polyline3DVertex();


    /** \brief  Tells whether this vertex is part of the given line.
      */
    bool partOfLine( const Polyline& ) const;


    virtual void paint( const Renderer& ) const override;


protected:

    friend class Polyline;  // TODO: check whether this is still needed

    /** \brief  Redeclaration of \ref model::Scene::BaseObject3D::invalidateObjects3D so that it can be accessed by \ref Polyline.
      */
    void invalidateObjects3D( model::Object3DEvent& ev );


private: // TODO: implement PIMPL

    friend class Polyline;


    typedef std::set< Polyline* > PolylineSet;

    mutable PolylineSet lines;


    bool dead;

    
    void paintPoint( const Renderer& renderer ) const
    {
        Point3D::paint( renderer );
    }

}; // Polyline3DVertex



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // POLYLINE3DVERTEX_H_6014714286
