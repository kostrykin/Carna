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

#include <Carna/base/model/BufferedMaskAdapter.h>
#include <Carna/base/model/BufferedMaskAdapterDetails.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// BufferedMaskAdapter :: Details
// ----------------------------------------------------------------------------------

BufferedMaskAdapter::Details::Details( Association< FactorialMask >* mask )
    : factorialMask( mask )
    , binaryMask( nullptr )
{
}


BufferedMaskAdapter::Details::Details( Association< BinaryMask >* mask )
    : factorialMask( nullptr )
    , binaryMask( mask )
{
}



// ----------------------------------------------------------------------------------
// BufferedMaskAdapter
// ----------------------------------------------------------------------------------

BufferedMaskAdapter::BufferedMaskAdapter( Association< FactorialMask >* mask )
    : pimpl( new Details( mask ) )
{
    CARNA_ASSERT( mask != nullptr );
}


BufferedMaskAdapter::BufferedMaskAdapter( Association< BinaryMask >* mask )
    : pimpl( new Details( mask ) )
{
    CARNA_ASSERT( mask != nullptr );
}


BufferedMaskAdapter::~BufferedMaskAdapter()
{
}


bool BufferedMaskAdapter::isFactorial() const
{
    if( pimpl->factorialMask.mask.get() != nullptr )
    {
        CARNA_ASSERT( !isBinary() );
        return true;
    }
    else
    {
        return false;
    }
}


bool BufferedMaskAdapter::isBinary() const
{
    if( pimpl->binaryMask.mask.get() != nullptr )
    {
        CARNA_ASSERT( !isFactorial() );
        return true;
    }
    else
    {
        return false;
    }
}


const BufferedMaskAdapter::FactorialMask& BufferedMaskAdapter::factorial() const
{
    CARNA_ASSERT( isFactorial() );
    return *pimpl->factorialMask.mask->get();
}


const BufferedMaskAdapter::BinaryMask& BufferedMaskAdapter::binary() const
{
    CARNA_ASSERT( isBinary() );
    return *pimpl->binaryMask.mask->get();
}


BufferedMaskAdapter::FactorialMask& BufferedMaskAdapter::factorial()
{
    CARNA_ASSERT( isFactorial() );
    return *pimpl->factorialMask.mask->get();
}


BufferedMaskAdapter::BinaryMask& BufferedMaskAdapter::binary()
{
    CARNA_ASSERT( isBinary() );
    return *pimpl->binaryMask.mask->get();
}



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna
