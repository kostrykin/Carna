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
// IntensityTextureFactory< SegmentIntensityVolumeType, SegmentNormalsVolumeType >
// ----------------------------------------------------------------------------------

/** \brief
  * Creates \ref base::ManagedTexture3D "textures" that represents
  * \ref base::VolumeSegment::intensities in video memory.
  *
  * \author Leonid Kostrykin
  * \date   26.3.15 - 27.3.15
  */
template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
struct IntensityTextureFactory
{
    /** \brief
      * Reflects the type to use for storing the intensity volume of a single partition.
      */
    typedef SegmentIntensityVolumeType SegmentIntensityVolume;

    /** \brief
      * Reflects the type to use for storing the normal map of a single partition.
      */
    typedef SegmentNormalsVolumeType SegmentNormalsVolume;

    /** \brief
      * Creates \ref base::ManagedTexture3D "texture" that represents the
      * \ref base::VolumeSegment::intensities of \a segment in video memory.
      */
    static base::ManagedTexture3D& createTexture
        ( const base::VolumeSegment< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& segment );
};


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
base::ManagedTexture3D& IntensityTextureFactory< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::createTexture
    ( const base::VolumeSegment< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& segment )
{
    return base::BufferedVectorFieldTexture< SegmentIntensityVolumeType >::create( segment.intensities() );
}



// ----------------------------------------------------------------------------------
// NormalsTextureFactory< SegmentIntensityVolumeType, SegmentNormalsVolumeType >
// ----------------------------------------------------------------------------------

/** \brief
  * Creates \ref base::ManagedTexture3D "textures" that represents
  * \ref base::VolumeSegmentNormalsComponent::normals in video memory.
  *
  * \author Leonid Kostrykin
  * \date   26.3.15 - 27.3.15
  */
template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
struct NormalsTextureFactory
{
    /** \brief
      * Reflects the type to use for storing the intensity volume of a single partition.
      */
    typedef SegmentIntensityVolumeType SegmentIntensityVolume;

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
        ( const base::VolumeSegment< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& segment );
};


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
base::ManagedTexture3D& NormalsTextureFactory< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::createTexture
    ( const base::VolumeSegment< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& segment )
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
            < typename TextureFactory::SegmentIntensityVolume
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
            < typename TextureFactory::SegmentIntensityVolume
            , typename TextureFactory::SegmentNormalsVolume >& segment ) const;

private:

    base::ManagedTexture3D& getTexture
        ( const base::VolumeSegment
            < typename TextureFactory::SegmentIntensityVolume
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
        < typename TextureFactory::SegmentIntensityVolume
        , typename TextureFactory::SegmentNormalsVolume >& segment ) const
{
    base::ManagedTexture3D& texture = getTexture( segment );
    geometry.putFeature( role, texture );
}


template< typename TextureFactory >
base::ManagedTexture3D& TextureManager< TextureFactory >::getTexture
    ( const base::VolumeSegment
        < typename TextureFactory::SegmentIntensityVolume
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
// IntensityComponent< SegmentIntensityVolumeType, SegmentNormalsVolumeType >
// ----------------------------------------------------------------------------------

/** \brief
  * Defines the \ref helpers::VolumeGridHelper component that maintains intensity
  * volume data.
  *
  * \author Leonid Kostrykin
  * \date   27.3.15 - 29.3.15
  */
template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
class IntensityComponent : public TextureManager< IntensityTextureFactory< SegmentIntensityVolumeType, SegmentNormalsVolumeType > >
{

    unsigned int role;

public:

    /** \brief
      * Holds the default \ref GeometryTypes "role" to use for
      * \ref attachTexture "attaching textures" to \ref base::Geometry nodes.
      */
    const static unsigned int DEFAULT_ROLE_INTENSITIES = 0;

    /** \brief
      * Sets the \ref GeometryTypes "role" to use for
      * \ref attachTexture "attaching textures" to \ref base::Geometry nodes to
      * \ref DEFAULT_ROLE_INTENSITIES.
      */
    IntensityComponent();

    /** \brief
      * Sets the \ref GeometryTypes "role" to use for
      * \ref attachTexture "attaching textures" to \ref base::Geometry nodes.
      */
    void setIntensitiesRole( unsigned int role );
    
    /** \brief
      * Tels the \ref GeometryTypes "role" used for
      * \ref attachTexture "attaching textures" to \ref base::Geometry nodes.
      */
    unsigned int intensitiesRole() const;

protected:

    /** \brief
      * Attaches the \ref base::ManagedTexture3D "texture" that represents the
      * \ref base::VolumeSegment::intensities of \a segment to \a geometry
      * using the \ref setIntensitiesRole "previously configured role".
      */
    void attachTexture
        ( base::Geometry& geometry
        , const base::VolumeSegment< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& segment ) const;

    /** \brief
      * Initializes \ref base::VolumeSegment::setIntensities "intensity volume" of \a segment.
      */
    void initializeSegment
        ( base::VolumeSegment< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& segment
        , const base::math::Vector3ui& size ) const;

}; // IntensityComponent


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
IntensityComponent< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::IntensityComponent()
    : role( DEFAULT_ROLE_INTENSITIES )
{
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
void IntensityComponent< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::setIntensitiesRole( unsigned int role )
{
    this->role = role;
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
unsigned int IntensityComponent< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::intensitiesRole() const
{
    return role;
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
void IntensityComponent< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::attachTexture
    ( base::Geometry& geometry
    , const base::VolumeSegment< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& segment ) const
{
    TextureManager< IntensityTextureFactory< SegmentIntensityVolumeType, SegmentNormalsVolumeType > >
        ::attachTexture( geometry, role, segment );
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
void IntensityComponent< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::initializeSegment
    ( base::VolumeSegment< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& segment
    , const base::math::Vector3ui& size ) const
{
    SegmentIntensityVolumeType* const intensities = new SegmentIntensityVolumeType( size );
    segment.setIntensities( new base::Composition< SegmentIntensityVolumeType >( intensities ) );
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
// NormalsComponent< SegmentIntensityVolumeType, SegmentNormalsVolumeType >
// ----------------------------------------------------------------------------------

/** \brief
  * Defines the \ref helpers::VolumeGridHelper component that computes and maintains
  * normal maps.
  *
  * \author Leonid Kostrykin
  * \date   27.3.15 - 29.3.15
  */
template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
class NormalsComponent
    : public TextureManager< NormalsTextureFactory< SegmentIntensityVolumeType, SegmentNormalsVolumeType > >
    , public NormalsComponentBase
{

    unsigned int role;
    base::VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >* grid;

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
    void setGrid( base::VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& grid );
    
    /** \brief
      * Attaches the \ref base::ManagedTexture3D "texture" that represents the
      * \ref base::VolumeSegmentNormalsComponent::normals of \a segment to
      * \a geometry using the \ref setNormalsRole "previously configured role".
      */
    void attachTexture
        ( base::Geometry& geometry
        , const base::VolumeSegment< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& segment ) const;
    
    /** \brief
      * Initializes \ref base::VolumeSegmentNormalsComponent::setNormals "normal map"
      * of \a segment.
      */
    void initializeSegment
        ( base::VolumeSegment< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& segment
        , const base::math::Vector3ui& size ) const;

}; // NormalsComponent


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
NormalsComponent< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::NormalsComponent()
    : role( DEFAULT_ROLE_NORMALS )
{
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
void NormalsComponent< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::setNormalsRole( unsigned int role )
{
    this->role = role;
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
unsigned int NormalsComponent< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::normalsRole() const
{
    return role;
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
void NormalsComponent< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::computeNormals()
{
    typedef typename base::VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::   NormalSelector    NormalSelector;
    typedef typename base::VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::IntensitySelector IntensitySelector;

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
        /* Compute the gradient vector.
         */
        const Vector3f gradient = base::math::computeFastGradient3f(
            [this, &coord]( unsigned int dx, unsigned int dy, unsigned int dz )
            {
                return grid->template getVoxel< IntensitySelector >( Vector3ui( coord.x() + dx, coord.y() + dy, coord.z() + dz ) );
            }
        );

        /* The normal vector points to the *reverse* direction of the gradient, i.e.
         * away from the steepest ascent.
         */
        Vector3f normal = -gradient;
        if( normal.squaredNorm() > 1e-12 )
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


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
void NormalsComponent< SegmentIntensityVolumeType, SegmentNormalsVolumeType >
    ::setGrid( base::VolumeGrid< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& grid )
{
    this->grid = &grid;
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
void NormalsComponent< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::attachTexture
    ( base::Geometry& geometry
    , const base::VolumeSegment< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& segment ) const
{
    TextureManager< NormalsTextureFactory< SegmentIntensityVolumeType, SegmentNormalsVolumeType > >
        ::attachTexture( geometry, role, segment );
}


template< typename SegmentIntensityVolumeType, typename SegmentNormalsVolumeType >
void NormalsComponent< SegmentIntensityVolumeType, SegmentNormalsVolumeType >::initializeSegment
    ( base::VolumeSegment< SegmentIntensityVolumeType, SegmentNormalsVolumeType >& segment
    , const base::math::Vector3ui& size ) const
{
    SegmentNormalsVolumeType* const normals = new SegmentNormalsVolumeType( size );
    segment.setNormals( new base::Composition< SegmentNormalsVolumeType >( normals ) );
}



// ----------------------------------------------------------------------------------
// NormalsComponent< SegmentIntensityVolumeType, void >
// ----------------------------------------------------------------------------------

/** \brief
  * Specializes \ref NormalsComponent when no normals are required.
  *
  * \author Leonid Kostrykin
  * \date   27.3.15
  */
template< typename SegmentIntensityVolumeType >
class NormalsComponent< SegmentIntensityVolumeType, void > : public NormalsComponentBase
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
    void setGrid( base::VolumeGrid< SegmentIntensityVolumeType, void >& grid );
    
    /** \brief
      * Does nothing.
      */
    void attachTexture
        ( base::Geometry& geometry
        , const base::VolumeSegment< SegmentIntensityVolumeType, void >& segment ) const;
    
    /** \brief
      * Does nothing.
      */
    void initializeSegment
        ( base::VolumeSegment< SegmentIntensityVolumeType, void >& segment
        , const base::math::Vector3ui& size ) const;

}; // NormalsComponent


template< typename SegmentIntensityVolumeType >
void NormalsComponent< SegmentIntensityVolumeType, void >::releaseGeometryFeatures()
{
}


template< typename SegmentIntensityVolumeType >
void NormalsComponent< SegmentIntensityVolumeType, void >::computeNormals()
{
}


template< typename SegmentIntensityVolumeType >
void NormalsComponent< SegmentIntensityVolumeType, void >::setGrid
    ( base::VolumeGrid< SegmentIntensityVolumeType, void >& grid )
{
}


template< typename SegmentIntensityVolumeType >
void NormalsComponent< SegmentIntensityVolumeType, void >::attachTexture
    ( base::Geometry& geometry
    , const base::VolumeSegment< SegmentIntensityVolumeType, void >& segment ) const
{
}


template< typename SegmentIntensityVolumeType >
void NormalsComponent< SegmentIntensityVolumeType, void >::initializeSegment
    ( base::VolumeSegment< SegmentIntensityVolumeType, void >& segment
    , const base::math::Vector3ui& size ) const
{
}



// ----------------------------------------------------------------------------------
// Partionining
// ----------------------------------------------------------------------------------

/** \brief
  * Computes the partitioning that \ref VolumeGridHelper uses along one dimension.
  *
  * \author Leonid Kostrykin
  * \date   13.4.15
  */
struct Partionining
{
    /** \brief
      * Computes the partitioning.
      *
      * \param nativeSize is the resolution of the actual payload data.
      * \param regularPartitionSize is the effective resolution of a single regular
      *     partition. It must be odd.
      */
    Partionining( std::size_t nativeSize, std::size_t regularPartitionSize )
        : regularPartitionSize( regularPartitionSize )
        , regularPartitionsCount( nativeSize / regularPartitionSize )
        , tailSize( base::math::makeEven( nativeSize % regularPartitionSize, +1 ) )
    {
        CARNA_ASSERT_EX( regularPartitionSize % 2 == 1, "Effective regular partition size must be odd!" );
    }

    /** \brief
      * Holds the always odd, effective resolution of a single regular partition.
      */
    std::size_t regularPartitionSize;

    /** \brief
      * Holds the number of regular partitions, i.e. such of the size held by
      * \ref regularPartitionSize.
      */
    std::size_t regularPartitionsCount;

    /** \brief
      * Holds the resolution of the last partition that may also be `0`.
      */
    std::size_t tailSize;

    /** \brief
      * Computes the effective total resolution.
      */
    std::size_t totalSize() const
    {
        return tailSize + regularPartitionsCount * regularPartitionSize;
    }

    /** \brief
      * Tells the total partitions number.
      */
    std::size_t partitionsCount() const
    {
        return regularPartitionsCount + ( tailSize > 0 ? 1 : 0 );
    }
};



}  // namespace VolumeGridHelper

}  // namespace details

}  // namespace Carna :: helpers

}  // namespace Carna

#endif // VOLUMEGRIDHELPERDETAILS_H_6014714286
