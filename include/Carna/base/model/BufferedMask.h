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

#ifndef BUFFEREDMASK_H_6014714286
#define BUFFEREDMASK_H_6014714286

/** \file   BufferedMask.h
  *
  * \brief  Defines \ref Carna::base::model::BufferedMask.
  *
  * \author Leonid Kostrykin
  * \date   19.6.13 - 26.7.13
  */

#include <Carna/base/model/ScalarField.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// BufferedMask
// ----------------------------------------------------------------------------------
    
/** \brief  Implements \f$\mathbb Z^3 \to \text{VoxelType}\f$ as linear buffer.
  *
  * \since  \ref v_2_2_2
  *
  * \author Leonid Kostrykin
  * \date   19.6.13 - 26.7.13
  */
template< typename VoxelType >
class BufferedMask : public ScalarField< VoxelType >
{

public:

    /** \brief  Creates new linear buffer from given mask.
      */
    BufferedMask( const Vector3ui& size, const ScalarField< VoxelType >& mask );

    /** \brief  Creates new, empty linear buffer.
      */
    BufferedMask( const Vector3ui& size );

    /** \brief  Releases acquired resources.
      */
    virtual ~BufferedMask();


    /** \brief  Holds the size of this mask.
      */
    const Vector3ui size;
    
    /** \brief  Underlying linear buffer
      */
    std::vector< VoxelType > buffer;


    /** \brief  Returns immutable value of specified voxel.
      */
    virtual VoxelType operator()( unsigned int x, unsigned int y, unsigned int z ) const override;
    
    /** \brief  Returns mutable value of specified voxel.
      */
    VoxelType& operator()( unsigned int x, unsigned int y, unsigned int z );

}; // BufferedMask


template< typename VoxelType >
BufferedMask< VoxelType >::BufferedMask( const Vector3ui& size, const ScalarField< VoxelType >& mask )
    : size( size )
    , buffer( size.x * size.y * size.z )
{
    for( unsigned int x = 0; x < size.x; ++x )
    for( unsigned int y = 0; y < size.y; ++y )
    for( unsigned int z = 0; z < size.z; ++z )
    {
        const float voxel = mask( x, y, z );
        const unsigned int position = x + size.x * y + size.y * size.x * z;
        buffer[ position ] = voxel;
    }
}


template< typename VoxelType >
BufferedMask< VoxelType >::BufferedMask( const Vector3ui& size )
    : size( size )
    , buffer( size.x * size.y * size.z )
{
}


template< typename VoxelType >
BufferedMask< VoxelType >::~BufferedMask()
{
}


template< typename VoxelType >
VoxelType BufferedMask< VoxelType >::operator()( unsigned int x, unsigned int y, unsigned int z ) const
{
    return const_cast< BufferedMask& >( *this )( x, y, z );
}


template< typename VoxelType >
VoxelType& BufferedMask< VoxelType >::operator()( unsigned int x, unsigned int y, unsigned int z )
{
    const unsigned int position = x + size.x * y + size.y * size.x * z;
    return buffer[ position ];
}



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // BUFFEREDMASK_H_6014714286
