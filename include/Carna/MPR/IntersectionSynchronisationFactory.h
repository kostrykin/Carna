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

#ifndef AXIALPLANE_INTERSECTIONSYNCHRONISATIONFACTORY_H_6014714286
#define AXIALPLANE_INTERSECTIONSYNCHRONISATIONFACTORY_H_6014714286

/** \file   IntersectionSynchronisationFactory.h
  * \brief  Defines \ref Carna::MPR::IntersectionSynchronisationFactory.
  */

#include <Carna/MPR/AxialPlaneRenderer.h>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: IntersectionSynchronisationFactory
// ----------------------------------------------------------------------------------

/** \brief  Makes a set of \ref AxialPlaneRenderer "renderers" use the same intersection.
  *
  * Refer to \ref MPRExample for an example on how to use this class.
  *
  * \author Leonid Kostrykin
  * \date   9.11.11 - 19.11.13
  */
class IntersectionSynchronisationFactory
{

public:

    /** \brief  Appends a renderer.
      */
    void addRenderer( AxialPlaneRenderer& renderer )
    {
        Renderers::iterator it = std::find( renderers.begin(), renderers.begin() + renderers.size(), &renderer );
        if( it == renderers.end() )
        {
            renderers.push_back( &renderer );
        }
    }

    /** \brief  Removes a renderer.
      *
      * Has no effect on previously called \ref create.
      */
    void removeRenderer( AxialPlaneRenderer& renderer )
    {
        Renderers::iterator it = std::find( renderers.begin(), renderers.begin() + renderers.size(), &renderer );
        if( it != renderers.end() )
        {
            renderers.erase( it );
        }
    }

    /** \brief  Makes the currently configured renderer assembly use the same intersection.
      */
    void create()
    {
        const unsigned int n = renderers.size();
        for( unsigned int i = 0; i < n; ++i )
        {
            const unsigned int j = ( i + 1 ) % renderers.size();

            QObject::connect
                ( renderers[ i ]
                , SIGNAL( intersectionChanged( const Carna::base::Vector& ) )
                , renderers[ j ]
                , SLOT( setIntersection( const Carna::base::Vector& ) ) );
        }
    }


private: // TODO: implement PIMPL

    typedef std::deque< AxialPlaneRenderer* > Renderers;

    Renderers renderers;

}; // MPR :: IntersectionSynchronisationFactory



}  // namespace Carna :: MPR

}  // namespace Carna

#endif // AXIALPLANE_INTERSECTIONSYNCHRONISATIONFACTORY_H_6014714286
