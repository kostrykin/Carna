/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#ifndef VOLUME_H_6014714286
#define VOLUME_H_6014714286

/** \file   Volume.h
  *
  * \brief  Defines \ref Carna::base::model::Volume.
  *
  * \author Leonid Kostrykin
  * \date   25.7.11
  */

#include <Carna/base/model/ScalarField.h>
#include <Carna/base/noncopyable.h>

class DicomSeries;  // from dicom-interface
class QWidget;

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// Volume
// ----------------------------------------------------------------------------------

/** \brief  Abstract definition of \f$\mathbb N_0^3 \to [-1024,3071]\f$ volumetric data.
  *
  * \see    Histograms can be computed easily using the \ref VolumeHistogram class.
  *
  * \author Leonid Kostrykin
  * \date   25.7.11
  */
class CARNA_EXPORT Volume : public ScalarField< signed short >
{

    NON_COPYABLE

public:

    /** \brief  Instantiates.
      */
    Volume();

    /** \brief  Instantiates.
      */
    explicit Volume( const Vector3ui& size );
    

    /** \brief  Holds the dimensions.
      */
    Vector3ui size;
    

    /** \brief  Uploads the data as 3D texture to the video memory.
      *
      * The implementation must meet the following requirements:
      * - The data must be uploaded to the texture object currently bound.
      * - The texture must be initialized as a single-channel (red) \c GL_TEXTURE_3D texture.
      * - The HU-values from \f$[-1024, 3071]\f$ must be provided as a linearly mapped \f$[0, 1]\f$ interval.
      *
      * Exemplary implementation:
      *
      * \code
      * glTexImage3D( GL_TEXTURE_3D, 0, GL_INTENSITY16
      *     , size.x, size.y, size.z
      *     , 0, GL_RED, GL_UNSIGNED_SHORT, buffer_ptr );
      * \endcode
      */
    virtual void uploadTexture() const = 0;

    
    /** \brief  Returns HUV of specified voxel.
      */
    virtual signed short operator()( unsigned int x
                                   , unsigned int y
                                   , unsigned int z ) const override = 0;

    /** \brief  Returns HUV of specified voxel.
      */
    virtual signed short operator()( const Vector3ui& at ) const override = 0;


    /** \brief  Describes the corresponding 3D texture.
      */
    class Texture
    {

        NON_COPYABLE

    public:

        /** \brief  Instantiates new GL texture object.
          *
          * The GL texture object is populated with the data obtained from \a volume.
          */
        Texture( const Volume& volume );

        /** \brief  Destroys associated GL texture object.
          */
        ~Texture();

        /** \brief  References associated GL texture object.
          */
        const unsigned int textureID;

    }; // Texture

}; // Volume



}  // namesapce Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // VOLUME_H_6014714286
