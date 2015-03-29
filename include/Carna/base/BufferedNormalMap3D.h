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

#ifndef BUFFEREDNORMALMAP3D_H_6014714286
#define BUFFEREDNORMALMAP3D_H_6014714286

/** \file   BufferedNormalMap3D.h
  * \brief  Defines \ref Carna::base::BufferedNormalMap3D.
  */

#include <Carna/base/NormalMap3D.h>
#include <Carna/base/Composition.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/text.h>
#include <vector>
#include <memory>
#include <type_traits>
#include <climits>

namespace Carna
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
  *     of a single normal.
  *
  * \param BufferType is the data type used as container for the normal vectors.
  *
  * \author Leonid Kostrykin
  * \date   26.3.15 - 29.3.15
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
      * Returns the normal vector component corresponding to
      * \a bufferedVectorComponent.
      */
    static float bufferComponentToNormalComponent( BufferedVectorComponentType bufferedVectorComponent )
    {
        const float range = static_cast< float >( static_cast< signed long >( std::numeric_limits< BufferedVectorComponentType >::max() )
            - static_cast< signed long >( std::numeric_limits< BufferedVectorComponentType >::min() ) );
        const signed long x = bufferedVectorComponent;
        const float fraction = ( x - static_cast< signed long >( std::numeric_limits< BufferedVectorComponentType >::min() ) ) / range;
        return ( fraction - 0.5f ) * 2;
    }

    /** \brief
      * Returns the buffered vector component corresponding to
      * \a normalVectorComponent.
      */
    static BufferedVectorComponentType normalComponentToBufferComponent( float normalVectorComponent )
    {
        CARNA_ASSERT_EX
            ( std::abs( normalVectorComponent ) <= 1
            , "Unnormalized vector! Component: " + text::lexical_cast< std::string >( normalVectorComponent ) );
        
        const signed long range = static_cast< signed long >( std::numeric_limits< BufferedVectorComponentType >::max() )
            - static_cast< signed long >( std::numeric_limits< BufferedVectorComponentType >::min() );
        const signed long result = static_cast< signed long >( ( ( normalVectorComponent + 1 ) * range ) / 2 )
            + static_cast< signed long >( std::numeric_limits< BufferedVectorComponentType >::min() );
        return static_cast< BufferedVectorComponentType >( result );
    }

    math::Vector3f operator()
        ( unsigned int x
        , unsigned int y
        , unsigned int z ) const
    {
        math::Vector3f result;
        const std::size_t index = 4 * ( x + size.x() * y + size.y() * size.x() * z );
        result.x() = bufferComponentToNormalComponent( myBuffer->get()->at( index + 0 ) );
        result.y() = bufferComponentToNormalComponent( myBuffer->get()->at( index + 1 ) );
        result.z() = bufferComponentToNormalComponent( myBuffer->get()->at( index + 2 ) );
        return result;
    }

    math::Vector3f operator()( const math::Vector3ui& at ) const
    {
        return ( *this )( at.x(), at.y(), at.z() );
    }

    void setVoxel( unsigned int x, unsigned int y, unsigned int z, const math::Vector3f& normal )
    {
        CARNA_ASSERT( x < size.x() && y < size.y() && z < size.z() );
        const std::size_t index = 4 * ( x + size.x() * y + size.y() * size.x() * z );
        myBuffer->get()->at( index + 0 ) = normalComponentToBufferComponent( normal.x() );
        myBuffer->get()->at( index + 1 ) = normalComponentToBufferComponent( normal.y() );
        myBuffer->get()->at( index + 2 ) = normalComponentToBufferComponent( normal.z() );
    }

    void setVoxel( const math::Vector3ui& at, const math::Vector3f& normal )
    {
        this->setVoxel( at.x(), at.y(), at.z(), normal );
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

    const std::unique_ptr< Association< BufferType > > myBuffer;

private:

    void checkBuffer()
    {
        CARNA_ASSERT_EX
            ( myBuffer.get() && myBuffer->get()
            , "No volume data buffer supplied!" );

        CARNA_ASSERT_EX
            ( myBuffer->get()->size() >= size.x() * size.y() * size.z()
            , "Supplied volume data buffer is of size "
                << myBuffer->get()->size()
                << " bytes but must be at least "
                << 4 * sizeof( BufferedVectorComponentType ) * size.x() * size.y() * size.z()
                << " bytes!" );
    }

}; // BufferedHUVolume



}  // namespace Carna :: base

}  // namespace Carna

#endif // BUFFEREDNORMALMAP3D_H_6014714286
