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
  * \brief  Defines \ref Carna::base::GeometryFeature.
  */

#include <Carna/base/noncopyable.h>
#include <set>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// GeometryFeature
// ----------------------------------------------------------------------------------

/** \brief
  * Represents \em "components" that are aggregated by \ref Geometry instances.
  *
  * \author Leonid Kostrykin
  * \date   1.3.15 - 13.3.15
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

    virtual bool controlsSameVideoResource( const GeometryFeature& ) const = 0;

    /** \brief
      * Denotes that this object is no longer required and may be deleted as soon as
      * it is valid to delete it.
      *
      * It will be deleted as soon as the following conditions become true, which may
      * also be immediately:
      *
      * - The video resources are not loaded, i.e. the last acquisition was released.
      * - This aggregate is no longer referenced by a scene graph.
      */
    void release();

    /** \brief
      * Assumes that all remained instances are leaked and records their number on
      * the \ref Log "log" with `error` severity if it is not zero. Instances, that
      * are counted this time, will not be counted by succeeding method calls.
      */
    static void checkLeakedInstances();

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

    // ------------------------------------------------------------------------------
    // GeometryFeature :: VideoResourceAcquisition
    // ------------------------------------------------------------------------------

    class CARNA_LIB VideoResourceAcquisition
    {
    
        NON_COPYABLE
        
    protected:
    
        explicit VideoResourceAcquisition( GeometryFeature& gf );
        
    public:
    
        virtual ~VideoResourceAcquisition();
        
        GeometryFeature& geometryFeature;
        
    }; // GeometryFeature :: VideoResourceAcquisition
    
    virtual VideoResourceAcquisition* acquireVideoResource() = 0;

}; // GeometryFeature



}  // namespace Carna :: base

}  // namespace Carna

#endif // GEOMETRYFEATURE_H_6014714286
