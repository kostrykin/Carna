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

#include <Carna/base/view/Texture3D.h>

/** \file   HUVolumeTexture.h
  * \brief  Defines \ref Carna::base::view::HUVolumeTexture.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// HUVolumeTexture
// ----------------------------------------------------------------------------------

class CARNA_LIB HUVolumeTexture : public Texture3D
{

protected:

    HUVolumeTexture( const model::HUVolume& volume );

public:

    const model::HUVolume& volume;

    virtual bool controlsSameVideoResource( const GeometryFeature& ) const override;

}; // HUVolumeTexture



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // HUVOLUMETEXTURE_H_6014714286
