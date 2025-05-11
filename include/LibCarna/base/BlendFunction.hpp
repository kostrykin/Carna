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

#ifndef BLENDFUNCTION_H_6014714286
#define BLENDFUNCTION_H_6014714286

/** \file
  * \brief
  * Defines \ref LibCarna::base::BlendFunction.
  */

#include <LibCarna/LibCarna.hpp>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BlendFunction
// ----------------------------------------------------------------------------------

/** \brief
  * Defines how alpha blending is to be performed.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA BlendFunction
{

public:

    /** \brief
      * Creates blend function with \a sourceFactor and \a destinationFactor.
      *
      * \attention
      * The arguments \a sourceFactor and \a destinationFactor do not accept
      * arbitrary values. Instead, use constants like `GL_ONE` and `GL_SRC_ALPHA`.
      */
    BlendFunction( int sourceFactor, int destinationFactor );

    /** \brief
      * Holds the source factor.
      */
    int sourceFactor;

    /** \brief
      * Holds the destination factor.
      */
    int destinationFactor;
    
    /** \brief
      * Tells whether this function equals \a other.
      */
    bool operator==( const BlendFunction& other ) const;

}; // BlendFunction



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // BLENDFUNCTION_H_6014714286
