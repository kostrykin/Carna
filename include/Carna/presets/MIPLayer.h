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

/** \brief  Defines an intensity range and the way it is visualized by a
  * \ref MIPStage "maximum intensity projection".
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

    const static base::BlendFunction LAYER_FUNCTION_ADD;     ///< Defines the additive blending function.
    const static base::BlendFunction LAYER_FUNCTION_REPLACE; ///< Defines "replacing" blending function.

    /** \brief  Instantiates.
      */
    MIPLayer
        ( const base::math::Span< float >& intensityRange
        , const base::math::Vector4f& color
        , const base::BlendFunction& function = LAYER_FUNCTION_REPLACE );

    /** \brief  Instantiates.
      */
    MIPLayer
        ( float minIntensity, float maxIntensity
        , const base::math::Vector4f& color
        , const base::BlendFunction& function = LAYER_FUNCTION_REPLACE );

    /** \brief
      * Holds the intensity range that this layer covers as described
      * \ref MIPStageLayers "here".
      */
    base::math::Span< float > intensityRange;
    
    /** \brief
      * Holds the color of this layer as described \ref MIPStageLayers "here".
      */
    base::math::Vector4f color;

    /** \brief
      * Sets the blending function to be used to combine this layer with the previous
      * layers.
      */
    void setFunction( const base::BlendFunction& layerFunction );
    
    /** \brief
      * References the blending function to be used to combine this layer with the
      * previous layers.
      */
    const base::BlendFunction& function() const;

}; // presets :: MIPLayer



}  // namespace Carna :: presets

}  // namespace Carna

#endif // MIPLAYER_H_6014714286
