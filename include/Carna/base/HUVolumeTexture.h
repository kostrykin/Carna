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

#ifndef HUVOLUMETEXTURE_H_6014714286
#define HUVOLUMETEXTURE_H_6014714286

#include <Carna/base/ManagedTexture3D.h>

/** \file   HUVolumeTexture.h
  * \brief  Defines \ref Carna::base::HUVolumeTexture.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// HUVolumeTexture
// ----------------------------------------------------------------------------------

class CARNA_LIB HUVolumeTexture : public ManagedTexture3D
{

protected:

    HUVolumeTexture( const HUVolume& huVolume, int internalFormat, int bufferType, const void* bufferPtr );

public:

    const HUVolume& huVolume;

    virtual bool controlsSameVideoResource( const GeometryFeature& ) const override;

}; // HUVolumeTexture



}  // namespace Carna :: base

}  // namespace Carna

#endif // HUVOLUMETEXTURE_H_6014714286
