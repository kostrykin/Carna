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

#ifndef BUFFEREDVECTORFIELDTEXTURE_H_6014714286
#define BUFFEREDVECTORFIELDTEXTURE_H_6014714286

#include <Carna/base/ManagedTexture3D.h>
#include <Carna/base/BufferedVectorFieldFormat.h>
#include <memory>

/** \file   BufferedVectorFieldTexture.h
  * \brief  Defines \ref Carna::base::BufferedVectorFieldTexture.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BufferedVectorFieldTexture
// ----------------------------------------------------------------------------------

template< typename BufferedVectorFieldType >
class BufferedVectorFieldTexture : public ManagedTexture3D
{

protected:

    BufferedVectorFieldTexture( const BufferedVectorFieldType& field );

public:

    const BufferedVectorFieldType& field;

    /** \brief
      * 
      * \todo Write brief.
      * Invoke \ref release when it isn't needed any longer.
      */
    static BufferedVectorFieldTexture< BufferedVectorFieldType >& create( const BufferedVectorFieldType& field );

    virtual bool controlsSameVideoResource( const GeometryFeature& ) const override;

}; // BufferedVectorFieldTexture


template< typename BufferedVectorFieldType >
BufferedVectorFieldTexture< BufferedVectorFieldType >::BufferedVectorFieldTexture( const BufferedVectorFieldType& volume )
    : ManagedTexture3D
        ( volume.size
        , BufferedVectorFieldFormat< BufferedVectorFieldType >::INTERNAL_FORMAT
        , BufferedVectorFieldFormat< BufferedVectorFieldType >::PIXEL_FORMAT
        , BufferedVectorFieldFormat< BufferedVectorFieldType >::BUFFER_TYPE
        , &volume.buffer().front() )
    , field( field )
{
}


template< typename BufferedVectorFieldType >
BufferedVectorFieldTexture< BufferedVectorFieldType >& BufferedVectorFieldTexture< BufferedVectorFieldType >::create
    ( const BufferedVectorFieldType& field )
{
    return *new BufferedVectorFieldTexture< BufferedVectorFieldType >( field );
}


template< typename BufferedVectorFieldType >
bool BufferedVectorFieldTexture< BufferedVectorFieldType >::controlsSameVideoResource( const GeometryFeature& other ) const
{
    typedef BufferedVectorFieldTexture< BufferedVectorFieldType > CompatibleBufferedVectorFieldTexture;
    const CompatibleBufferedVectorFieldTexture* const other2
        = dynamic_cast< const CompatibleBufferedVectorFieldTexture* >( &other );
    if( other2 == nullptr )
    {
        return false;
    }
    else
    {
        return &field == &other2->field;
    }
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // BUFFEREDVECTORFIELDTEXTURE_H_6014714286
