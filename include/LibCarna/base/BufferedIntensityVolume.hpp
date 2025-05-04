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

#ifndef BUFFEREDINTENSITYVOLUME_H_6014714286
#define BUFFEREDINTENSITYVOLUME_H_6014714286

/** \file
  * \brief
  * Defines \ref LibCarna::base::BufferedIntensityVolume.
  */

#include <LibCarna/base/IntensityVolume.hpp>
#include <LibCarna/base/Composition.hpp>
#include <LibCarna/base/LibCarnaException.hpp>
#include <vector>
#include <memory>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BufferedIntensityVolume
// ----------------------------------------------------------------------------------

/** \brief
  * Implements \ref IntensityVolume generically for a particular \a VoxelType.
  *
  * \param VoxelType is the data type used to store the value of a single voxel.
  * \param BufferType is the data type used as voxel container.
  *
  * \author Leonid Kostrykin
  * \since  \ref v_3_2_0
  */
template< typename VoxelType, typename BufferType >
class BufferedIntensityVolume : public IntensityVolume
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
      * Instantiates \f$D \to \left[0, 1\right]\f$ with
      * \f$D = [0, s_x) \times [0, s_y) \times [0, s_z)\f$, where \f$s_x, s_y, s_z\f$
      * is \a size.
      *
      * \pre `buffer != nullptr && buffer->get() != nullptr`
      * \pre `(**buffer).size() >= size.x * size.y * size.z`
      */
    BufferedIntensityVolume( const math::Vector3ui& size, Association< BufferType >* buffer )
        : IntensityVolume( size )
        , myBuffer( buffer )
    {
        initializeBuffer();
    }

    /** \overload
      */
    explicit BufferedIntensityVolume( const math::Vector3ui& size )
        : IntensityVolume( size )
        , myBuffer( new Composition< BufferType >( new BufferType( size.x() * size.y() * size.z() ) ) )
    {
        initializeBuffer();
    }

    /** \brief
      * Returns the intensity value corresponding to \a bufferValue.
      */
    static float bufferValueToIntensity( VoxelType bufferValue )
    {
        return bufferValue / static_cast< float >( ( 1 << ( 8 * sizeof( VoxelType ) ) ) - 1 );
    }

    /** \brief
      * Returns the buffer value corresponding to an \a intensity.
      */
    static VoxelType intensityToBufferValue( float intensity )
    {
        LIBCARNA_ASSERT_EX( 0 <= intensity && intensity <= 1, "Intensity out of range: " << intensity ); // TODO: check how much this slows down?
        const auto valueMax = ( 1 << ( 8 * sizeof( VoxelType ) ) ) - 1;
        float value = intensity * valueMax + 0.5f;
        if( value > valueMax ) value = valueMax;
        return static_cast< VoxelType >( value );
    }

    /** \brief
      * Returns intensity of specified voxel.
      */
    float operator()( unsigned int x
                    , unsigned int y
                    , unsigned int z ) const
    {
        const std::size_t index = x + size.x() * y + size.y() * size.x() * z;
        return bufferValueToIntensity( myBuffer->get()->at( index ) );
    }

    /** \overload
      */
    float operator()( const math::Vector3ui& at ) const
    {
        return ( *this )( at.x(), at.y(), at.z() );
    }

    /** \brief
      * Sets the HUV of a voxel.
      */
    void setVoxel( unsigned int x, unsigned int y, unsigned int z, float intensity )
    {
        LIBCARNA_ASSERT( x < size.x() && y < size.y() && z < size.z() );
        const std::size_t index = x + size.x() * y + size.y() * size.x() * z;
        myBuffer->get()->at( index ) = intensityToBufferValue( intensity );
    }

    /** \overload
      */
    void setVoxel( const math::Vector3ui& at, float intensity )
    {
        this->setVoxel( at.x(), at.y(), at.z(), intensity );
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

}; // BufferedIntensityVolume



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // BUFFEREDINTENSITYVOLUME_H_6014714286
