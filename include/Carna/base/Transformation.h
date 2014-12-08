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

#ifndef TRANSFORMATION_H_6014714286
#define TRANSFORMATION_H_6014714286

#include <Carna/Carna.h>
#include <TRTK/Transform3D.hpp>
#include <TRTK/Coordinate.hpp>

/** \file   Transformation.h
  * \brief  Defines \ref Carna::base::Transformation,
  *                 \ref Carna::base::Vector,
  *                 \ref Carna::base::fetchModelViewMatrix,
  *                 \ref Carna::base::fetchProjectionMatrix.
  *         Also overloads \ref glTranslate,
  *                        \ref glMultMatrix,
  *                        \ref glLoadMatrix.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Transformation
// ----------------------------------------------------------------------------------

/** \brief  Defines matrix and vector transformations through TRTK classes.
  */
typedef TRTK::Transform3D< double > Transformation;



// ----------------------------------------------------------------------------------
// Vector
// ----------------------------------------------------------------------------------

/** \brief  Defines vectors through TRTK classes.
  */
typedef TRTK::Coordinate< double > Vector;



// ----------------------------------------------------------------------------------
// fetchModelViewMatrix
// ----------------------------------------------------------------------------------

/** \brief      Queries current model-view matrix.
  *
  * \ingroup    MathTools

  */
void CARNA_EXPORT fetchModelViewMatrix( Transformation& out );



// ----------------------------------------------------------------------------------
// fetchProjectionMatrix
// ----------------------------------------------------------------------------------

/** \brief      Queries current projection matrix.
  *
  * \ingroup    MathTools
  */
void CARNA_EXPORT fetchProjectionMatrix( Transformation& out );



}  // namespace Carna :: base

}  // namespace Carna



// ----------------------------------------------------------------------------------
// glTranslate
// ----------------------------------------------------------------------------------

/** \brief      Overloads \c %glTranslate from the \c GL.h header.
  *             Multiplies the current with a translation matrix, created from given translation vector.
  *
  * Since this function overloads \c %glTranslate from the \c GL.h header, it is not placed within the \ref Carna namespace.
  *
  * \ingroup    MathTools
  */
void CARNA_EXPORT glTranslate( const Carna::base::Vector& );



// ----------------------------------------------------------------------------------
// glMultMatrix
// ----------------------------------------------------------------------------------

/** \brief      Overloads \c %glTranslate from the \c GL.h header.
  *             Multiplies the current with the specified matrix via \c %glMultMatrix.
  *
  * Since this function overloads \c %glTranslate from the \c GL.h header, it is not placed within the \ref Carna namespace.
  *
  * \ingroup    MathTools
  */
void CARNA_EXPORT glMultMatrix( const Carna::base::Transformation& );



// ----------------------------------------------------------------------------------
// glLoadMatrix
// ----------------------------------------------------------------------------------

/** \brief      Overloads \c %glTranslate from the \c GL.h header.
  *             Loads the specified matrix via \c %glLoadMatrix.
  *
  * Since this function overloads \c %glTranslate from the \c GL.h header, it is not placed within the \ref Carna namespace.
  *
  * \ingroup    MathTools
  */
void CARNA_EXPORT glLoadMatrix( const Carna::base::Transformation& );



#endif // TRANSFORMATION_H_6014714286
