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

#ifndef BUFFEREDNORMALMAP3D_H_6014714286
#define BUFFEREDNORMALMAP3D_H_6014714286

/** \file   BufferedNormalMap3D.h
  * \brief  Defines \ref LibCarna::base::BufferedNormalMap3D.
  */

#include <LibCarna/base/NormalMap3D.h>
#include <LibCarna/base/Composition.h>
#include <LibCarna/base/LibCarnaException.h>
#include <LibCarna/base/text.h>
#include <vector>
#include <memory>
#include <type_traits>
#include <climits>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BufferedNormalMap3D
// ----------------------------------------------------------------------------------

/** \brief
  * Implements \ref NormalMap3D generically for a particular \a VoxelType.
  *
  * \param BufferedVectorComponentType is the data type used to store the components
  *     of a single normal. Only integral data types are allowed.
  *
  * \param BufferType is the data type used as container for the normal vectors.
  *
  * \note
  * Although three channels would be sufficient to store the three components of a
  * normal vector, we use four channels here. The reasons for this is that
  * `glTexImage3d` caused read access errors on Windows when used with three channel
  * textures, whereas it worked nicely on Linux. This class ignores the values stored
  * within the fourth channel.
  *
  * \author Leonid Kostrykin
  */
template< typename BufferedVectorComponentType, typename BufferType >
class BufferedNormalMap3D : public NormalMap3D
{

    static_assert
        ( std::is_integral< BufferedVectorComponentType >::value
        , "Only integral buffer vector component types allowed." );

public:

    /** \brief
      * Holds the used buffer type.
      */
    typedef BufferType Buffer;

    /** \brief
      * Holds the type used to store the components of the normal vectors.
      */
    typedef BufferedVectorComponentType BufferedVectorComponent;

    /** \brief
      * Instantiates.
      *
      * \pre `buffer != nullptr && buffer->get() != nullptr`
      * \pre `(**buffer).size() >= 4 * sizeof(BufferedVectorComponentType) * size.x * size.y * size.z`
      */
    BufferedNormalMap3D( const math::Vector3ui& size, Association< BufferType >* buffer )
        : NormalMap3D( size )
        , myBuffer( buffer )
    {
        checkBuffer();
    }

    /** \overload
      */
    explicit BufferedNormalMap3D( const math::Vector3ui& size )
        : NormalMap3D( size )
        , myBuffer( new Composition< BufferType >
            ( new BufferType( 4 * sizeof( BufferedVectorComponentType ) * size.x() * size.y() * size.z() ) ) )
    {
        checkBuffer();
    }

    /** \brief
      * Returns the actual normal vector component corresponding to
      * \a encodedVectorComponent.
      *
      * This function maps \a encodedVectorComponent to \f$\left[-1, 1\right]\f$,
      * whereby the minimum of `BufferedVectorComponentType` is mapped to \f$-1\f$
      * and its maximum to \f$+1\f$.
      */
    static float decodeComponent( BufferedVectorComponentType encodedVectorComponent )
    {
        const float range = static_cast< float >( static_cast< signed long >( std::numeric_limits< BufferedVectorComponentType >::max() )
            - static_cast< signed long >( std::numeric_limits< BufferedVectorComponentType >::min() ) );
        const signed long x = encodedVectorComponent;
        const float fraction = ( x - static_cast< signed long >( std::numeric_limits< BufferedVectorComponentType >::min() ) ) / range;
        return ( fraction - 0.5f ) * 2;
    }

    /** \brief
      * Returns the buffered vector component corresponding to
      * \a actualVectorComponent.
      *
      * This function maps \a actualVectorComponent to values from the minimum of
      * `BufferedVectorComponentType` to its maximum linearly.
      *
      * \pre `std::abs(actualVectorComponent) <= 1`
      */
    static BufferedVectorComponentType encodeComponent( float actualVectorComponent )
    {
        LIBCARNA_ASSERT_EX
            ( std::abs( actualVectorComponent ) <= 1
            , "Unnormalized vector! Component: " + text::lexical_cast< std::string >( actualVectorComponent ) );
        
        const signed long range = static_cast< signed long >( std::numeric_limits< BufferedVectorComponentType >::max() )
            - static_cast< signed long >( std::numeric_limits< BufferedVectorComponentType >::min() );
        const signed long result = static_cast< signed long >( ( ( actualVectorComponent + 1 ) * range ) / 2 )
            + static_cast< signed long >( std::numeric_limits< BufferedVectorComponentType >::min() );
        return static_cast< BufferedVectorComponentType >( result );
    }

    /** \brief
      * Decodes and tells the vector stored at \a location.
      */
    math::Vector3f operator()( const math::Vector3ui& location ) const
    {
        return ( *this )( location.x(), location.y(), location.z() );
    }

    /** \overload
      */
    math::Vector3f operator()
        ( unsigned int x
        , unsigned int y
        , unsigned int z ) const
    {
        math::Vector3f result;
        const std::size_t index = 4 * ( x + size.x() * y + size.y() * size.x() * z );
        result.x() = decodeComponent( myBuffer->get()->at( index + 0 ) );
        result.y() = decodeComponent( myBuffer->get()->at( index + 1 ) );
        result.z() = decodeComponent( myBuffer->get()->at( index + 2 ) );
        return result;
    }

    /** \brief
      * Encodes \a normal and stores it at \a location.
      */
    void setVoxel( const math::Vector3ui& location, const math::Vector3f& normal )
    {
        this->setVoxel( location.x(), location.y(), location.z(), normal );
    }

    /** \overload
      */
    void setVoxel( unsigned int x, unsigned int y, unsigned int z, const math::Vector3f& normal )
    {
        LIBCARNA_ASSERT( x < size.x() && y < size.y() && z < size.z() );
        const std::size_t index = 4 * ( x + size.x() * y + size.y() * size.x() * z );
        myBuffer->get()->at( index + 0 ) = encodeComponent( normal.x() );
        myBuffer->get()->at( index + 1 ) = encodeComponent( normal.y() );
        myBuffer->get()->at( index + 2 ) = encodeComponent( normal.z() );
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
      * Holds the buffer.
      */
    const std::unique_ptr< Association< BufferType > > myBuffer;

private:

    void checkBuffer()
    {
        LIBCARNA_ASSERT_EX
            ( myBuffer.get() && myBuffer->get()
            , "No volume data buffer supplied!" );

        LIBCARNA_ASSERT_EX
            ( myBuffer->get()->size() >= size.x() * size.y() * size.z()
            , "Supplied volume data buffer is of size "
                << myBuffer->get()->size()
                << " bytes but must be at least "
                << 4 * sizeof( BufferedVectorComponentType ) * size.x() * size.y() * size.z()
                << " bytes!" );
    }

}; // BufferedNormalMap3D



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // BUFFEREDNORMALMAP3D_H_6014714286
