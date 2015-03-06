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

#ifndef MIPCHANNEL_H_6014714286
#define MIPCHANNEL_H_6014714286

/** \file   MIPChannel.h
  * \brief  Defines \ref Carna::presets::MIPChannel.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/Span.h>
#include <Carna/base/math.h>
#include <Carna/base/BlendFunction.h>

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// presets :: MIPChannel
// ----------------------------------------------------------------------------------

/** \brief  Defines an HUV range and the way it's visualized within a \ref MIPStage.
  *
  * \author Leonid Kostrykin
  * \date   26.7.11 - 26.2.15
  */
class CARNA_LIB MIPChannel
{

    NON_COPYABLE

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    const static base::BlendFunction CHANNEL_FUNCTION_ADD;
    const static base::BlendFunction CHANNEL_FUNCTION_REPLACE;

    /** \brief  Instantiates.
      */
    MIPChannel
        ( const base::Span< base::HUV >& huRange
        , const base::math::Vector4f& color
        , const base::BlendFunction& function = CHANNEL_FUNCTION_REPLACE );

    /** \brief  Instantiates.
      */
    MIPChannel
        ( base::HUV firstHuv, base::HUV lastHuv
        , const base::math::Vector4f& color
        , const base::BlendFunction& function = CHANNEL_FUNCTION_REPLACE );

    base::Span< base::HUV > huRange;

    base::math::Vector4f color;

    const base::BlendFunction& function;

}; // presets :: MIPChannel



}  // namespace Carna :: presets

}  // namespace Carna

#endif // MIPCHANNEL_H_6014714286
