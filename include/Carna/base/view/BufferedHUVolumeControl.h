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

#ifndef BUFFEREDHUVOLUMECONTROL_H_6014714286
#define BUFFEREDHUVOLUMECONTROL_H_6014714286

#include <Carna/base/view/Texture3DControl.h>
#include <Carna/base/view/BufferedHUVolumeUploader.h>
#include <Carna/base/view/Texture3D.h>
#include <memory>

/** \file   BufferedHUVolumeControl.h
  * \brief  Defines \ref Carna::base::view::BufferedHUVolumeControl.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// BufferedHUVolumeControl
// ----------------------------------------------------------------------------------

/** \brief
  * Generic \ref VideoResourcesControl for \ref BufferedHUVolume instances.
  *
  * \author Leonid Kostrykin
  * \date 22.2.2015
  */
template< typename BufferedHUVolume >
class BufferedHUVolumeControl : public Texture3DControl
{

    std::unique_ptr< Texture3D > myTexture;

public:

    const BufferedHUVolume& volume;

    BufferedHUVolumeControl( const BufferedHUVolume& );

    virtual void uploadResources() override;

    virtual void deleteResources() override;

    virtual const Texture3D& texture() const override;

}; // BufferedHUVolumeControl


template< typename BufferedHUVolume >
BufferedHUVolumeControl< BufferedHUVolume >::BufferedHUVolumeControl( const BufferedHUVolume& volume )
    : volume( volume )
{
}


template< typename BufferedHUVolume >
void BufferedHUVolumeControl< BufferedHUVolume >::uploadResources()
{
    CARNA_ASSERT( myTexture.get() == nullptr );
    myTexture.reset( new Texture3D() );
    BufferedHUVolumeUploader< BufferedHUVolume >::upload( volume, *myTexture );
}


template< typename BufferedHUVolume >
void BufferedHUVolumeControl< BufferedHUVolume >::deleteResources()
{
    myTexture.reset();
}


template< typename BufferedHUVolume >
const Texture3D& BufferedHUVolumeControl< BufferedHUVolume >::texture() const
{
    CARNA_ASSERT( myTexture.get() != nullptr );
    return *myTexture;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // BUFFEREDHUVOLUMECONTROL_H_6014714286
