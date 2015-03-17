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
  * Represents \em "components" that are aggregated by \ref Geometry objects.
  * Closer description is given \ref GeometryFeatures "here".
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

    /** \brief
      * Instantiates.
      */
    GeometryFeature();

    /** \brief
      * Deletes.
      */
    virtual ~GeometryFeature();

public:

    /** \brief
      * Tells current number of
      * \ref VideoResourceAcquisition "video resource acquisitions".
      */
    unsigned int videoResourceAcquisitionsCount() const;

    /** \brief
      * Tells whether this instance maintains the same video resources like \a other.
      */
    virtual bool controlsSameVideoResource( const GeometryFeature& other ) const = 0;

    /** \brief
      * Denotes that this object is no longer required and may be deleted as soon as
      * it is valid to delete it.
      *
      * It will be deleted as soon as the following conditions become true, which may
      * also be immediately:
      *
      * - The video resources are not loaded, i.e. the last acquisition was released.
      * - This feature is no longer referenced by a scene graph.
      */
    void release();

    /** \brief
      * Assumes that all remained instances are leaked and records their number on
      * the \ref Log "log" with `error` severity if it is not zero. Instances, that
      * are counted this time, will not be counted by succeeding method calls.
      */
    static void checkLeakedInstances();

    /** \brief
      * Puts this geometry feature on the \a sceneGraphNode.
      *
      * This is equivalent to \ref Geometry::setDefinition.
      */
    void addTo( Geometry& sceneGraphNode, unsigned int role );

    /** \brief
    * Removes this geometry feature from the \a sceneGraphNode.
    *
    * This is equivalent to \ref Geometry::removeDefinition.
      */
    void removeFrom( Geometry& sceneGraphNode );

    // ------------------------------------------------------------------------------
    // GeometryFeature :: VideoResourceAcquisition
    // ------------------------------------------------------------------------------

    /** \brief
      * Represents an acquisition of video resources from a particular
      * \ref GeometryFeature. This realizes the RAII idiom.
      */
    class CARNA_LIB VideoResourceAcquisition
    {
    
        NON_COPYABLE
        
    protected:
    
        /** \brief
          * Acquires the video resources from \a geometryFeature.
          *
          * The instantiated \ref VideoResourceAcquisition object is only valid
          * within the \ref GLContext "current OpenGL context". Delete the object in
          * order to release the acquired resources. Usually the resources are
          * uploaded to video memory, when they are first acquired, and deleted from
          * video memory, when the last acquisition is released. This depends on
          * whether the resources are \ref GLContext "sharable" across OpenGL
          * contexts.
          */
        explicit VideoResourceAcquisition( GeometryFeature& geometryFeature );
        
    public:
    
        /** \brief
          * Releases the video resources previously acquired from
          * \ref geometryFeature.
          */
        virtual ~VideoResourceAcquisition();
        
        /** \brief
          * References the `%GeometryFeature` that video resources are acquired from.
          */
        GeometryFeature& geometryFeature;
        
    }; // GeometryFeature :: VideoResourceAcquisition
    
    /** \brief
      * Acquires the video resources from this `%GeometryFeature` by returning new
      * instance of a class derived from \ref VideoResourceAcquisition, that realizes
      * the RAII idiom. Refer to its documentation for details.
      */
    virtual VideoResourceAcquisition* acquireVideoResource() = 0;

}; // GeometryFeature



}  // namespace Carna :: base

}  // namespace Carna

#endif // GEOMETRYFEATURE_H_6014714286
