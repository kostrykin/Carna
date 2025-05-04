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

#ifndef MANAGEDTEXTURE3DINTERFACE_H_6014714286
#define MANAGEDTEXTURE3DINTERFACE_H_6014714286

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/GeometryFeature.h>
#include <LibCarna/base/noncopyable.h>
#include <LibCarna/base/math.h>

/** \file   ManagedTexture3DInterface.h
  * \brief  Defines \ref Carna::base::ManagedTexture3DInterface.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ManagedTexture3DInterface
// ----------------------------------------------------------------------------------

/** \brief
  * Interfaces the \ref Texture<3> that a \ref ManagedTexture3D represents.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 24.3.15
  */
class LIBCARNA ManagedTexture3DInterface : public GeometryFeature::ManagedInterface
{

    NON_COPYABLE

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    /** \brief
      * Creates a new interface to the \ref Texture<3> that \a managed represents.
      */
    explicit ManagedTexture3DInterface( ManagedTexture3D& managed );

    /** \brief
      * Releases this interface.
      */
    virtual ~ManagedTexture3DInterface();

    /** \brief
      * References the `%ManagedTexture3D` object this interface belongs to.
      */
    ManagedTexture3D& managed;
        
    /** Referencs the texture represented by \ref managed.
      */
    const Texture< 3 >& get() const;

}; // ManagedTexture3DInterface



}  // namespace Carna :: base

}  // namespace Carna

#endif // MANAGEDTEXTURE3DINTERFACE_H_6014714286
