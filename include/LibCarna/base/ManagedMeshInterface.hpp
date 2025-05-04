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

#ifndef MANAGEDMESHINTERFACE_H_6014714286
#define MANAGEDMESHINTERFACE_H_6014714286

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/GeometryFeature.hpp>
#include <LibCarna/base/noncopyable.hpp>
#include <LibCarna/base/math.hpp>

/** \file
  * \brief
  * Defines \ref LibCarna::base::ManagedMeshInterface.
  */

namespace LibCarna
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



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // MANAGEDMESHINTERFACE_H_6014714286
