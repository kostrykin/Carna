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

#include <LibCarna/base/ManagedTexture3D.h>
#include <LibCarna/base/BufferedVectorFieldFormat.h>
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

/** \brief
  * Specializes the \ref ManagedTexture3D class s.t. the texture's data is
  * initialized from a \ref math::VectorField that supports a `buffer` method, like
  * \ref BufferedHUVolume and \ref BufferedNormalMap3D do.
  *
  * \author Leonid Kostrykin
  * \date   26.3.15 - 30.3.15
  */
template< typename BufferedVectorFieldType >
class BufferedVectorFieldTexture : public ManagedTexture3D
{

protected:

    /** \brief
      * Instantiates.
      *
      * \param field
      *     references the \ref math::VectorField to be uploaded to the texture.
      */
    BufferedVectorFieldTexture( const BufferedVectorFieldType& field );

public:

    /** \brief
      * References the \ref math::VectorField to be uploaded to the texture.
      */
    const BufferedVectorFieldType& field;

    /** \brief
      * Instantiates.
      * Invoke \ref release when it isn't needed any longer.
      *
      * \copydetails BufferedVectorFieldTexture::BufferedVectorFieldTexture(const BufferedVectorFieldType&)
      */
    static BufferedVectorFieldTexture< BufferedVectorFieldType >& create( const BufferedVectorFieldType& field );
    
    /** \brief
      * Tells `true` if \a other is of same type and references the same \ref field.
      * Tells `false` otherwise.
      */
    virtual bool controlsSameVideoResource( const GeometryFeature& other ) const override;

}; // BufferedVectorFieldTexture


template< typename BufferedVectorFieldType >
BufferedVectorFieldTexture< BufferedVectorFieldType >::BufferedVectorFieldTexture( const BufferedVectorFieldType& field )
    : ManagedTexture3D
        ( field.size
        , BufferedVectorFieldFormat< BufferedVectorFieldType >::INTERNAL_FORMAT
        , BufferedVectorFieldFormat< BufferedVectorFieldType >::PIXEL_FORMAT
        , BufferedVectorFieldFormat< BufferedVectorFieldType >::BUFFER_TYPE
        , &field.buffer().front() )
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
