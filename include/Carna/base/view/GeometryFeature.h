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

#ifndef GEOMETRYFEATURE_H_6014714286
#define GEOMETRYFEATURE_H_6014714286

/** \file   GeometryFeature.h
  * \brief  Defines \ref Carna::base::view::GeometryFeature.
  */

#include <Carna/base/noncopyable.h>
#include <set>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// GeometryFeature
// ----------------------------------------------------------------------------------

/** \brief
  * Represents \em "components" that are aggregated by \ref Geometry instances.
  *
  * \author Leonid Kostrykin
  * \date   1.3.2015
  */
class CARNA_LIB GeometryFeature
{

    NON_COPYABLE

    unsigned int myVideoResourceAcquisitions;

    std::set< Geometry* > referencingSceneGraphNodes;

    bool released;

    bool deleteIfAllowed();

protected:

    GeometryFeature();

    virtual ~GeometryFeature();

public:

    unsigned int videoResourceAcquisitionsCount() const;

    /** \brief
      * Instantiates. Call \ref release when you do not need the object any longer.
      */
    //static GeometryFeature& create();

    /** \brief
      * Increments video resource acquisition counter.
      * Uploads resources to video memory if not happened yet.
      *
      * \important
      * The caller must ensure that this method is called while proper OpenGL context is active.
      */
    virtual void acquireVideoResource();
    
    /** \brief
      * Decrements video resource acquisition counter.
      * Frees acquired video memory if counter reaches 0.
      *
      * \important
      * The caller must ensure that this method is called while proper OpenGL context is active.
      */
    virtual void releaseVideoResource();

    virtual bool controlsSameVideoResource( const GeometryFeature& ) const = 0;

    /** \brief
      * Denotes that this object is no longer required and may be deleted as soon as it is valid to delete it.
      *
      * It will be deleted as soon as the following conditions become true, which may also be immediately:
      * - The video resources are not loaded, i.e. the last acquisition has been released.
      * - This aggregate is no longer referenced by a scene graph.
      *
      * If no video resources are acquired currently, this object is deleted immediately.
      */
    void release();

    /** \brief
      * Puts this geometry aggregate on the \a sceneGraphNode.
      *
      * This is equivalent to \ref Geometry::setDefinition.
      */
    void addTo( Geometry& sceneGraphNode, unsigned int role );

    /** \brief
    * Removes this geometry aggregate from the \a sceneGraphNode.
    *
    * This is equivalent to \ref Geometry::removeDefinition.
      */
    void removeFrom( Geometry& sceneGraphNode );

}; // GeometryFeature



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // GEOMETRYFEATURE_H_6014714286
