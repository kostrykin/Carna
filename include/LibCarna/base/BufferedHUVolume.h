/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#ifndef BUFFEREDHUVOLUME_H_6014714286
#define BUFFEREDHUVOLUME_H_6014714286

/** \file   BufferedHUVolume.h
  * \brief  Defines \ref LibCarna::base::BufferedHUVolume.
  */

#include <LibCarna/base/HUVolume.h>
#include <LibCarna/base/Composition.h>
#include <LibCarna/base/LibCarnaException.h>
#include <vector>
#include <memory>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BufferedHUVolume
// ----------------------------------------------------------------------------------

/** \brief
  * Implements \ref HUVolume generically for a particular \a VoxelType.
  *
  * \invariant `sizeof(VoxelType) >= 2`
  *
  * \param VoxelType is the data type used to store the value of a single voxel.
  * \param BufferType is the data type used as voxel container.
  *
  * \author Leonid Kostrykin
  */
template< typename VoxelType, typename BufferType >
class BufferedHUVolume : public HUVolume
{

public:

    /** \brief
      * Holds the used buffer type.
      */
    typedef BufferType Buffer;

    /** \brief
      * Holds the type used to store the value of a single voxel.
      */
    typedef VoxelType Voxel;

    /** \brief
      * Instantiates \f$D \to \left[-1024, 3071\right]\f$ with
      * \f$D = [0, s_x) \times [0, s_y) \times [0, s_z)\f$, where \f$s_x, s_y, s_z\f$
      * is \a size.
      *
      * \pre `buffer != nullptr && buffer->get() != nullptr`
      * \pre `(**buffer).size() >= size.x * size.y * size.z`
      */
    BufferedHUVolume( const math::Vector3ui& size, Association< BufferType >* buffer )
        : HUVolume( size )
        , myBuffer( buffer )
    {
        initializeBuffer();
    }

    /** \overload
      */
    explicit BufferedHUVolume( const math::Vector3ui& size )
        : HUVolume( size )
        , myBuffer( new Composition< BufferType >( new BufferType( size.x() * size.y() * size.z() ) ) )
    {
        initializeBuffer();
    }

    /** \brief
      * Returns the HU value corresponding to \a bufferValue.
      */
    static HUV bufferValueToHUV( VoxelType bufferValue )
    {
        return HUV::abs( ( bufferValue >> ( sizeof( VoxelType ) * 8 - 12 ) ) - 1024 );
    }

    /** \brief
      * Returns the buffer value corresponding to \a huValue.
      */
    static VoxelType HUVToBufferValue( HUV huValue )
    {
        return ( static_cast< VoxelType >( huValue + 1024 ) << ( sizeof( VoxelType ) * 8 - 12 ) );
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
        LIBCARNA_ASSERT( x < size.x() && y < size.y() && z < size.z() );
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
      * Holds the underlying buffer.
      *
      * The voxels are written \f$x\f$-\f$y\f$-plane wise, each plane \f$x\f$-row
      * wise. To compute the index of some voxel \f$\left(x, y, z\right)\f$, use the
      * following computation rule:
      *
      * \f[ x + \mathrm{width} \cdot y + \mathrm{height} \cdot \mathrm{width} \cdot z \f]
      */
    const std::unique_ptr< Association< BufferType > > myBuffer;

private:

    void initializeBuffer()
    {
        LIBCARNA_ASSERT_EX
            ( myBuffer.get() && myBuffer->get()
            , "No volume data buffer supplied!" );

        LIBCARNA_ASSERT_EX
            ( myBuffer->get()->size() >= size.x() * size.y() * size.z()
            , "Supplied volume data buffer is of size "
                << myBuffer->get()->size()
                << " bytes but must be at least "
                << size.x() * size.y() * size.z()
                << " bytes!" );
    }

}; // BufferedHUVolume



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // BUFFEREDHUVOLUME_H_6014714286
