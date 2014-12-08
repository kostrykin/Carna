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

#ifndef HISTOGRAM_H_6014714286
#define HISTOGRAM_H_6014714286

/** \file   Histogram.h
  * \brief  Defines \ref Carna::base::model::Counter,
  *                 \ref Carna::base::model::AssociativeCounter,
  *                 \ref Carna::base::model::IndexedCounter,
  *                 \ref Carna::base::model::Histogram,
  *                 \ref Carna::base::model::VolumeHistogramVolumeAdapter,
  *                 \ref Carna::base::model::VolumeHistogram.
  */

#include <Carna/base/model/Volume.h>
#include <Carna/base/CarnaException.h>
#include <map>
#include <vector>
#include <numeric>

namespace Carna
{

namespace base
{

namespace model
{


// ----------------------------------------------------------------------------------
// Counter
// ----------------------------------------------------------------------------------

/** \brief  Class template for generalized modeling of \f$\text{KeyType}\to\text{CountingType}\f$ mappings with \f$\text{CountingType} \subset \mathbb{N}_0\f$.
  *
  * The standard library types \c std::vector and \c std::map are commonly used for \a Container.
  * Refer to the specializations \ref IndexedCounter and \ref AssociativeCounter of this class template.
  * Hints on when to use which of these containers are given there.
  *
  * \param  Container must support element access through the index operator <code>[const %KeyType&]</code>. Besides that, it must provide a method \c clear that removes all elements.
  * \param  KeyType is the object type whose occurrences are to be counted.
  * \param  ContingType defines the counting domain.
  * \param  ConstReferencedKeyType defines the type to be used for referencing objects of type \a %KeyType constantly.
  *         Depending on the size of \a %KeyType it can be more efficient to use a copy instead of a reference by setting this type argument to \c %KeyType.
  *
  * \since  \ref v_2_4
  * \author Leonid Kostrykin
  * \date   19.1.14 - 23.1.14
  */
template< typename Container, typename KeyType, typename CountingType = unsigned int, typename ConstReferencedKeyType = const KeyType& >
struct Counter
{
    
    /** \brief  This is the object type whose occurrences are to be counted.
      */
    typedef KeyType KeyType;

    /** \brief  This is the counting domain.
      */
    typedef CountingType CountingType;

    /** \brief  This is the preferred type to be used whenever both \c %KeyType and \c const \c %KeyType& could be used.
      */
    typedef ConstReferencedKeyType ConstReferencedKeyType;


    /** \brief  References the current value counted for the given key.
      */
    CountingType& operator[]( ConstReferencedKeyType key );
    
    /** \brief  Tells the current value counted for the given key.
      */
    CountingType operator[]( ConstReferencedKeyType key ) const;

    /** \brief  Resets the counted values for all keys.
      *
      * \post   <code>operator[]( key ) == 0</code> \f$\forall\f$ \c key \f$\in\f$ \c KeyType
      */
    void reset();


protected:

    /** \brief  This is the underlying data container.
      */
    Container data;

}; // Counter


template< typename Container, typename KeyType, typename CountingType, typename ConstReferencedKeyType >
CountingType& Counter< Container, KeyType, CountingType, ConstReferencedKeyType >::operator[]( ConstReferencedKeyType key )
{
    return data[ key ];
}


template< typename Container, typename KeyType, typename CountingType, typename ConstReferencedKeyType >
CountingType Counter< Container, KeyType, CountingType, ConstReferencedKeyType >::operator[]( ConstReferencedKeyType key ) const
{
    return data[ key ];
}


template< typename Container, typename KeyType, typename CountingType, typename ConstReferencedKeyType >
void Counter< Container, KeyType, CountingType, ConstReferencedKeyType >::reset()
{
    data.clear();
}



// ----------------------------------------------------------------------------------
// AssociativeCounter
// ----------------------------------------------------------------------------------

/** \brief  \ref Counter specialization that uses \c std::map as the underlying data container.
  *
  * The usage of this specialization and not \ref IndexedCounter is the only possible choice whenever \a %KeyType cannot be considered an index, e.g. neither it is an integral data type nor can it be converted to such implicitly.
  * Furthermore the usage of this specialization is to be preferred whenever the counted keys are luckily to be distributed with huge gaps within \a %KeyType.
  *
  * \param  KeyType is the object type whose occurrences are to be counted.
  * \param  ContingType defines the counting domain.
  * \param  ConstReferencedKeyType defines the type to be used for referencing objects of type \a %KeyType constantly.
  *         Depending on the size of \a %KeyType it can be more efficient to use a copy instead of a reference by setting this type argument to \c %KeyType.
  *
  * \since  \ref v_2_4
  * \author Leonid Kostrykin
  * \date   19.1.14 - 23.1.14
  */
template< typename KeyType, typename CountingType = unsigned int, typename ConstReferencedKeyType = const KeyType& >
struct AssociativeCounter : public Counter< std::map< KeyType, CountingType >, KeyType, CountingType, ConstReferencedKeyType >
{

    CountingType& operator[]( ConstReferencedKeyType key )
    {
        return Counter< std::map< KeyType, CountingType >, KeyType, CountingType, ConstReferencedKeyType >::operator[]( key );
    }

    CountingType operator[]( ConstReferencedKeyType key ) const
    {
        const auto itr = data.find( key );
        if( itr == data.end() )
        {
            return 0;
        }
        else
        {
            return itr->second;
        }
    }

}; // AssociativeCounter



// ----------------------------------------------------------------------------------
// IndexedCounter
// ----------------------------------------------------------------------------------

/** \brief  \ref Counter specialization that uses \c std::vector as the underlying data container.
  *
  * This specialization can only be used if \a %KeyType can be considered an index, e.g. either it is an integral data type or can it be converted to such implicitly.
  * Furthermore the usage of this specialization is to be preferred over \ref AssociativeCounter whenever the counted keys are luckily to leave no gaps within \a %KeyType.
  *
  * \param  KeyType is the object type whose occurrences are to be counted.
  * \param  ContingType defines the counting domain.
  * \param  ConstReferencedKeyType defines the type to be used for referencing objects of type \a %KeyType constantly.
  *         Depending on the size of \a %KeyType it can be more efficient to use a copy instead of a reference by setting this type argument to \c %KeyType.
  *
  * \since  \ref v_2_4
  * \author Leonid Kostrykin
  * \date   19.1.14 - 23.1.14
  */
template< typename KeyType, typename CountingType = unsigned int, typename ConstReferencedKeyType = const KeyType& >
struct IndexedCounter : public Counter< std::vector< CountingType >, KeyType, CountingType, ConstReferencedKeyType >
{

    CountingType& operator[]( ConstReferencedKeyType key )
    {
        const std::size_t index = indexOf( key );
        if( index >= data.size() )
        {
            data.resize( index + 1 );
        }
        return data[ index ];
    }

    CountingType operator[]( ConstReferencedKeyType key ) const
    {
        const std::size_t index = indexOf( key );
        if( index >= data.size() )
        {
            return 0;
        }
        else
        {
            return data[ index ];
        }
    }

private:

    static std::size_t indexOf( ConstReferencedKeyType key );

}; // IndexedCounter


template< typename KeyType, typename CountingType, typename ConstReferencedKeyType >
std::size_t IndexedCounter< KeyType, CountingType, ConstReferencedKeyType >::indexOf( ConstReferencedKeyType key )
{
    return static_cast< std::size_t >( static_cast< long >( key ) - std::numeric_limits< KeyType >::min() );
}



// ----------------------------------------------------------------------------------
// Histogram
// ----------------------------------------------------------------------------------

/** \brief  Computes the histogram of any given scalar field in any given dimension.
  *
  * Usually it is sufficient to use either \ref IndexedCounter or \ref AssociativeCounter for the underlying \f[\mathrm{FieldCodomain}(\text{ScalarFieldType})\to\text{CountingType}\f] realization.
  * Refer to the particular counter's documentation for details.
  *
  * \param  ConcreteCounter defines the \f$\mathrm{FieldCodomain}(\text{ScalarFieldType})\to\text{CountingType}\f$ realization.
  * \param  ScalarFieldType must define the parenthesis operator to map from \f$\mathbb{N}_0^N\f$ with \a scalar_field_dimension as \f$N\f$ to codomain values.
  * \param  scalar_field_dimension defines the dimension of the scalar field's domain.
  *
  * \see    The class \ref VolumeHistogram is a specialization that simplifies histogram computation for \ref Volume objects.
  *
  * \see    The class template \ref HistogramClassDetector provides an algorithm for automatic detection of histogram classes.
  *
  * \since  \ref v_2_4
  * \author Leonid Kostrykin
  * \date   19.1.14 - 23.1.14
  */
template< typename ConcreteCounter, typename ScalarFieldType, unsigned int scalar_field_dimension >
class Histogram
{

public:

    /** \brief  Defines the \f$\mathrm{FieldCodomain}(\text{ScalarFieldType})\to\text{CountingType}\f$ realization.
      */
    typedef ConcreteCounter Counter;

    /** \brief  Maps a given \ref Vector to a codomain value.
      */
    typedef ScalarFieldType ScalarFieldType;

    /** \brief  Defines the \ref ScalarFieldType dimension.
      */
    const static unsigned int SCALAR_FIELD_DIMENSION = scalar_field_dimension;

    /** \brief  Defines a vector in \f$\mathbb{N}_0^N\f$ with \ref SCALAR_FIELD_DIMENSION as \f$N\f$.
      */
    typedef Eigen::Matrix< unsigned int, SCALAR_FIELD_DIMENSION, 1 > Vector;

    /** \brief  Derives the scalar field codomain type from \ref Counter.
      */
    typedef typename Counter::KeyType FieldCodomain;

    /** \brief  Derives the preferred type to be used whenever both \c %FieldCodomain and \c const \c %FieldCodomain& could be used from \ref Counter.
      */
    typedef typename Counter::ConstReferencedKeyType ConstReferencedFieldCodomain;
    
    /** \brief  Derives the counting type from \ref Counter.
      */
    typedef typename Counter::CountingType CountingDomain;


    /** \brief  Instantiates.
      *
      * \pre    All components of \a field_size are greater zero.
      *
      * \post   <code>&field == Histogram(field, field_size).field</code>
      * \post   <code>field_size == Histogram(field, field_size).fieldSize</code>
      */
    Histogram( const ScalarFieldType& field, const Vector& field_size );


    /** \brief  Holds the histogram computed lastly.
      */
    Counter counter;

    /** \brief  References the analyzed scalar field.
      */
    const ScalarFieldType& field;

    /** \brief  Holds the size of the analyzed scalar field.
      */
    const Vector fieldSize;


    /** \brief  Computes the histogram by updating the \ref counter.
      */
    void updateCounter();

}; // Histogram


template< typename ConcreteCounter, typename ScalarFieldType, unsigned int scalar_field_dimension >
Histogram< ConcreteCounter, ScalarFieldType, scalar_field_dimension >::Histogram( const ScalarFieldType& field, const Vector& fieldSize )
    : field( field )
    , fieldSize( fieldSize )
{
    for( unsigned int dimension = 0; dimension < SCALAR_FIELD_DIMENSION; ++dimension )
    {
        CARNA_ASSERT( fieldSize[ dimension ] > 0 );
    }
}


template< typename ConcreteCounter, typename ScalarFieldType, unsigned int scalar_field_dimension >
void Histogram< ConcreteCounter, ScalarFieldType, scalar_field_dimension >::updateCounter()
{
    counter.reset();

    const Vector& end_position = fieldSize;
    Vector position = Vector::Zero();

    bool position_moved;
    do
    {
        position_moved = false;

     // update the counter for the current location

        const FieldCodomain value = field( position );
        ++counter[ value ];

     // iterate over all vector components and increment the first that hasn't reached it's maximum yet

        for( unsigned int incremented_dimension = 0; incremented_dimension < SCALAR_FIELD_DIMENSION; ++incremented_dimension )
        {
            if( position[ incremented_dimension ] + 1 < end_position[ incremented_dimension ] )
            {
                ++position[ incremented_dimension ];

             // reset all vector components that are of lower index as the incremented component to zero,
             // e.g. the first and second vector components will be reset whenever the third vector component increases

                for( unsigned int lower_dimension = 0; lower_dimension < incremented_dimension; ++lower_dimension )
                {
                    if( lower_dimension != incremented_dimension )
                    {
                        position[ lower_dimension ] = 0;
                    }
                }

             // move on to the next position

                position_moved = true;
                break;
            }
        }
    }
    while( position_moved );
}



// ----------------------------------------------------------------------------------
// VolumeHistogramVolumeAdapter
// ----------------------------------------------------------------------------------

/** \brief  Defines an adapter that allows to access \ref Volume objects using positions that are specified by the \ref Histogram::Vector data type.
  *
  * This is a \ref VolumeHistogram helper class.
  * Usually there should be no reason to use this class directly.
  *
  * \param  ConcreteCounter defines the \f$\mathrm{FieldCodomain}(\text{ScalarFieldType})\to\text{CountingType}\f$ realization.
  *
  * \since  \ref v_2_4
  * \author Leonid Kostrykin
  * \date   19.1.14 - 23.1.14
  */
template< typename CounterType = IndexedCounter< signed short, unsigned int, signed short > >
class VolumeHistogramVolumeAdapter
{

public:

    /** \brief  Instantiates.
      */
    VolumeHistogramVolumeAdapter( const Volume& volume, const Vector3ui& size );

    /** \brief  References the adapted volume.
      */
    const Volume& volume;

    /** \brief  Holds the size of the adapted volume.
      */
    const Vector3ui size;

    /** \brief  Tells the HUV of the voxel located at \a position.
      */
    signed short operator()( const typename Histogram< CounterType, VolumeHistogramVolumeAdapter, 3u >::Vector& position ) const;

}; // VolumeHistogramVolumeAdapter


template< typename CounterType >
VolumeHistogramVolumeAdapter< CounterType >::VolumeHistogramVolumeAdapter( const Volume& volume, const Vector3ui& size )
    : volume( volume )
    , size( size )
{
}


template< typename CounterType >
signed short VolumeHistogramVolumeAdapter< CounterType >::operator()( const typename Histogram< CounterType, VolumeHistogramVolumeAdapter, 3u >::Vector& position ) const
{
    return volume( position.x(), position.y(), position.z() );
}



// ----------------------------------------------------------------------------------
// VolumeHistogram
// ----------------------------------------------------------------------------------

/** \brief  Computes the histogram of \ref Volume objects.
  *
  * Usually it is sufficient to use either \ref IndexedCounter as the underlying \f[\mathrm{FieldCodomain}(\text{ScalarFieldType})\to\text{CountingType}\f] realization.
  * Refer to the particular counter's documentation for details.
  *
  * \param  ConcreteCounter defines the \f$\mathrm{FieldCodomain}(\text{ScalarFieldType})\to\text{CountingType}\f$ realization.
  *
  * Example with \c volume as an \ref Volume typed object reference:
  * \code
  * Carna::base::model::VolumeHistogram histogram( volume, volume.size );
  * histogram.updateCounter();
  * for( signed short huv = -1024; huv <= 3071; ++huv )
  * {
  *     const unsigned int absolute_frequency = histogram.counter[ huv ];
  *     report_histogram_sample( huv, absolute_frequency );
  * }
  * \endcode
  *
  * \see    The class template \ref HistogramClassDetector provides an algorithm for automatic detection of histogram classes.
  *
  * \since  \ref v_2_4
  * \author Leonid Kostrykin
  * \date   19.1.14 - 23.1.14
  */
template< typename CounterType = IndexedCounter< signed short, unsigned int, signed short > >
class VolumeHistogram : private VolumeHistogramVolumeAdapter< CounterType >, public Histogram< CounterType, VolumeHistogramVolumeAdapter< CounterType >, 3u >
{

public:

    /** \brief  Instantiates.
      */
    VolumeHistogram( const Volume& volume, const Vector3ui& size );

}; // VolumeHistogram


template< typename CounterType >
VolumeHistogram< CounterType >::VolumeHistogram( const Volume& volume, const Vector3ui& size )
    : VolumeHistogramVolumeAdapter< CounterType >( volume, size )
    , Histogram( static_cast< const VolumeHistogramVolumeAdapter& >( *this ), Vector( size.x, size.y, size.z ) )
{
}



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // HISTOGRAM_H_6014714286
