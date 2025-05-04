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

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/GeometryFeature.hpp>
#include <LibCarna/base/noncopyable.hpp>
#include <LibCarna/base/math.hpp>

/** \file   ManagedTexture3DInterface.h
  * \brief  Defines \ref LibCarna::base::ManagedTexture3DInterface.
  */

namespace LibCarna
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



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // MANAGEDTEXTURE3DINTERFACE_H_6014714286
