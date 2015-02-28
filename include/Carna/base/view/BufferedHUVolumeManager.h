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

#include <Carna/base/view/Texture3DManager.h>
#include <Carna/base/view/BufferedHUVolumeUploader.h>
#include <Carna/base/view/Texture3D.h>
#include <memory>

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
  * Generic \ref VideoResourceManager for \ref BufferedHUVolume instances.
  *
  * \author Leonid Kostrykin
  * \date 22.2.2015
  */
template< typename BufferedHUVolume >
class BufferedHUVolumeManager
    : public VideoResourceManager< Texture3D, BufferedHUVolumeManager< BufferedHUVolume >, const BufferedHUVolume&, const BufferedHUVolume& >
{

protected:

    friend class VideoResourceManagerBase;

    BufferedHUVolumeManager( const BufferedHUVolume& volume );

    virtual Texture3D* createResource() override;

public:

    const BufferedHUVolume& volume() const;

    virtual bool controlsSameVideoResource( const GeometryAggregate& ) const override;

}; // BufferedHUVolumeManager


template< typename BufferedHUVolume >
BufferedHUVolumeManager< BufferedHUVolume >::BufferedHUVolumeManager( const BufferedHUVolume& volume )
    : VideoResourceManagerBase::VideoResourceManagerBase( volume )
{
}


template< typename BufferedHUVolume >
const BufferedHUVolume& BufferedHUVolumeManager< BufferedHUVolume >::volume() const
{
    return source;
}


template< typename BufferedHUVolume >
Texture3D* BufferedHUVolumeManager< BufferedHUVolume >::createResource()
{
    Texture3D* const texture = new Texture3D();
    BufferedHUVolumeUploader< BufferedHUVolume >::upload( source, *texture );
    return texture;
}


template< typename BufferedHUVolume >
bool BufferedHUVolumeManager< BufferedHUVolume >::controlsSameVideoResource( const GeometryAggregate& other ) const
{
    typedef BufferedHUVolumeManager< typename BufferedHUVolume > MyType;
    const MyType* const other2 = dynamic_cast< const MyType* >( &other );
    if( other2 == nullptr )
    {
        return false;
    }
    else
    {
        return &source == &other2->source;
    }
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // BUFFEREDHUVOLUMEMANAGER_H_6014714286
