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

#ifndef TEXTURE3D_H_6014714286
#define TEXTURE3D_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/ManagedTexture3D.h>
#include <Carna/base/GeometryFeature.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/math.h>

/** \file   Texture3D.h
  * \brief  Defines \ref Carna::base::Texture3D.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Texture3D
// ----------------------------------------------------------------------------------

/** \brief
  * Maintains a \ref Texture3DObject "3D OpenGL texture object".
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 24.3.15
  */
class CARNA_LIB Texture3D : public GeometryFeature
{

    NON_COPYABLE

protected:

    friend class GeometryFeature;
    friend class ManagedTexture3D;

    /** \brief
      * Instantiates.
      *
      * \copydetails Texture3DObject::Texture3DObject
      *
      * \attention
      * The instance does neither upload the pixel data from \a bufferPtr
      * immediately, nor does it create a copy. Hence the pixel data located at
      * \a bufferPtr must stay valid as long as an upload might be required.
      */
    Texture3D( const math::Vector3ui& size, int internalFormat, int pixelFormat, int bufferType, const void* bufferPtr );

    /** \brief
      * Deletes.
      */
    ~Texture3D();

    /** \brief
      * Holds the maintained OpenGL texture object.
      */
    std::unique_ptr< Texture3DObject > textureObject;

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    /** \brief
      * Defines the type to be used for interfacing the video resource.
      */
    typedef ManagedTexture3D VideoResourceAcquisition;

    /** \brief
      * Instantiates and associates with a newly created OpenGL texture object.
      * Invoke \ref release when it isn't needed any longer.
      *
      * \copydetails Texture3D::Texture3D(const math::Vector3ui&, int, int, int, const void*)
      */
    static Texture3D& create
        ( const math::Vector3ui& size
        , int internalFormat
        , int pixelFormat
        , int bufferType
        , const void* bufferPtr );

    const math::Vector3ui size;  ///< \copydoc Texture3DObject::size
    const int internalFormat;    ///< \copydoc Texture3DObject::internalFormat
    const int pixelFormat;       ///< \copydoc Texture3DObject::pixelFormat
    const int bufferType;        ///< \copydoc Texture3DObject::bufferType
    const void* const bufferPtr; ///< \copydoc Texture3DObject::bufferPtr

    /** \brief
      * Stretches texture coordinates s.t. the centers of the texels, that are
      * located in the texture corners, become located in those corners.
      *
      * Consider a \f$4 \times 4\f$ texture. Each texel occupies \f$\frac{1}{4}\f$
      * along each axis, hence the texels' centers are located at \f$\frac{1}{8}\f$,
      * \f$\frac{3}{8}\f$, \f$\frac{5}{8}\f$ and \f$\frac{7}{8}\f$ along those axis.
      *
      * The matrix returned by this method transforms texture coordinates s.t.
      * \f$ 0 \mapsto \frac{1}{8}\f$ and \f$ 1 \mapsto \frac{7}{8}\f$ following the
      * considerations from above. The matrix is computed the first time the method
      * is invoked. It is reused until the \ref upload "texture resolution changes".
      */
    const base::math::Matrix4f textureCoordinatesCorrection;
    
    /** \copydoc GeometryFeature::controlsSameVideoResource(const GeometryFeature&) const
      *
      * This implementation always returns `false`.
      */
    virtual bool controlsSameVideoResource( const GeometryFeature& ) const override;
    
    virtual ManagedTexture3D* acquireVideoResource() override;

}; // Texture3D



}  // namespace Carna :: base

}  // namespace Carna

#endif // TEXTURE3D_H_6014714286
