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

#ifndef MIPLAYER_H_6014714286
#define MIPLAYER_H_6014714286

/** \file   MIPLayer.h
  * \brief  Defines \ref Carna::presets::MIPLayer.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/math/Span.h>
#include <Carna/base/math.h>
#include <Carna/base/BlendFunction.h>

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// presets :: MIPLayer
// ----------------------------------------------------------------------------------

/** \brief  Defines an HUV range and the way it's visualized within a \ref MIPStage.
  *
  * \author Leonid Kostrykin
  * \date   26.7.11 - 26.2.15
  */
class CARNA_LIB MIPLayer
{

    NON_COPYABLE

    const base::BlendFunction* myFunction;

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    const static base::BlendFunction LAYER_FUNCTION_ADD;
    const static base::BlendFunction LAYER_FUNCTION_REPLACE;

    /** \brief  Instantiates.
      */
    MIPLayer
        ( const base::math::Span< base::HUV >& huRange
        , const base::math::Vector4f& color
        , const base::BlendFunction& function = LAYER_FUNCTION_REPLACE );

    /** \brief  Instantiates.
      */
    MIPLayer
        ( base::HUV firstHuv, base::HUV lastHuv
        , const base::math::Vector4f& color
        , const base::BlendFunction& function = LAYER_FUNCTION_REPLACE );

    base::math::Span< base::HUV > huRange;

    base::math::Vector4f color;

    void setFunction( const base::BlendFunction& layerFunction );

    const base::BlendFunction& function() const;

}; // presets :: MIPLayer



}  // namespace Carna :: presets

}  // namespace Carna

#endif // MIPLAYER_H_6014714286
