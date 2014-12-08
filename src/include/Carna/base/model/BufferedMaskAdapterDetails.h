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

#ifndef BUFFEREDMASKADAPTER_DETAILS_H_6014714286
#define BUFFEREDMASKADAPTER_DETAILS_H_6014714286

/** \file   BufferedMaskAdapterDetails.h
  *
  * \brief  Defines \ref Carna::base::model::BufferedMaskAdapter::Details.
  *
  * \author Leonid Kostrykin
  * \date   26.7.13
  */

#include <Carna/base/model/BufferedMask.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// BufferedMask :: Details
// ----------------------------------------------------------------------------------
    
class CARNA_EXPORT BufferedMaskAdapter :: Details
{

    NON_COPYABLE

public:

    Details( Association< FactorialMask >* mask );

    Details( Association< BinaryMask >* mask );


    template< typename ValueType >
    struct Slot
    {

        Slot( Association< BufferedMask< ValueType > >* mask )
            : mask( mask )
        {
        }

        std::unique_ptr< Association< BufferedMask< ValueType > > > mask;

    };  // Slot

    const Slot< FactorialMask::ValueType > factorialMask;
    const Slot< BinaryMask::ValueType > binaryMask;

}; // BufferedMask :: Details



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // BUFFEREDMASKADAPTER_DETAILS_H_6014714286
