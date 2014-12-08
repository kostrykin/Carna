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

#ifndef UINT16TEXTUREUPLOADER_H_6014714286
#define UINT16TEXTUREUPLOADER_H_6014714286

/** \file   UInt16TextureUploader.h
  *
  * \brief  Defines \ref Carna::base::view::UInt16TextureUploader.
  *
  * \author Leonid Kostrykin
  * \date   25.7.11
  */

#include <Carna/base/Vector3.h>
#include <Carna/Carna.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// UInt16TextureUploader
// ----------------------------------------------------------------------------------

/** \brief  Uploads the data as 16bit unsigned integer 3D texture to the GPU.
  *
  * \author Leonid Kostrykin
  * \date   2011
  */
class CARNA_EXPORT UInt16TextureUploader
{

public:

    /** \brief  Uploads the data as 16bit unsigned integer 3D texture to the GPU.
      */
    void uploadTexture( const Vector3ui& size, const void* bufferPtr ) const;

}; // UInt16TextureUploader



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // UINT16TEXTUREUPLOADER_H_6014714286
