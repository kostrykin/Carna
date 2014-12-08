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

#ifndef DVR_COLORMAP_H_6014714286
#define DVR_COLORMAP_H_6014714286

/** \file   ColorMap.h
  * \brief  Defines \ref Carna::VolumeRenderings::DVR::ColorMap.
  */

#include <Carna/base/noncopyable.h>
#include <Carna/base/Span.h>
#include <Carna/Carna.h>
#include <QColor>
#include <QObject>
#include <vector>
#include <map>

class QColor;

namespace Carna
{

namespace VolumeRenderings
{

namespace DVR
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: DVR :: ColorMap
// ----------------------------------------------------------------------------------

/** \brief  Maps HU values to color gradients.
  *
  * \author Leonid Kostrykin
  * \date   5.3.13 - 18.3.13
  */
class CARNA_EXPORT ColorMap : public QObject
{

    Q_OBJECT

    NON_COPYABLE

public:

    /** \brief  Refer to \ref writeToBuffer.
      */
    typedef std::vector< char > ColorBuffer;

    /** \brief  Defines a color interval.
      */
    typedef base::Span< QColor > ColorSpan;

    /** \brief  Defines a HU interval.
      */
    typedef base::Span< short > HUSpan;

    /** \brief  Maps color intervals to HU intervals. Refer to \ref data.
      */
    typedef std::map< HUSpan, ColorSpan > SpanMap;


    /** \brief  Instantiates.
      */
    explicit ColorMap( QObject* parent = nullptr );

    /** \brief  Destroys itself.
      */
    ~ColorMap();


    /** \brief  Dumps the current mapping to given buffer.
      *
      * \post   <code>out.size() == 4 * 4096</code>
      */
    void writeToBuffer( ColorBuffer& out ) const;

    /** \brief  Removes all mappings.
      *
      * Emits the \ref changed signal.
      */
    void clear();

    /** \brief  Adds given mapping.
      *
      * \pre    <code>huRange.first != huRange.last</code>
      * \pre    The given \a huRange does not intersect with any previously mapped HU ranges.
      *
      * Emits the \ref changed signal.
      */
    void put( const HUSpan& huRange, const ColorSpan& colorGradient );

    /** \brief  References the current mappings.
      */
    const SpanMap& data() const;


signals:

    /** \brief  Indicates that the mappings have been changed.
      *
      * Emitted by \ref put and \ref clear.
      */
    void changed();


private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // VolumeRenderings :: DVR :: ColorMap



}  // namespace Carna :: VolumeRenderings :: DVR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // DVR_COLORMAP_H_6014714286
