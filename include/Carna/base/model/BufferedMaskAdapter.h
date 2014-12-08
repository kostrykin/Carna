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

#ifndef BUFFEREDMASKADAPTER_H_6014714286
#define BUFFEREDMASKADAPTER_H_6014714286

/** \file   BufferedMaskAdapter.h
  *
  * \brief  Defines \ref Carna::base::model::BufferedMaskAdapter.
  *
  * \author Leonid Kostrykin
  * \date   26.7.13
  */

#include <Carna/base/model/BufferedMask.h>
#include <cstdint>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// BufferedMaskAdapter
// ----------------------------------------------------------------------------------
    
/** \brief  Implements \f$\mathbb Z^3 \to \text{VoxelType}\f$ as linear buffer.
  *
  * \since  \ref v_2_2_2
  *
  * \author Leonid Kostrykin
  * \date   26.7.13
  */
class CARNA_EXPORT BufferedMaskAdapter
{

    NON_COPYABLE

public:

    /** \brief  Holds the factorial mask type.
      */
    typedef BufferedMask< float > FactorialMask;
    
    /** \brief  Holds the binary mask type.
      */
    typedef BufferedMask< uint8_t > BinaryMask;


    /** \brief  Instantiates as adapter to factorial masks.
      *
      * \pre    <code>mask != nullptr</code>
      */
    BufferedMaskAdapter( Association< FactorialMask >* mask );
    
    /** \brief  Instantiates as adapter to binary masks.
      *
      * \pre    <code>mask != nullptr</code>
      */
    BufferedMaskAdapter( Association< BinaryMask >* mask );

    /** \brief  Releases acquired resources.
      */
    ~BufferedMaskAdapter();


    /** \brief  Tells whether this is an adapter to a factorial mask.
      */
    bool isFactorial() const;
    
    /** \brief  Tells whether this is an adapter to a binary mask.
      */
    bool isBinary() const;


    /** \brief  Interfaces the immutable factorial mask.
      *
      * \pre    <code>isFactorial() == true</code>
      */
    const FactorialMask& factorial() const;
    
    /** \brief  Interfaces the immutable binary mask.
      *
      * \pre    <code>isBinary() == true</code>
      */
    const BinaryMask& binary() const;
    
    /** \brief  Interfaces the mutable factorial mask.
      *
      * \pre    <code>isFactorial() == true</code>
      */
    FactorialMask& factorial();
    
    /** \brief  Interfaces the mutable binary mask.
      *
      * \pre    <code>isBinary() == true</code>
      */
    BinaryMask& binary();


private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // BufferedMask



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // BUFFEREDMASKADAPTER_H_6014714286
