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

#include <Carna/base/Texture3D.h>

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

class CARNA_LIB HUVolumeTexture : public Texture3D
{

protected:

    HUVolumeTexture( const HUVolume& volume );

public:

    const HUVolume& volume;

    virtual bool controlsSameVideoResource( const GeometryFeature& ) const override;

}; // HUVolumeTexture



}  // namespace Carna :: base

}  // namespace Carna

#endif // HUVOLUMETEXTURE_H_6014714286
