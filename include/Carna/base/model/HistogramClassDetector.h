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

#ifndef HISTOGRAMCLASSDETECTOR_H_6014714286
#define HISTOGRAMCLASSDETECTOR_H_6014714286

/** \file   Histogram.h
  * \brief  Defines \ref Carna::base::model::HistogramClassDetector.
  */

#include <Carna/base/model/Histogram.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Math.h>

namespace Carna
{

namespace base
{

namespace model
{


// ----------------------------------------------------------------------------------
// HistogramClassDetector
// ----------------------------------------------------------------------------------

/** \brief  Computes classes of a given histogram.
  *
  * \param  HistogramType   Must be a \ref Carna::base::model::Histogram class template instance with \c HistogramType::FieldCodomain as an integral data type.
  *
  * Since \c HistogramType::FieldCodomain must be an integral data type, elements of HistogramType::FieldCodomain always can be arranged in some order.
  * From this arises the terminology of \em lowest or \em first and \em highest or \em last keys, as it is used below.
  *
  * The usual usage of this class looks like:
  * \code
  * #include <Carna/base/model/HistogramClassDetector.h>
  *
  * template< typename Histogram >
  * void printHistogramClasses( const Histogram& histogram )
  * {
  *     typedef Carna::base::model::HistogramClassDetector< Histogram > HistogramClassDetector;
  *     HistogramClassDetector class_detector( histogram );
  *
  *     class_detector.compute( HistogramClassDetector::Configuration
  *         ( -1024 //< first: the lowest key of the histogram
  *         ,  3071 //< last: the highest key of the histogram
  *         ,   30u //< gaussian radius
  *         ,  100u //< minimum width of a typical class
  *         ,   0.2 //< minimum logarithmic height difference ) );
  *
  *     for( auto class_itr = class_detector.result().begin(); class_itr != class_detector.result().end(); ++class_itr )
  *     {
  *         qDebug() << class_itr->first << " - " << class_itr->last << ( class_itr->typical ? " (typical)" : "" );
  *     }
  * }
  * \endcode
  *
  * For details on the last three arguments of the \c %compute method refer to the \ref HistogramClassDetectorAlgorithm "algorithm documentation".
  *
  * \section    HistogramClassDetectorAlgorithm Histogram Class Detection Algorithm
  *
  * The algorithm presented here is based on the assumption that the width of typical histogram classes is lower bound with the threshold \f$t \in \mathbb{N}\f$.
  * Therefore, given the following two conditions are fulfilled, a class \f$\left[a;b\right]\f$ is called \em typical:
  * - The class' width \f$b - a + 1\f$ is larger or equal the threshold \f$t\f$.
  * - The logarithmic gap between the lowest and the highest frequency the histogram \f$ H : K \to \mathbb{N}_0 \f$ attains within \f$\left[a;b\right]\f$ is greater or equal a certain constant \f$h \in \left[ 0; 1 \right]\f$:
  *   \f[ \exists x \in \left[ a; b \right] : \frac{ \log_{10} H \left( x \right) }{ \max\limits_{x' \in K}\ \log_{10} H \left( x' \right) } \geq h \f]
  *
  * The algorithm partitionates the whole histogram into a gapless sequence of typical and non-typical classes.
  *
  * Hereinafter all indices are considered to start with 1, not with 0.
  * A logarithmic representation \f$L\f$ of the key counts carried by the \ref Histogram::counter "histogram counter" \f$H\f$ is computed.
  * The lowest and the highest values that are to be recognized by the algorithm must be specified as \f$k_\text{min}\f$ and \f$k_\text{max}\f$:
  * \f[ L \left( k_\text{min}, k_\text{max} \right) = \Big( \log_{10} \big( H \left( k \right) + 1 \big) : k \in \left[ k_\text{min}, k_\text{max} \right] \subseteq K \Big) \f]
  *
  * Second, a smoothed representation \f$S\f$ of \f$L\f$ is computed, utilizing Gaussian kernel \f$\mathcal{G}_{\sigma = r/2}\f$ with radius \f$r\f$.
  * Looking at the derivative of \f$S_i\f$ we identify those \f$i\f$ that do represent local minimums,
  * whereby \f$k_\text{min}\f$ and \f$k_\text{max}\f$ (represented by the first and the last elements of \f$S\f$) are always considered such local minimums:
  * \f[ M = \Big\{ i : S_{i-1} < 0 \wedge S_i > 0, i \in \big[ 2; \left\| S \right\| \big] \Big\} \cup \Big\{ 1, \left\| S \right\| \Big\} \f]
  *
  * Now the actual class detection begins.
  * Lets consider \f$M \subset \mathbb{N}\f$ to be sorted ascendingly.
  * The algorithm now goes as follows:
  *
  * -# Set \f$a = M_1\f$.
  * -# For all \f$i = 2, 3, \dots, \left\| M \right\| \f$:
  *   -# Set \f$b = M_i\f$.
  *   -# If \f$ b - a \geq t \f$:
  *     -# If \f$ \big( \max_{j = a}^b S_j \ - \ \min_{j = a}^b S_j \big) / \max S > h \f$:
  *       -# If we haven't notated a histogram class yet and \f$ b > 1 \f$:
  *         -# <b>Notate the non-typical histogram class \f$\left[ 1, b - 1 \right]\f$.</b>
  *       -# Else, if we already have notated at least one histogram class and \f$c + 1 < b\f$:
  *         -# <b>Notate the non-typical histogram class \f$\left[ c + 1, b - 1 \right]\f$.</b>
  *       -# <b>Notate the typical histogram class \f$\left[ a; b \right]\f$.</b>
  *       -# Set \f$c = b\f$.
  *   -# Set \f$a = b + 1\f$.
  * -# If no classes have been notated:
  *   -# <b>Notate the non-typical histogram class \f$\left[ 1, \left\| M \right\| \right]\f$.</b>
  * -# Shift all notated classes by adding \f$k_{min}\f$ as an offset.
  *
  * It is a side effect of the algorithm that two or more non-typical classes may not occur in a row.
  *
  * \since  \ref v_2_4
  * \author Leonid Kostrykin
  * \date   23.1.14 - 25.1.14
  */
template< typename HistogramType >
class HistogramClassDetector
{

    NON_COPYABLE

public:

    /** \brief  Defines the histogram class instance this one is compatible with.
      */
    typedef HistogramType HistogramType;

    /** \brief  Derives the scalar field codomain from \ref HistogramType. Must be an integral data type.
      */
    typedef typename HistogramType::FieldCodomain FieldCodomain;
    
    /** \brief  Derives the data type to be used for scalar field codomain values when both <code>%FieldCodomain</code> and <code>const %FieldCodomain&</code> could be used.
      */
    typedef typename HistogramType::ConstReferencedFieldCodomain ConstReferencedFieldCodomain;


    /** \brief  Represents a histogram class.
      */
    struct Class
    {

        /** \brief  Holds the lowest key that belongs to that particular class.
          */
        FieldCodomain first;
        
        /** \brief  Holds the highest key that belongs to that particular class.
          */
        FieldCodomain last;

        /** \brief  Holds whether this particular class is \ref HistogramClassDetectorAlgorithm "considered typical".
          */
        bool typical;


        /** \brief  Instantiates.
          */
        Class( ConstReferencedFieldCodomain first, ConstReferencedFieldCodomain last, bool typical );

    };  // Class


    /** \brief  Holds the \ref HistogramClassDetectorAlgorithm "algorithm" parameters.
      */
    struct Configuration
    {

        /** \brief  Holds the \ref HistogramClassDetectorAlgorithm "algorithm's" \f$k_\text{min}\f$ parameter.
          *         Describes the lowest value the algorithm should recognize.
          */
        FieldCodomain first; 
        
        /** \brief  Holds the \ref HistogramClassDetectorAlgorithm "algorithm's" \f$k_\text{max}\f$ parameter.
          *         Describes the highest value the algorithm should recognize.
          */
        FieldCodomain last;
        
        /** \brief  Holds the \ref HistogramClassDetectorAlgorithm "algorithm's" \f$r\f$ parameter.
          *         Describes the radius of the Gaussian blur.
          */
        unsigned int radius;
        
        /** \brief  Holds the \ref HistogramClassDetectorAlgorithm "algorithm's" \f$t\f$ parameter.
          *         Describes the lower bound of typical histogram classes.
          */
        unsigned int minimumTypicalClassWidth;
        
        /** \brief  Holds the \ref HistogramClassDetectorAlgorithm "algorithm's" \f$h \in \left[ 0; 1 \right]\f$ parameter.
          *         Describes the minimum logarithmic gap between the lowest and the highest logarithmic frequency the histogram attains within a typical class.
          */
        double minimumLogarithmicHeightDifference;


        /** \brief  Instantiates.
          *
          * \param  first
          *         Describes the lowest value the algorithm should recognize.
          * \param  last
          *         Describes the highest value the algorithm should recognize.
          * \param  radius
          *         Describes the radius of the Gaussian blur.
          * \param  minimum_typical_class_width
          *         Describes the lower bound of typical histogram classes.
          * \param  minimum_logarithmic_height_difference
          *         Describes the minimum logarithmic gap between the lowest and the highest logarithmic frequency the histogram attains within a typical class.
          *
          * See the \ref HistogramClassDetectorAlgorithm for details.
          */
        Configuration( ConstReferencedFieldCodomain first, ConstReferencedFieldCodomain last, unsigned int radius, unsigned int minimum_typical_class_width, double minimum_logarithmic_height_difference );

    };  // Configuration


    /** \brief  References the analyzed histogram.
      */
    const HistogramType& histogram;


    /** \brief  Instantiates the class detection for the given histogram object.
      */
    HistogramClassDetector( const HistogramType& );

    /** \brief  Runs the \ref HistogramClassDetectorAlgorithm "algorithm" with the specified \a parameters.
      *
      * \post   <code>done() == true</code>
      */
    void compute( const Configuration& parameters );

    /** \brief  Tells the previously \ref compute "computed" classes.
      *
      * \pre   <code>done() == true</code>
      */
    const std::vector< Class >& result() const;


    /** \brief  Tells whether a \ref result "result" has been \ref compute "computed" previously.
      */
    bool done() const;
    
    /** \brief  Clears any previously computed result.
      *
      * \post   <code>done() == false</code>
      */
    void reset();


private:

    typedef std::vector< Class > Result;

    std::unique_ptr< Result > classes;

}; // HistogramClassDetector


template< typename HistogramType >
HistogramClassDetector< HistogramType >::HistogramClassDetector( const HistogramType& histogram )
    : histogram( histogram )
{
}


template< typename HistogramType >
const std::vector< typename HistogramClassDetector< HistogramType >::Class >& HistogramClassDetector< HistogramType >::result() const
{
    CARNA_ASSERT( done() );
    return *classes;
}


template< typename HistogramType >
bool HistogramClassDetector< HistogramType >::done() const
{
    return classes.get() != nullptr;
}


template< typename HistogramType >
void HistogramClassDetector< HistogramType >::reset()
{
    return classes.reset();
}


template< typename HistogramType >
void HistogramClassDetector< HistogramType >::compute( const Configuration& cfg )
{
    const std::size_t data_range = static_cast< std::size_t >( cfg.last - cfg.first + 1 );

    CARNA_ASSERT( data_range > cfg.radius );

    const double sigma = cfg.radius / 2.;

    reset();

 // compute logarithmic data

    double logarithmic_data_peak = std::numeric_limits< double >::min();
    std::vector< double > logarithmic_data( data_range );
    for( unsigned int sample_index = 0; sample_index < data_range; ++sample_index )
    {
        logarithmic_data[ sample_index ] = std::log10( static_cast< double >( histogram.counter[ cfg.first + sample_index ] + 1u ) );
        logarithmic_data_peak = std::max( logarithmic_data_peak, logarithmic_data[ sample_index ] );
    }

 // define Gaussian kernel

    const auto gaussian_coefficient = [&]( unsigned int distance )->double
    {
        return std::exp( -Math::sq( static_cast< signed int >( distance ) ) / ( 2 * Math::sq( sigma ) ) );
    };

 // pre-compute Gaussian coefficients

    std::map< unsigned int, double > gaussian_coefficients;
    double gaussian_coefficients_sum = 0.;
    for( unsigned int sample_distance = 0u; sample_distance <= cfg.radius; ++sample_distance )
    {
        gaussian_coefficients[ sample_distance ] = gaussian_coefficient( sample_distance );
        gaussian_coefficients_sum += gaussian_coefficients[ sample_distance ];
    }
    for( auto gaussian_coefficient_itr = gaussian_coefficients.begin(); gaussian_coefficient_itr != gaussian_coefficients.end(); ++gaussian_coefficient_itr )
    {
        gaussian_coefficient_itr->second /= ( 2 * gaussian_coefficients_sum );
    }

 // apply Gaussian kernel

    std::vector< double > smoothed_data( data_range );
    for( unsigned int dst_sample_index = 0u; dst_sample_index < data_range; ++dst_sample_index )
    {
        const auto sample_at_distance = [&]( signed int distance )->double
        {
            const unsigned int src_sample_index =
                static_cast< unsigned int >( Math::clamp< int >( static_cast< signed int >( dst_sample_index ) + distance, 0, data_range - 1 ) );
            return logarithmic_data[ src_sample_index ];
        };

        double result = 0.;

        for( signed int sample_distance = -static_cast< signed int >( cfg.radius ); sample_distance <= static_cast< signed int >( cfg.radius ); ++sample_distance )
        {
            const double sample = sample_at_distance( sample_distance );
            const double gaussian_coefficient = gaussian_coefficients[ std::abs( sample_distance ) ];
            const double summand = sample * gaussian_coefficient;
            result += summand;
        }

        smoothed_data[ dst_sample_index ] = result;
    }

 // find local negative peaks through derivative evaluation
 // the first and the last samples are assumed to be such peaks explicitly

    std::vector< unsigned int > peak_sample_indices;
    peak_sample_indices.push_back( 0u );
    double last_sample = smoothed_data[ peak_sample_indices.front() ];
    double last_derivative_sample = last_sample;

    for( unsigned int sample_index = 1u; sample_index < data_range; ++sample_index )
    {
        const double derivative_sample = smoothed_data[ sample_index ] - last_sample;
        if( last_derivative_sample < 0 && derivative_sample > 0 )
        {
            peak_sample_indices.push_back( sample_index );
        }
        last_sample = smoothed_data[ sample_index ];
        last_derivative_sample = derivative_sample;
    }

    if( peak_sample_indices.back() != smoothed_data.size() - 1 )
    {
        peak_sample_indices.push_back( smoothed_data.size() - 1 );
    }

 // find classes

    classes.reset( new Result() );
    const auto add_class = [&]( unsigned int first_index, unsigned int last_index, bool typical )
    {
        if( classes->empty() && first_index >= 1u )
        {
            const Class new_class( 0u, first_index - 1, false );
            classes->push_back( new_class );
        }
        else
        if( !classes->empty() && static_cast< unsigned int >( classes->back().last + 1 ) < first_index )
        {
            const Class new_class( classes->back().last + 1, first_index - 1, false );
            classes->push_back( new_class );
        }
        const Class new_class( first_index, last_index, typical );
        classes->push_back( new_class );
    };

    auto peak_sample_index_itr = peak_sample_indices.begin();
    unsigned int first_class_sample_index = *peak_sample_index_itr;
    for( ++peak_sample_index_itr; peak_sample_index_itr != peak_sample_indices.end(); ++peak_sample_index_itr )
    {
        const unsigned int last_class_sample_index = *peak_sample_index_itr;
        unsigned int class_candidate_width = last_class_sample_index - first_class_sample_index + 1u;

        if( class_candidate_width > cfg.minimumTypicalClassWidth )
        {
            double min_sample = +std::numeric_limits< double >::infinity();
            double max_sample = -std::numeric_limits< double >::infinity();
            for( unsigned int sample_index = first_class_sample_index; sample_index <= last_class_sample_index; ++sample_index )
            {
                max_sample = std::max( max_sample, smoothed_data[ sample_index ] );
                min_sample = std::min( min_sample, smoothed_data[ sample_index ] );
            }

            const double logarithmic_height_difference = ( max_sample - min_sample ) / logarithmic_data_peak;
            if( logarithmic_height_difference > cfg.minimumLogarithmicHeightDifference )
            {
                add_class( first_class_sample_index, last_class_sample_index, true );
            }
        }

        first_class_sample_index = last_class_sample_index + 1u;
    }

    if( classes->empty() )
    {
        add_class( peak_sample_indices.front(), peak_sample_indices.back(), false );
    }
    else
    if( static_cast< unsigned int >( classes->back().last ) < peak_sample_indices.back() )
    {
        add_class( classes->back().last + 1u, peak_sample_indices.back(), false );
    }

 // add offset to classes

    for( auto class_itr = classes->begin(); class_itr != classes->end(); ++class_itr )
    {
        class_itr->first += cfg.first;
        class_itr->last += cfg.first;
    }

 // finish

    CARNA_ASSERT( done() );
}



// ----------------------------------------------------------------------------------
// HistogramClassDetector :: Class
// ----------------------------------------------------------------------------------

template< typename HistogramType >
HistogramClassDetector< HistogramType >::Class::Class( ConstReferencedFieldCodomain first, ConstReferencedFieldCodomain last, bool typical )
    : first( first )
    , last( last )
    , typical( typical )
{
    CARNA_ASSERT( last >= first );
}



// ----------------------------------------------------------------------------------
// HistogramClassDetector :: Configuration
// ----------------------------------------------------------------------------------

template< typename HistogramType >
HistogramClassDetector< HistogramType >::Configuration::Configuration( ConstReferencedFieldCodomain first, ConstReferencedFieldCodomain last, unsigned int radius, unsigned int minimum_typical_class_width, double minimum_logarithmic_height_difference )
    : first( first )
    , last( last )
    , radius( radius )
    , minimumTypicalClassWidth( minimum_typical_class_width )
    , minimumLogarithmicHeightDifference( minimum_logarithmic_height_difference )
{
}



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // HISTOGRAMCLASSDETECTOR_H_6014714286
