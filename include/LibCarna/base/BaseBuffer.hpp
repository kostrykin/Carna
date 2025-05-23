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

#ifndef BASEBUFFER_H_6014714286
#define BASEBUFFER_H_6014714286

#include <cstdlib>

/** \file
  * \brief
  * Defines \ref LibCarna::base::BaseBuffer.
  */

namespace LibCarna
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



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // BASEBUFFER_H_6014714286
