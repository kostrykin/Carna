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
#include <Carna/base/BufferedNormalMap3D.h>
#include <Carna/base/Geometry.h>
#include <Carna/base/Log.h>
#include <Carna/base/Stopwatch.h>
#include <Carna/base/text.h>
#include <map>

/** \file   VolumeGridHelperDetails.h
  * \brief  Defines \ref Carna::helpers::details::VolumeGridHelper.
  */

namespace Carna
{

namespace helpers
{

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
  * Creates \ref base::ManagedTexture3D "textures" that represents
  * \ref base::VolumeSegment::huVolume in video memory.
  *
  * \author Leonid Kostrykin
  * \date   26.3.15 - 27.3.15
  */
template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
struct HUTextureFactory
{
    /** \brief
      * Reflects the type to use for storing the HU volume of a single partition.
      */
    typedef SegmentHUVolumeType SegmentHUVolume;

    /** \brief
      * Reflects the type to use for storing the normal map of a single partition.
      */
    typedef SegmentNormalsVolumeType SegmentNormalsVolume;

    /** \brief
      * Creates \ref base::ManagedTexture3D "texture" that represents the
      * \ref base::VolumeSegment::huVolume of \a segment in video memory.
      */
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
  * Creates \ref base::ManagedTexture3D "textures" that represents
  * \ref base::VolumeSegmentNormalsComponent::normals in video memory.
  *
  * \author Leonid Kostrykin
  * \date   26.3.15 - 27.3.15
  */
template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
struct NormalsTextureFactory
{
    /** \brief
      * Reflects the type to use for storing the HU volume of a single partition.
      */
    typedef SegmentHUVolumeType SegmentHUVolume;

    /** \brief
      * Reflects the type to use for storing the normal map of a single partition.
      */
    typedef SegmentNormalsVolumeType SegmentNormalsVolume;
    
    /** \brief
      * Creates \ref base::ManagedTexture3D "texture" that represents the
      * \ref base::VolumeSegmentNormalsComponent::normals of \a segment in video
      * memory.
      */
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

    /** \brief
      * \ref releaseGeometryFeatures "Releases all textures" and deletes.
      */
    virtual ~TextureManager();

    /** \brief
      * \ref Carna::base::GeometryFeature::release "Releases" all textures.
      */
    void releaseGeometryFeatures();

protected:

    /** \brief
      * Attaches the texture that `TextureFactory` creates from \a segment to
      * \a geometry using \a role.
      */
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

/** \brief
  * Defines the \ref helpers::VolumeGridHelper component that maintains HU volume
  * data.
  *
  * \author Leonid Kostrykin
  * \date   27.3.15 - 29.3.15
  */
template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
class HUComponent : public TextureManager< HUTextureFactory< SegmentHUVolumeType, SegmentNormalsVolumeType > >
{

    unsigned int role;

public:

    /** \brief
      * Holds the default \ref GeometryTypes "role" to use for
      * \ref attachTexture "attaching textures" to \ref base::Geometry nodes.
      */
    const static unsigned int DEFAULT_ROLE_HU_VOLUME = 0;

    /** \brief
      * Sets the \ref GeometryTypes "role" to use for
      * \ref attachTexture "attaching textures" to \ref base::Geometry nodes to
      * \ref DEFAULT_ROLE_HU_VOLUME.
      */
    HUComponent();

    /** \brief
      * Sets the \ref GeometryTypes "role" to use for
      * \ref attachTexture "attaching textures" to \ref base::Geometry nodes.
      */
    void setHUVolumeRole( unsigned int role );
    
    /** \brief
      * Tels the \ref GeometryTypes "role" used for
      * \ref attachTexture "attaching textures" to \ref base::Geometry nodes.
      */
    unsigned int huVolumeRole() const;

protected:

    /** \brief
      * Attaches the \ref base::ManagedTexture3D "texture" that represents the
      * \ref base::VolumeSegment::huVolume of \a segment to \a geometry using the
      * \ref setHUVolumeRole "previously configured role".
      */
    void attachTexture
        ( base::Geometry& geometry
        , const base::VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment ) const;

    /** \brief
      * Initializes \ref base::VolumeSegment::setHUVolume "HU volume" of \a segment.
      */
    void initializeSegment
        ( base::VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment
        , const base::math::Vector3ui& size ) const;

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


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
void HUComponent< SegmentHUVolumeType, SegmentNormalsVolumeType >::initializeSegment
    ( base::VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment
    , const base::math::Vector3ui& size ) const
{
    SegmentHUVolumeType* const huVolume = new SegmentHUVolumeType( size );
    segment.setHUVolume( new base::Composition< SegmentHUVolumeType >( huVolume ) );
}



// ----------------------------------------------------------------------------------
// NormalsComponentBase
// ----------------------------------------------------------------------------------

/** \brief
  * Defines the \ref NormalsComponent base class that is independent of type
  * arguments.
  *
  * \author Leonid Kostrykin
  * \date   27.3.15 - 30.3.15
  */
class CARNA_LIB NormalsComponentBase
{

public:

    /** \brief
      * Deletes.
      */
    virtual ~NormalsComponentBase();

protected:

    /** \brief
      * Tells the \ref VolumeGridHelperResolutions "effective grid resolution".
      */
    virtual base::math::Vector3ui gridResolution() const = 0;

}; // NormalsComponentBase



// ----------------------------------------------------------------------------------
// NormalsComponent< SegmentHUVolumeType, SegmentNormalsVolumeType >
// ----------------------------------------------------------------------------------

/** \brief
  * Defines the \ref helpers::VolumeGridHelper component that computes and maintains
  * normal maps.
  *
  * \author Leonid Kostrykin
  * \date   27.3.15 - 29.3.15
  */
template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
class NormalsComponent
    : public TextureManager< NormalsTextureFactory< SegmentHUVolumeType, SegmentNormalsVolumeType > >
    , public NormalsComponentBase
{

    unsigned int role;
    base::VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >* grid;

public:
    
    /** \brief
      * Holds the default \ref GeometryTypes "role" to use for
      * \ref attachTexture "attaching textures" to \ref base::Geometry nodes.
      */
    const static unsigned int DEFAULT_ROLE_NORMALS = 1;
    
    /** \brief
      * Sets the \ref GeometryTypes "role" to use for
      * \ref attachTexture "attaching textures" to \ref base::Geometry nodes to
      * \ref DEFAULT_ROLE_NORMALS.
      */
    NormalsComponent();
    
    /** \brief
      * Sets the \ref GeometryTypes "role" to use for
      * \ref attachTexture "attaching textures" to \ref base::Geometry nodes.
      */
    void setNormalsRole( unsigned int role );
    
    /** \brief
      * Tels the \ref GeometryTypes "role" used for
      * \ref attachTexture "attaching textures" to \ref base::Geometry nodes.
      */
    unsigned int normalsRole() const;

    /** \brief
      * Computes the normal map on the \ref setGrid "previously set grid".
      */
    void computeNormals();

protected:

    /** \brief
      * Sets the grid that \ref computeNormals operates on.
      */
    void setGrid( base::VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >& grid );
    
    /** \brief
      * Attaches the \ref base::ManagedTexture3D "texture" that represents the
      * \ref base::VolumeSegmentNormalsComponent::normals of \a segment to
      * \a geometry using the \ref setNormalsRole "previously configured role".
      */
    void attachTexture
        ( base::Geometry& geometry
        , const base::VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment ) const;
    
    /** \brief
      * Initializes \ref base::VolumeSegmentNormalsComponent::setNormals "normal map"
      * of \a segment.
      */
    void initializeSegment
        ( base::VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment
        , const base::math::Vector3ui& size ) const;

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
void NormalsComponent< SegmentHUVolumeType, SegmentNormalsVolumeType >::computeNormals()
{
    typedef typename base::VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::NormalSelector NormalSelector;
    typedef typename base::VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::   HUVSelector    HUVSelector;

    using base::math::Vector3ui;
    using base::math::Vector3i;
    using base::math::Vector3f;
    
    const base::Stopwatch stopwatch;
    const Vector3ui resolution = gridResolution();

    /* Lets start with the normals for the edge faces of the volume, that do require
     * an ad-hoc processing.
     */
    for( unsigned int dim0 = 0; dim0 < 3; ++dim0 )
    {
        for( signed int sign = -1; sign <= +1; sign += 2 )
        {
            /* Construct the normal vector along dimension 'dim0' signed with 'sign'.
             */
            Vector3f normal( 0, 0, 0 );
            normal( dim0 ) = sign;

            /* Write the normal vector to the whole face.
             */
            const unsigned int dim1 = ( dim0 + 1 ) % 3;
            const unsigned int dim2 = ( dim0 + 2 ) % 3;

            Vector3ui coord;
            coord( dim0 ) = sign < 0 ? 0 : resolution( dim0 ) - 1;

            for( coord( dim1 ) = 0; coord( dim1 ) < resolution( dim1 ); ++coord( dim1 ) )
            for( coord( dim2 ) = 0; coord( dim2 ) < resolution( dim2 ); ++coord( dim2 ) )
            {
                grid->template setVoxel< NormalSelector >( coord, normal );
            }
        }
    }

    /* Now we can process all the inner voxels regularly.
    */
    const Vector3ui coordLowerBound = Vector3ui( 1, 1, 1 );
    const Vector3ui coordUpperBound = ( resolution.cast< int >() - Vector3i( 1, 1, 1 ) ).cast< unsigned int >();
    CARNA_FOR_VECTOR3UI_EX( coord, coordUpperBound, coordLowerBound )
    {
        /* Sample the neighboring voxels.
         */
        const base::HUV huv_0yz = grid->template getVoxel< HUVSelector >( Vector3ui( coord.x() - 1, coord.y(), coord.z() ) );
        const base::HUV huv_1yz = grid->template getVoxel< HUVSelector >( Vector3ui( coord.x() + 1, coord.y(), coord.z() ) );
        const base::HUV huv_x0z = grid->template getVoxel< HUVSelector >( Vector3ui( coord.x(), coord.y() - 1, coord.z() ) );
        const base::HUV huv_x1z = grid->template getVoxel< HUVSelector >( Vector3ui( coord.x(), coord.y() + 1, coord.z() ) );
        const base::HUV huv_xy0 = grid->template getVoxel< HUVSelector >( Vector3ui( coord.x(), coord.y(), coord.z() - 1 ) );
        const base::HUV huv_xy1 = grid->template getVoxel< HUVSelector >( Vector3ui( coord.x(), coord.y(), coord.z() + 1 ) );

        /* Compute the normal vector and write the result. Note that the normal
         * vector points to the *inverse* direction of the gradient, i.e. away from
         * the steepest ascent.
         */
        Vector3f normal = Vector3f( huv_0yz - huv_1yz, huv_x0z - huv_x1z, huv_xy0 - huv_xy1 ) / 2;
        if( !base::math::isEqual< float >( normal.squaredNorm(), 0 ) )
        {
            normal.normalize();
        }
        else
        {
            normal = Vector3f( 0, 0, 0 );
        }
        grid->template setVoxel< NormalSelector >( coord, normal );
    }
    
    /* Log how long it took to compute the normals.
     */
    const unsigned int seconds = base::math::round_ui( stopwatch.result() );
    base::Log::instance().record( base::Log::verbose
        , "VolumeGridHelper finished normals computation in "
        + base::text::lexical_cast< std::string >( seconds )
        + " seconds." );
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
void NormalsComponent< SegmentHUVolumeType, SegmentNormalsVolumeType >
    ::setGrid( base::VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >& grid )
{
    this->grid = &grid;
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
void NormalsComponent< SegmentHUVolumeType, SegmentNormalsVolumeType >::attachTexture
    ( base::Geometry& geometry
    , const base::VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment ) const
{
    TextureManager< NormalsTextureFactory< SegmentHUVolumeType, SegmentNormalsVolumeType > >
        ::attachTexture( geometry, role, segment );
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
void NormalsComponent< SegmentHUVolumeType, SegmentNormalsVolumeType >::initializeSegment
    ( base::VolumeSegment< SegmentHUVolumeType, SegmentNormalsVolumeType >& segment
    , const base::math::Vector3ui& size ) const
{
    SegmentNormalsVolumeType* const normals = new SegmentNormalsVolumeType( size );
    segment.setNormals( new base::Composition< SegmentNormalsVolumeType >( normals ) );
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
class NormalsComponent< SegmentHUVolumeType, void > : public NormalsComponentBase
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
    void setGrid( base::VolumeGrid< SegmentHUVolumeType, void >& grid );
    
    /** \brief
      * Does nothing.
      */
    void attachTexture
        ( base::Geometry& geometry
        , const base::VolumeSegment< SegmentHUVolumeType, void >& segment ) const;
    
    /** \brief
      * Does nothing.
      */
    void initializeSegment
        ( base::VolumeSegment< SegmentHUVolumeType, void >& segment
        , const base::math::Vector3ui& size ) const;

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
void NormalsComponent< SegmentHUVolumeType, void >::setGrid
    ( base::VolumeGrid< SegmentHUVolumeType, void >& grid )
{
}


template< typename SegmentHUVolumeType >
void NormalsComponent< SegmentHUVolumeType, void >::attachTexture
    ( base::Geometry& geometry
    , const base::VolumeSegment< SegmentHUVolumeType, void >& segment ) const
{
}


template< typename SegmentHUVolumeType >
void NormalsComponent< SegmentHUVolumeType, void >::initializeSegment
    ( base::VolumeSegment< SegmentHUVolumeType, void >& segment
    , const base::math::Vector3ui& size ) const
{
}



}  // namespace VolumeGridHelper

}  // namespace details

}  // namespace Carna :: helpers

}  // namespace Carna

#endif // VOLUMEGRIDHELPERDETAILS_H_6014714286
