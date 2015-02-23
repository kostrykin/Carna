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

#ifndef BUFFEREDHUVOLUMEMANAGER_H_6014714286
#define BUFFEREDHUVOLUMEMANAGER_H_6014714286

#include <Carna/base/view/VideoResourcesManager.h>
#include <Carna/base/view/BufferedHUVolumeUploader.h>
#include <Carna/base/view/Texture3D.h>

/** \file   BufferedHUVolumeManager.h
  * \brief  Defines \ref Carna::base::view::BufferedHUVolumeManager.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// BufferedHUVolumeManager
// ----------------------------------------------------------------------------------

/** \brief
  * Generic \ref VideoResourcesManager for \ref BufferedHUVolume instances.
  */
template< typename BufferedHUVolume >
class BufferedHUVolumeManager : public VideoResourcesManager
{

    std::unique_ptr< Texture3D > myTexture;

public:

    const BufferedHUVolume& volume;

    BufferedHUVolumeManager( const BufferedHUVolume& );

    virtual void uploadResources() override;

    virtual void deleteResources() override;

    const Texture3D& texture() const;

}; // BufferedHUVolumeManager


template< typename BufferedHUVolume >
BufferedHUVolumeManager< BufferedHUVolume >::BufferedHUVolumeManager( const BufferedHUVolume& volume )
    : volume( volume )
{
}


template< typename BufferedHUVolume >
void BufferedHUVolumeManager< BufferedHUVolume >::uploadResources()
{
    CARNA_ASSERT( myTexture.get() == nullptr );
    myTexture.reset( new Texture3D() );
    BufferedHUVolumeUploader< BufferedHUVolume >::upload( volume, *myTexture );
}


template< typename BufferedHUVolume >
void BufferedHUVolumeManager< BufferedHUVolume >::deleteResources()
{
    myTexture.reset();
}


template< typename BufferedHUVolume >
const Texture3D& BufferedHUVolumeManager< BufferedHUVolume >::texture() const
{
    CARNA_ASSERT( myTexture.get() != nullptr );
    return *myTexture;
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // BUFFEREDHUVOLUMEMANAGER_H_6014714286
