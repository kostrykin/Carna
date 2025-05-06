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

#ifndef COLORMAP_H_6014714286
#define COLORMAP_H_6014714286

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/noncopyable.hpp>
#include <memory>

/** \file
  * \brief
  * Defines \ref LibCarna::base::ColorMap.
  */

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ColorMap
// ----------------------------------------------------------------------------------

/** \brief
  * Represents a mapping of intensity values to RGBA colors, that can be queried in a shader.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA ColorMap
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Holds the default resolution of the color map (16bit).
      */
    const static unsigned int DEFAULT_RESOLUTION = ( 1 << 16 );

    /** \brief
      * Instantiates.
      *
      * The parameter \a resolution determines the resolution of the color map. If your
      * data is 8bit, using a 8bit color map is sufficient. If your data is 32bit, you
      * probably also want to use a 32bit color map.
      */
    explicit ColorMap( unsigned int resolution = DEFAULT_RESOLUTION );

    /** \brief
      * Deletes the maintained OpenGL texture and sampler objects.
      */
    ~ColorMap();

    /** \brief
      * Clears the color map. All intensity values are mapped to
      * \ref base::Color::BLACK_NO_ALPHA after calling this method.
      */
    void clear();
    
    /** \brief
      * Linearly maps all intensity values from \a intensityRange to \a colorRange.
      *
      * The first/last intensity values from \a intensityRange are mapped to the
      * first/last values of \a colorRange, respectively. The values are interpolated
      * linearly in between.
      *
      * Nothing happens if the last intensity value of \a intensityRange is *smaller*
      * than the first. If the first and the last intensity values of
      * \a intensityRange correspond to the same entry of the color map, the *mean*
      * of the first and the last values from \a colorRange is written.
      */
    ColorMap& writeLinearSegment( const base::math::Span< float >& intensityRange, const base::math::Span< base::Color > colorRange );
    
    /** \overload
      */
    ColorMap& writeLinearSegment( float intensityFirst, float intensityLast, const base::Color& colorFirst, const base::Color& colorLast );

    /** \brief
      * Writes a linear segment with \ref writeLinearSegment for each subsequent pair
      * of \a colors. The \a colors are spaced equidistantly to cover the whole range
      * of values in \f$[0, 1]\f$.
      */
    ColorMap& writeLinearSpline( const std::vector< base::Color >& colors );

    /** \brief
      * Returns the list of colors stored in the color map.
      */
    const std::vector< base::Color >& getColorList() const;

    /** \brief
      * Binds this texture and the corresponding sampler to \a unit.
      */
    void bind( int unit ) const;

    /** \brief
      * Writes the content the \a other color map into this.
      */
    ColorMap& operator=( const ColorMap& other );

}; // base :: ColorMap



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // COLORMAP_H_6014714286
