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

#ifndef BASEBUFFER_H_6014714286
#define BASEBUFFER_H_6014714286

#include <cstdlib>

/** \file   BaseBuffer.h
  * \brief  Defines \ref Carna::base::BaseBuffer.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// BaseBuffer
// ----------------------------------------------------------------------------------

/** \brief
  * Implements OpenGL buffer objects maintenance RAII base class.
  *
  * \author Leonid Kostrykin
  * \date   1.9.14 - 14.3.15
  */
class LIBCARNA BaseBuffer
{

    std::size_t mySize;

protected:

    /** \brief
      * Flag that indicates whether the maintained OpenGL buffer object is valid.
      */
    bool valid;

    /** \brief
      * Acquires new OpenGL buffer object.
      *
      * \param target
      *     specifies where the buffer object is to be bound to by `glBindBuffer`.
      */
    explicit BaseBuffer( unsigned int target );

    /** \brief
      * Sets the size of the maintained buffer object.
      */
    void setSize( std::size_t size );

public:

    /** \brief
      * Deletes the maintained OpenGL buffer object.
      */
    virtual ~BaseBuffer();

    /** \brief
      * Holds where the maintained buffer object is to be bound to by `glBindBuffer`.
      */
    const unsigned int target;

    /** \brief
      * Holds the ID of the maintained OpenGL buffer object.
      */
    const unsigned int id;

    /** \brief
      * Tells the \ref setSize "previously set size" of the maintained buffer object.
      */
    std::size_t size() const;

    /** \brief
      * Invokes `glBindBuffer` to bind the maintained buffer object to \ref target.
      */
    void bind() const;

    /** \brief
      * Tells whether the buffer object \ref valid "was marked as valid".
      */
    bool isValid() const;

}; // BaseBuffer



}  // namespace Carna :: base

}  // namespace Carna

#endif // BASEBUFFER_H_6014714286
