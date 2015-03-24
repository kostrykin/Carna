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

#ifndef MANAGEDTEXTURE3D_H_6014714286
#define MANAGEDTEXTURE3D_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/GeometryFeature.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/math.h>

/** \file   ManagedTexture3D.h
  * \brief  Defines \ref Carna::base::ManagedTexture3D.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ManagedTexture3D
// ----------------------------------------------------------------------------------

/** \brief
  * Interfaces a \ref Texture3DObject that is managed by a \ref Texture3D.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 24.3.15
  */
class CARNA_LIB ManagedTexture3D : public GeometryFeature::VideoResourceAcquisition
{

    NON_COPYABLE

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    /** \brief
      * Creates a new interface to the \ref Texture3DObject that is managed by
      * \a manager.
      */
    ManagedTexture3D( Texture3D& manager );

    /** \brief
      * Releases this interface.
      */
    virtual ~ManagedTexture3D();

    /** \brief
      * References the `%Texture3D` object that manages the \ref Texture3DObject that
      * this object interfaces.
      */
    Texture3D& manager;
        
    /** \brief
      * Tells the ID of the managed texture.
      */
    unsigned int id() const;

    /** \brief
      * Binds the managed texture to \a unit.
      *
      * \copydetails Texture3DObject::bind
      */
    void bind( unsigned int unit ) const;

}; // ManagedTexture3D



}  // namespace Carna :: base

}  // namespace Carna

#endif // MANAGEDTEXTURE3D_H_6014714286
