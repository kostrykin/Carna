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

#ifndef MANAGEDTEXTURE3D_H_6014714286
#define MANAGEDTEXTURE3D_H_6014714286

#include <LibCarna/LibCarna.h>
#include <LibCarna/base/ManagedTexture3DInterface.h>
#include <LibCarna/base/GeometryFeature.h>
#include <LibCarna/base/noncopyable.h>
#include <LibCarna/base/math.h>

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
  * Represents \ref Texture "3D OpenGL texture object" whose lifetime is managed by
  * instances of this class.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA ManagedTexture3D : public GeometryFeature
{

    NON_COPYABLE

protected:

    friend class GeometryFeature;
    friend class ManagedTexture3DInterface;

    /** \brief
      * Instantiates.
      *
      * \param size
      *     is the resolution of this texture.
      *
      * \param internalFormat
      *     specifies the number of color components in the texture, e.g.
      *     `GL_RGBA8UI` or `GL_INTENSITY16`.
      *
      * \param pixelFormat
      *     specifies the format of the pixel data, e.g. `GL_RED`, `GL_RGB` or
      *     `GL_RGBA`.
      *
      * \param bufferType
      *     specifies the data type of the pixel data pointed to by \a bufferPtr.
      *
      * \param bufferPtr
      *     points to the pixel data that will be uploaded to the texture.
      *
      * \see
      * Valid values for the parameters are available here:
      * https://www.opengl.org/sdk/docs/man3/xhtml/glTexImage3D.xml
      *
      * \attention
      * The instance does neither upload the pixel data from \a bufferPtr
      * immediately, nor does it create a copy. Hence the pixel data located at
      * \a bufferPtr must stay valid as long as an upload might be required.
      */
    ManagedTexture3D( const math::Vector3ui& size, int internalFormat, int pixelFormat, int bufferType, const void* bufferPtr );

    /** \brief
      * Deletes.
      */
    ~ManagedTexture3D();

    /** \brief
      * Holds the maintained OpenGL texture object.
      */
    std::unique_ptr< Texture< 3 > > textureObject;

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    /** \brief
      * Defines the type to be used for interfacing the video resource.
      */
    typedef ManagedTexture3DInterface ManagedInterface;

    /** \brief
      * Instantiates.
      * Invoke \ref release when it isn't needed any longer.
      *
      * \copydetails ManagedTexture3D::ManagedTexture3D(const math::Vector3ui&, int, int, int, const void*)
      */
    static ManagedTexture3D& create
        ( const math::Vector3ui& size
        , int internalFormat
        , int pixelFormat
        , int bufferType
        , const void* bufferPtr );

    const math::Vector3ui size;  ///< \copydoc Texture::size
    const int internalFormat;    ///< \copydoc Texture::internalFormat
    const int pixelFormat;       ///< \copydoc Texture::pixelFormat
    const int bufferType;        ///< Specifies the data type of the pixel data to be uploaded to the texture.
    const void* const bufferPtr; ///< Points to the pixel data that will be uploaded to the texture.

    /** \brief
      * Stretches texture coordinates s.t. the centers of the texels, that are
      * located in the texture corners, become located in those corners.
      *
      * Consider a \f$4 \times 4\f$ texture. Each texel occupies \f$\frac{1}{4}\f$
      * along each axis, hence the texels' centers are located at \f$\frac{1}{8}\f$,
      * \f$\frac{3}{8}\f$, \f$\frac{5}{8}\f$ and \f$\frac{7}{8}\f$ along those axis.
      *
      * This matrix transforms texture coordinates s.t.
      * \f$ 0 \mapsto \frac{1}{8}\f$ and \f$ 1 \mapsto \frac{7}{8}\f$ following the
      * considerations from above.
      */
    const base::math::Matrix4f textureCoordinatesCorrection;
    
    /** \copydoc GeometryFeature::controlsSameVideoResource(const GeometryFeature&) const
      *
      * This implementation always returns `false`.
      */
    virtual bool controlsSameVideoResource( const GeometryFeature& ) const override;
    
    virtual ManagedTexture3DInterface* acquireVideoResource() override;

}; // ManagedTexture3D



}  // namespace Carna :: base

}  // namespace Carna

#endif // MANAGEDTEXTURE3D_H_6014714286
