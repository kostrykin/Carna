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

#ifndef BUFFEREDHUVOLUME_H_6014714286
#define BUFFEREDHUVOLUME_H_6014714286

/** \file   BufferedHUVolume.h
  *
  * \brief  Defines \ref Carna::base::BufferedHUVolume.
  *
  * \author Leonid Kostrykin
  * \date   25.7.11
  */

#include <Carna/base/HUVolume.h>
#include <Carna/base/Composition.h>
#include <Carna/base/CarnaException.h>
#include <vector>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BufferedHUVolume
// ----------------------------------------------------------------------------------

/** \brief  Generic \ref HUVolume implementation.
  *
  * \invariant  <code>sizeof( VoxelType ) >= 2</code>
  *
  * Template Arguments:
  *
  * - The \a VoxelType is the data type used to store the value of a single voxel / HU-value.
  * - The \a BufferType is the data type used as voxel container.
  *
  * \author Leonid Kostrykin
  * \date   2011 - 2015
  */
template< typename VoxelType, typename BufferType >
class BufferedHUVolume : public HUVolume
{

public:

    /** \brief  Holds the used buffer type.
      */
    typedef BufferType Buffer;

    /** \brief  Holds the used voxel type.
      */
    typedef VoxelType Voxel;

    /** \brief  Instantiates.
      */
    BufferedHUVolume( const math::Vector3ui& size, Association< BufferType >* buffer )
        : HUVolume( size )
        , myBuffer( buffer )
    {
        initializeBuffer();
    }

    /** \brief  Instantiates.
      *
      * This constructor is similar to:
      *
      * \code
      * BufferedHUVolume( size, new Composition< BufferType >( new BufferType( size.x * size.y * size.z ) ) );
      * \endcode
      */
    explicit BufferedHUVolume( const math::Vector3ui& size )
        : HUVolume( size )
        , myBuffer( new Composition< BufferType >( new BufferType( size.x() * size.y() * size.z() ) ) )
    {
        initializeBuffer();
    }

    /** \brief
      * Returns the buffer value corresponding to the given HU value.
      */
    static HUV bufferValueToHUV( VoxelType voxel_value )
    {
        return static_cast< HUV >( voxel_value >> ( sizeof( VoxelType ) * 8 - 12 ) ) - 1024;
    }
    
    /** \brief
      * Returns the HU value corresponding to the given buffer value.
      */
    static VoxelType HUVToBufferValue( HUV huv )
    {
        return ( static_cast< VoxelType >( huv + 1024 ) << ( sizeof( VoxelType ) * 8 - 12 ) );
    }

    /** \brief
      * Returns HUV of specified voxel.
      */
    HUV operator()( unsigned int x
                  , unsigned int y
                  , unsigned int z ) const
    {
        const std::size_t index = x + size.x() * y + size.y() * size.x() * z;
        return bufferValueToHUV( myBuffer->get()->at( index ) );
    }

    /** \brief
      * Returns HUV of specified voxel.
      */
    HUV operator()( const math::Vector3ui& at ) const
    {
        return ( *this )( at.x(), at.y(), at.z() );
    }

    /** \brief
      * Sets the HUV of a voxel.
      */
    void setVoxel( unsigned int x, unsigned int y, unsigned int z, HUV huv )
    {
        CARNA_ASSERT( x < size.x() && y < size.y() && z < size.z() );
        const std::size_t index = x + size.x() * y + size.y() * size.x() * z;
        myBuffer->get()->at( index ) = HUVToBufferValue( huv );
    }

    /** \brief
      * Sets the HUV of a voxel.
      */
    void setVoxel( const math::Vector3ui& at, HUV huv )
    {
        this->setVoxel( at.x(), at.y(), at.z(), huv );
    }

    /** \brief
      * References the underlying buffer.
      */
    BufferType& buffer()
    {
        return **myBuffer;
    }
    
    /** \brief
      * References the underlying buffer.
      */
    const BufferType& buffer() const
    {
        return **myBuffer;
    }

protected:

    /** \brief
      * Holds the computed data.
      *
      * The voxels are written \f$x\f$-\f$y\f$-plane wise, each plane \f$x\f$-row wise.
      *
      * To compute the position of some voxel \f$\big(\; x, y, z \;\big)\f$ in \a data:
      *
      * \f[ \mathrm{position} = x + \mathrm{width} \cdot y + \mathrm{height} \cdot \mathrm{width} \cdot z \f]
      */
    const std::unique_ptr< Association< BufferType > > myBuffer;

private:

    void initializeBuffer()
    {
        CARNA_ASSERT_EX
            ( myBuffer.get() && myBuffer->get()
            , "no volume data buffer supplied" );

        CARNA_ASSERT_EX
            ( myBuffer->get()->size() >= size.x() * size.y() * size.z()
            , "supplied volume data buffer is of size "
                << myBuffer->get()->size()
                << " bytes but must be at least "
                << size.x() * size.y() * size.z()
                << " bytes" );
    }

}; // BufferedHUVolume



}  // namespace Carna :: base

}  // namespace Carna

#endif // BUFFEREDHUVOLUME_H_6014714286
