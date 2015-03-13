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

class CARNA_LIB Texture3D : public GeometryFeature
{

    NON_COPYABLE
    
    const void* const bufferPtr;

    unsigned int id;

protected:

    friend class GeometryFeature;

    Texture3D( const math::Vector3ui& size, int internalFormat, int pixelFormat, int bufferType, const void* bufferPtr );

    /** \brief
      * Deletes the associated OpenGL object.
      */
    virtual ~Texture3D();

public:

    /** \brief
      * Texture unit that is fine to be used for temporal bindings, i.e. for setting
      * textures up. This unit shouldn't be used for lasting bindings.
      */
    const static unsigned int SETUP_UNIT = 0;

    /** \brief
      * Instantiates and associates with a newly created OpenGL texture object.
      * Invoke \ref release when it isn't needed any longer.
      */
    static Texture3D& create
        ( const math::Vector3ui& size
        , int internalFormat
        , int pixelFormat
        , int bufferType
        , const void* bufferPtr );

    const math::Vector3ui size;
    
    const int internalFormat;
    
    const int pixelFormat;
    
    const int bufferType;

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

    virtual bool controlsSameVideoResource( const GeometryFeature& ) const override;

    // ------------------------------------------------------------------------------
    // Texture3D :: VideoResourceAcquisition
    // ------------------------------------------------------------------------------

    class VideoResourceAcquisition : public GeometryFeature::VideoResourceAcquisition
    {
    
    public:
    
        VideoResourceAcquisition( GLContext& glc, Texture3D& texture );
    
        virtual ~VideoResourceAcquisition();
        
        unsigned int id() const;

        /** \brief
          * Binds this texture to \a unit.
          *
          * Consider using \ref SETUP_UNIT if you're binding the texture temporarily.
          */
        void bind( unsigned int unit ) const;
    
        Texture3D& texture;
    
    }; // Texture3D :: VideoResourceAcquisition
    
    virtual VideoResourceAcquisition* acquireVideoResource( GLContext& glc ) override;

}; // Texture3D



}  // namespace Carna :: base

}  // namespace Carna

#endif // TEXTURE3D_H_6014714286
