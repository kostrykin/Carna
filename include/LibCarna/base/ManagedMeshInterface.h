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

#ifndef MANAGEDMESHINTERFACE_H_6014714286
#define MANAGEDMESHINTERFACE_H_6014714286

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/GeometryFeature.h>
#include <LibCarna/base/noncopyable.h>
#include <LibCarna/base/math.h>

/** \file   ManagedMeshInterface.h
  * \brief  Defines \ref Carna::base::ManagedMeshInterface.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ManagedMeshInterface
// ----------------------------------------------------------------------------------

/** \brief
  * Interfaces the \ref MeshBase that a \ref ManagedMeshBase represents.
  *
  * \author Leonid Kostrykin
  * \date   1.9.14 - 24.3.15
  */
class LIBCARNA ManagedMeshInterface : public GeometryFeature::ManagedInterface
{

    NON_COPYABLE

public:

    /** \brief
      * Creates a new interface to the \ref MeshBase that \a managed represents.
      */
    explicit ManagedMeshInterface( ManagedMeshBase& managed );

    /** \brief
      * Releases this interface.
      */
    virtual ~ManagedMeshInterface();

    /** \brief
      * References the `%ManagedMeshBase` object this interface belongs to.
      */
    ManagedMeshBase& managed;
        
    /** Referencs the mesh represented by \ref managed.
      */
    const MeshBase& get() const;

}; // ManagedMeshInterface



}  // namespace Carna :: base

}  // namespace Carna

#endif // MANAGEDMESHINTERFACE_H_6014714286
