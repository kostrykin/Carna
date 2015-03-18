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
  * Maintains a 3D OpenGL texture object.
  *
  * \author Leonid Kostrykin
  * \date   22.2.15 - 18.3.15
  */
class CARNA_LIB Texture3D : public GeometryFeature
{

    NON_COPYABLE

    unsigned int id;

protected:

    friend class GeometryFeature;

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
      * \attention
      * The instance does neither upload the pixel data from \a bufferPtr
      * immediately, nor does it create a copy. Hence the pixel data located at
      * \a bufferPtr must stay valid as long as an upload might be required.
      *
      * \see
      * Valid values for the parameters are available here:
      * https://www.opengl.org/sdk/docs/man3/xhtml/glTexImage3D.xml
      */
    Texture3D( const math::Vector3ui& size, int internalFormat, int pixelFormat, int bufferType, const void* bufferPtr );

    /** \brief
      * Deletes.
      */
    virtual ~Texture3D();

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    /** \brief
      * Texture unit that is fine to be used for temporal bindings, i.e. for setting
      * textures up. This unit shouldn't be used for lasting bindings.
      */
    const static unsigned int SETUP_UNIT = 0;

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

    const math::Vector3ui size;  ///< Holds the resolution of this texture.
    const int internalFormat;    ///< Holds the number of color components in the texture, e.g. `GL_RGBA8UI` or `GL_INTENSITY16`.
    const int pixelFormat;       ///< Holds the format of the pixel data, e.g. `GL_RED`, `GL_RGB` or `GL_RGBA`.
    const int bufferType;        ///< Holds the data type of the pixel data pointed to by \ref bufferPtr.
    const void* const bufferPtr; ///< Points to the pixel data that will be uploaded to the texture.

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

    // ------------------------------------------------------------------------------
    // Texture3D :: VideoResourceAcquisition
    // ------------------------------------------------------------------------------
    
    /** \brief
      * Represents an acquisition of video resources from a particular
      * \ref Texture3D. This realizes the RAII idiom.
      *
      * \author Leonid Kostrykin
      * \date   22.2.15 - 18.3.15
      */
    class CARNA_LIB VideoResourceAcquisition : public GeometryFeature::VideoResourceAcquisition
    {
    
    public:
    
        /** \brief
          * Acquires the video resources from \a texture.
          *
          * \copydetails GeometryFeature::VideoResourceAcquisition::VideoResourceAcquisition(GeometryFeature&)
          */
        VideoResourceAcquisition( Texture3D& texture );
    
        /** \copydoc GeometryFeature::VideoResourceAcquisition::~VideoResourceAcquisition()
          */
        virtual ~VideoResourceAcquisition();
        
        /** \brief
          * Tells the texture ID.
          */
        unsigned int id() const;

        /** \brief
          * Binds this texture to \a unit.
          *
          * Consider using \ref SETUP_UNIT if you're binding the texture temporarily.
          */
        void bind( unsigned int unit ) const;
    
        /** \brief
          * References the texture.
          */
        Texture3D& texture;
    
    }; // Texture3D :: VideoResourceAcquisition
    
    virtual VideoResourceAcquisition* acquireVideoResource() override;

}; // Texture3D



}  // namespace Carna :: base

}  // namespace Carna

#endif // TEXTURE3D_H_6014714286
