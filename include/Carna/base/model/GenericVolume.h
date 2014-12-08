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

#ifndef GENERICVOLUME_H_6014714286
#define GENERICVOLUME_H_6014714286

/** \file   GenericVolume.h
  *
  * \brief  Defines \ref Carna::base::model::GenericVolume.
  *
  * \author Leonid Kostrykin
  * \date   25.7.11
  */

#include <Carna/base/model/Volume.h>
#include <Carna/base/Composition.h>
#include <Carna/base/CarnaException.h>
#include <vector>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// GenericVolume
// ----------------------------------------------------------------------------------

/** \brief  Generic \ref Volume implementation.
  *
  * \invariant  <code>sizeof( VoxelType ) >= 2</code>
  *
  * <b>Template Arguments:</b>
  *
  * - The \a VoxelType is the data type used to store the value of a single voxel / HU-value.
  * - The \a TextureUploader is a class, which meets the following requirements:
  *   - It has a public default constructor.
  *   - It implements a public method with the interface:<br>
  *     <code>uploadTexture( const Vector3ui& size, const void* bufferPtr ) const</code><br>
  *     This method satisfies the requirements documented \ref Volume::uploadTexture "here".
  * - The \a BufferType is the data type used as voxel container.
  *
  * \author Leonid Kostrykin
  * \date   2011 - 2014
  */
template< typename VoxelType
        , typename TextureUploader
        , typename BufferType = std::vector< VoxelType > >
class GenericVolume : public Volume
{

public:

    /** \brief  Holds the used buffer type.
      */
    typedef BufferType BufferType;

    /** \brief  Holds the used voxel type.
      */
    typedef VoxelType VoxelType;

    /** \brief  Instantiates.
      */
    GenericVolume( const Vector3ui& size, Association< BufferType >* buffer )
        : Volume( size )
        , buffer( buffer )
    {
        init();
    }

    /** \brief  Instantiates.
      *
      * This constructor is similar to:
      *
      * \code
      * GenericVolume( size, new Composition< BufferType >( new BufferType( size.x * size.y * size.z ) ) );
      * \endcode
      */
    explicit GenericVolume( const Vector3ui& size )
        : Volume( size )
        , buffer( new Composition< BufferType >( new BufferType( size.x * size.y * size.z ) ) )
    {
        init();
    }


    /** \brief  Returns the buffer value corresponding to the given HU value.
      *
      * \since  \ref v_2_4
      */
    static signed short bufferValueToHUV( VoxelType voxel_value )
    {
        return static_cast< signed short >( voxel_value >> ( sizeof( VoxelType ) * 8 - 12 ) ) - 1024;
    }

    
    /** \brief  Returns the HU value corresponding to the given buffer value.
      *
      * \since  \ref v_2_4
      */
    static VoxelType HUVToBufferValue( signed short huv )
    {
        return ( static_cast< VoxelType >( huv + 1024 ) << ( sizeof( VoxelType ) * 8 - 12 ) );
    }


    /** \brief  Returns HUV of specified voxel.
      */
    signed short operator()( unsigned int x
                           , unsigned int y
                           , unsigned int z ) const
    {
        return bufferValueToHUV( buffer->get()->at( x + size.x * y + size.y * size.x * z ) );
    }

    /** \brief  Returns HUV of specified voxel.
      */
    signed short operator()( const Vector3ui& at ) const
    {
        return ( *this )( at.x, at.y, at.z );
    }


    /** \brief  Sets the HUV of a voxel.
      */
    void setVoxel( unsigned int x, unsigned int y, unsigned int z, signed short huv )
    {
        buffer->get()->at( x + size.x * y + size.y * size.x * z ) = HUVToBufferValue( huv );
    }

    /** \brief  Sets the HUV of a voxel.
      */
    void setVoxel( const Vector3ui& at, signed short huv )
    {
        this->setVoxel( at.x, at.y, at.z, huv );
    }


    /** \brief  References the underlying buffer.
      *
      * This method wasn't working prior to \ref v_2_2_2.
      */
    BufferType& getBuffer()
    {
        return *buffer->get();
    }
    
    /** \brief  References the underlying buffer.
      *
      * This method wasn't working prior to \ref v_2_2_2.
      */
    const BufferType& getBuffer() const
    {
        return *buffer->get();
    }
    

    virtual void uploadTexture() const
    {
        textureUploader.uploadTexture( size, &( buffer->get()->front() ) );
    }


protected:

    /** \brief  Holds the computed data.
      *
      * The voxels are written \f$x\f$-\f$y\f$-plane wise, each plane \f$x\f$-row wise.
      *
      * To compute the position of some voxel \f$\big(\; x, y, z \;\big)\f$ in \a data:
      *
      * \f[ \mathrm{position} = x + \mathrm{width} \cdot y + \mathrm{height} \cdot \mathrm{width} \cdot z \f]
      */
    const std::unique_ptr< Association< BufferType > > buffer;

    /** \brief  Uploads texture buffer to video memory.
      */
    TextureUploader textureUploader;


private:

    void init()
    {
        CARNA_ASSERT_EX
            ( buffer.get() && buffer->get()
            , "no volume data buffer supplied" );

        CARNA_ASSERT_EX
            ( buffer->get()->size() >= size.x * size.y * size.z
            , "supplied volume data buffer is of size "
                << buffer->get()->size()
                << " bytes but must be at least "
                << size.x * size.y * size.z
                << " bytes" );
    }

}; // GenericVolume



}  // namesapce Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // GENERICVOLUME_H_6014714286
