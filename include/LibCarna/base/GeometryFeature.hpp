/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#ifndef GEOMETRYFEATURE_H_6014714286
#define GEOMETRYFEATURE_H_6014714286

/** \file
  * \brief
  * Defines \ref LibCarna::base::GeometryFeature.
  */

#include <LibCarna/base/noncopyable.hpp>
#include <set>

namespace LibCarna
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
  */
class LIBCARNA GeometryFeature
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

protected:

    /** \brief
      * Instantiates.
      */
    GeometryFeature();

    /** \brief
      * Deletes and \ref Log "logs" an error if video resources are leaked.
      */
    virtual ~GeometryFeature();

public:

    /** \brief
      * Tells current number of \ref ManagedInterface "video resource acquisitions".
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
      * Puts this geometry feature on the \a sceneGraphNode.
      *
      * This is equivalent to \ref Geometry::putFeature.
      */
    void addTo( Geometry& sceneGraphNode, unsigned int role );

    /** \brief
    * Removes this geometry feature from the \a sceneGraphNode.
    *
    * This is equivalent to \ref Geometry::removeFeature.
      */
    void removeFrom( Geometry& sceneGraphNode );

    // ------------------------------------------------------------------------------
    // GeometryFeature :: ManagedInterface
    // ------------------------------------------------------------------------------

    /** \brief
      * Represents an acquisition of the video resources from a particular
      * \ref GeometryFeature. This *acquisition* realizes the RAII idiom.
      *
      * \author Leonid Kostrykin
      */
    class LIBCARNA ManagedInterface
    {
    
        NON_COPYABLE
        
    protected:
    
        /** \brief
          * Acquires the video resources from \a gf.
          *
          * The instantiated `%ManagedInterface` object is only valid within the
          * \ref GLContext "current OpenGL context". Delete the object in order to
          * release the acquired resources. Usually the resources are uploaded to
          * video memory when they are first acquired, and deleted from video memory
          * when the last acquisition is released. This depends on whether the
          * resources are \ref GLContext "sharable" across OpenGL contexts.
          */
        explicit ManagedInterface( GeometryFeature& gf );
        
    public:
    
        /** \brief
          * Releases the video resources previously acquired from the managing
          * \ref geometryFeature.
          */
        virtual ~ManagedInterface() noexcept( false );
        
        /** \brief
          * References the `%GeometryFeature` that manages the video resources that
          * this object interfaces.
          */
        GeometryFeature& geometryFeature;
        
    }; // GeometryFeature :: ManagedInterface
    
    /** \brief
      * Acquires the video resources from this `%GeometryFeature` by returning new
      * instance of a class derived from \ref ManagedInterface, that realizes the
      * RAII idiom. Refer to its documentation for details. May return `nullptr` if
      * this `%GeometryFeature` does not provide any video resources.
      */
    virtual ManagedInterface* acquireVideoResource() = 0;

}; // GeometryFeature



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // GEOMETRYFEATURE_H_6014714286
