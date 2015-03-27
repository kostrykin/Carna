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

#ifndef VOLUMEGRIDHELPERDETAILS_H_6014714286
#define VOLUMEGRIDHELPERDETAILS_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/VolumeGrid.h>
#include <Carna/base/VolumeSegment.h>
#include <Carna/base/BufferedVectorFieldTexture.h>
#include <Carna/base/Geometry.h>
#include <map>

/** \file   VolumeGridHelperDetails.h
  * \brief  Defines \ref Carna::helpers::details::VolumeGridHelper.
  */

namespace Carna
{

namespace helpers
{

/** \brief
  * Holds class templates that contain implementation details.
  */
namespace details
{

/** \brief
  * Holds implementation details of \ref Carna::helpers::VolumeGridHelper.
  */
namespace VolumeGridHelper
{



// ----------------------------------------------------------------------------------
// HUTextureFactory< SegmentHUVolumeType, SegmentNormalsVolumeType >
// ----------------------------------------------------------------------------------

/** \brief
  * Creates \ref base::ManagedTexture "textures" that represents
  * \ref VolumeSegment::huVolume in video memory.
  *
  * \author Leonid Kostrykin
  * \date   26.3.15 - 27.3.15
  */
template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
struct HUTextureFactory
{
    typedef SegmentHUVolumeType SegmentHUVolume;
    typedef SegmentNormalsVolumeType SegmentNormalsVolume;

    static base::ManagedTexture3D& createTexture
        ( const base::VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment );
};


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
base::ManagedTexture3D& HUTextureFactory< SegmentHUVolumeType, SegmentNormalsVolumeType >::createTexture
    ( const base::VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment )
{
    return base::BufferedVectorFieldTexture< SegmentHUVolumeType >::create( segment.huVolume() );
}



// ----------------------------------------------------------------------------------
// NormalsTextureFactory< SegmentHUVolumeType, SegmentNormalsVolumeType >
// ----------------------------------------------------------------------------------

/** \brief
  * Creates \ref base::ManagedTexture "textures" that represents
  * \ref VolumeSegmentNormalsComponent::normals in video memory.
  *
  * \author Leonid Kostrykin
  * \date   26.3.15 - 27.3.15
  */
template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
struct NormalsTextureFactory
{
    typedef SegmentHUVolumeType SegmentHUVolume;
    typedef SegmentNormalsVolumeType SegmentNormalsVolume;

    static base::ManagedTexture3D& createTexture
        ( const base::VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment );
};


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
base::ManagedTexture3D& NormalsTextureFactory< SegmentHUVolumeType, SegmentNormalsVolumeType >::createTexture
    ( const base::VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment )
{
    return base::BufferedVectorFieldTexture< SegmentNormalsVolumeType >::create( segment.normals() );
}



// ----------------------------------------------------------------------------------
// TextureManager< TextureFactory >
// ----------------------------------------------------------------------------------

/** \brief
  * Provides mapping \ref base::VolumeSegment to \ref base::ManagedTexture3D objects
  * in a caching manner. Uses \a TextureFactory to create new
  * \ref base::ManagedTexture3D instances when cache entry does not exist.
  *
  * \author Leonid Kostrykin
  * \date   27.3.15
  */
template< typename TextureFactory >
class TextureManager
{

    NON_COPYABLE

    mutable std::map
        < const base::VolumeSegment
            < typename TextureFactory::SegmentHUVolume
            , typename TextureFactory::SegmentNormalsVolume >*
        , base::ManagedTexture3D* > textures;

public:

    virtual ~TextureManager();

    void releaseGeometryFeatures();

protected:

    void attachTexture
        ( base::Geometry& geometry
        , unsigned int role
        , const base::VolumeSegment
            < typename TextureFactory::SegmentHUVolume
            , typename TextureFactory::SegmentNormalsVolume >& segment ) const;

private:

    base::ManagedTexture3D& getTexture
        ( const base::VolumeSegment
            < typename TextureFactory::SegmentHUVolume
            , typename TextureFactory::SegmentNormalsVolume >& segment ) const;

}; // TextureManager


template< typename TextureFactory >
TextureManager< TextureFactory >::~TextureManager()
{
    releaseGeometryFeatures();
}


template< typename TextureFactory >
void TextureManager< TextureFactory >::releaseGeometryFeatures()
{
    for( auto itr = textures.begin(); itr != textures.end(); ++itr )
    {
        base::ManagedTexture3D& texture = *itr->second;
        texture.release();
    }
    textures.clear();
}


template< typename TextureFactory >
void TextureManager< TextureFactory >::attachTexture
    ( base::Geometry& geometry
    , unsigned int role
    , const base::VolumeSegment
        < typename TextureFactory::SegmentHUVolume
        , typename TextureFactory::SegmentNormalsVolume >& segment ) const
{
    base::ManagedTexture3D& texture = getTexture( segment );
    geometry.putFeature( role, texture );
}


template< typename TextureFactory >
base::ManagedTexture3D& TextureManager< TextureFactory >::getTexture
    ( const base::VolumeSegment
        < typename TextureFactory::SegmentHUVolume
        , typename TextureFactory::SegmentNormalsVolume >& segment ) const
{
    auto textureItr = textures.find( &segment );
    if( textureItr == textures.end() )
    {
        /* Create the texture.
         */
        base::ManagedTexture3D& texture = TextureFactory::createTexture( segment );
        textures[ &segment ] = &texture;
        return texture;
    }
    else
    {
        /* Use previously created texture.
         */
        return *textureItr->second;
    }
}



// ----------------------------------------------------------------------------------
// HUComponent< SegmentHUVolumeType, SegmentNormalsVolumeType >
// ----------------------------------------------------------------------------------

template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
class HUComponent : public TextureManager< HUTextureFactory< SegmentHUVolumeType, SegmentNormalsVolumeType > >
{

    unsigned int role;

public:

    const static unsigned int DEFAULT_ROLE_HU_VOLUME = 0;

    HUComponent();

    void setHUVolumeRole( unsigned int role );

    unsigned int huVolumeRole() const;

protected:

    void attachTexture
        ( base::Geometry& geometry
        , const base::VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment ) const;

}; // HUComponent


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
HUComponent< SegmentHUVolumeType, SegmentNormalsVolumeType >::HUComponent()
    : role( DEFAULT_ROLE_HU_VOLUME )
{
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
void HUComponent< SegmentHUVolumeType, SegmentNormalsVolumeType >::setHUVolumeRole( unsigned int role )
{
    this->role = role;
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
unsigned int HUComponent< SegmentHUVolumeType, SegmentNormalsVolumeType >::huVolumeRole() const
{
    return role;
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
void HUComponent< SegmentHUVolumeType, SegmentNormalsVolumeType >::attachTexture
    ( base::Geometry& geometry
    , const base::VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment ) const
{
    TextureManager< HUTextureFactory< SegmentHUVolumeType, SegmentNormalsVolumeType > >
        ::attachTexture( geometry, role, segment );
}



// ----------------------------------------------------------------------------------
// NormalsComponent< SegmentHUVolumeType, SegmentNormalsVolumeType >
// ----------------------------------------------------------------------------------

template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
class NormalsComponent : public TextureManager< NormalsTextureFactory< SegmentHUVolumeType, SegmentNormalsVolumeType > >
{

    unsigned int role;
    base::VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >* grid;

public:

    const static unsigned int DEFAULT_ROLE_NORMALS = 1;

    NormalsComponent();

    void setNormalsRole( unsigned int role );

    unsigned int normalsRole() const;

    void computeNormals();

protected:

    void setNormalsComponentGrid( base::VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >& grid );

    void attachTexture
        ( base::Geometry& geometry
        , const base::VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment ) const;

}; // NormalsComponent


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
NormalsComponent< SegmentHUVolumeType, SegmentNormalsVolumeType >::NormalsComponent()
    : role( DEFAULT_ROLE_NORMALS )
{
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
void NormalsComponent< SegmentHUVolumeType, SegmentNormalsVolumeType >::setNormalsRole( unsigned int role )
{
    this->role = role;
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
unsigned int NormalsComponent< SegmentHUVolumeType, SegmentNormalsVolumeType >::normalsRole() const
{
    return role;
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
void NormalsComponent< SegmentHUVolumeType, SegmentNormalsVolumeType >
    ::setNormalsComponentGrid( base::VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >& grid )
{
    this->grid = &grid;
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
void NormalsComponent< SegmentHUVolumeType, SegmentNormalsVolumeType >::attachTexture
    ( base::Geometry& geometry
    , const base::VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment ) const
{
    TextureManager< HUTextureFactory< SegmentHUVolumeType, SegmentNormalsVolumeType > >
        ::attachTexture( geometry, role, segment );
}



// ----------------------------------------------------------------------------------
// NormalsComponent< SegmentHUVolumeType, void >
// ----------------------------------------------------------------------------------

/** \brief
  * Specializes \ref NormalsComponent when no normals are desired.
  *
  * \author Leonid Kostrykin
  * \date   27.3.15
  */
template< typename SegmentHUVolumeType >
class NormalsComponent< SegmentHUVolumeType, void >
{

public:

    /** \brief
      * Does nothing.
      */
    void releaseGeometryFeatures();

protected:
    
    /** \brief
      * Does nothing.
      */
    void computeNormals();
    
    /** \brief
      * Does nothing.
      */
    void setNormalsComponentGrid( base::VolumeGrid< SegmentHUVolumeType, void >& grid );
    
    /** \brief
      * Does nothing.
      */
    void attachTexture
        ( base::Geometry& geometry
        , const base::VolumeSegment< SegmentHUVolumeType, void >& segment ) const;

}; // NormalsComponent


template< typename SegmentHUVolumeType >
void NormalsComponent< SegmentHUVolumeType, void >::releaseGeometryFeatures()
{
}


template< typename SegmentHUVolumeType >
void NormalsComponent< SegmentHUVolumeType, void >::computeNormals()
{
}


template< typename SegmentHUVolumeType >
void NormalsComponent< SegmentHUVolumeType, void >::setNormalsComponentGrid
    ( base::VolumeGrid< SegmentHUVolumeType, void >& grid )
{
}


template< typename SegmentHUVolumeType >
void NormalsComponent< SegmentHUVolumeType, void >::attachTexture
    ( base::Geometry& geometry
    , const base::VolumeSegment< SegmentHUVolumeType, void >& segment ) const
{
}



}  // namespace VolumeGridHelper

}  // namespace details

}  // namespace Carna :: helpers

}  // namespace Carna

#endif // VOLUMEGRIDHELPERDETAILS_H_6014714286
