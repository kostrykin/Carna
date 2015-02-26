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

#ifndef MIP_CHANNEL_H_6014714286
#define MIP_CHANNEL_H_6014714286

/** \file   Channel.h
  * \brief  Defines \ref Carna::VolumeRenderings::MIP::Channel.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/Span.h>
#include <Carna/base/math.h>
#include <Carna/base/view/BlendFunction.h>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// VolumeRenderings :: MIP :: Channel
// ----------------------------------------------------------------------------------

/** \brief  Defines an HUV range and the way it's visualized.
  *
  * \author Leonid Kostrykin
  * \date   26.7.11 - 26.2.15
  */
class CARNA_LIB Channel
{

    NON_COPYABLE

public:

    const static base::view::BlendFunction CHANNEL_FUNCTION_ADD;
    const static base::view::BlendFunction CHANNEL_FUNCTION_REPLACE;

    typedef signed short HUV;

    /** \brief  Instantiates.
      */
    Channel
        ( const base::Span< HUV >& huRange
        , const base::math::Vector3f& color
        , float opacity = 1
        , const base::view::BlendFunction& function = CHANNEL_FUNCTION_REPLACE );

    /** \brief  Instantiates.
      */
    Channel
        ( HUV firstHuv, HUV lastHuv
        , const base::math::Vector3f& color
        , float opacity = 1
        , const base::view::BlendFunction& function = CHANNEL_FUNCTION_REPLACE );

    base::Span< HUV > huRange;

    base::math::Vector3f color;

    float opacity;

    const base::view::BlendFunction& function;

}; // VolumeRenderings :: MIP :: Channel


const base::view::BlendFunction Channel::CHANNEL_FUNCTION_REPLACE( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
const base::view::BlendFunction Channel::CHANNEL_FUNCTION_ADD    ( GL_SRC_ALPHA, GL_ONE );



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // MIP_CHANNEL_H_6014714286
