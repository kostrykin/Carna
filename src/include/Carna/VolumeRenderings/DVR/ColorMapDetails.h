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

#ifndef DVR_COLORMAP_DETAILS_H_6014714286
#define DVR_COLORMAP_DETAILS_H_6014714286

/** \file   ColorMapDetails.h
  * \brief  Defines \ref Carna::VolumeRenderings::DVR::ColorMap::Details.
  */

#include <Carna/VolumeRenderings/DVR/ColorMap.h>
#include <vector>

namespace Carna
{

namespace VolumeRenderings
{

namespace DVR
{



// ----------------------------------------------------------------------------------
// DVR :: ColorMap :: Details
// ----------------------------------------------------------------------------------

class ColorMap :: Details
{

    NON_COPYABLE

    ColorMap& self;

public:

    typedef short HUV;

    explicit Details( ColorMap& );


    void clear( ColorBuffer& out );

    void mask( ColorBuffer& out, HUV firstHuv, HUV lastHuv, const QColor& firstColor, const QColor& lastColor );


    SpanMap spans;

}; // DVR :: ColorMap :: Details



}  // namespace Carna :: VolumeRenderings :: DVR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // DVR_COLORMAP_DETAILS_H_6014714286
