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

#include <LibCarna/base/RotatingColor.h>

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// RotatingColor
// ----------------------------------------------------------------------------------

RotatingColor::RotatingColor( ColorIdentifier initialColor )
    : initialColor( initialColor )
{
    color.a = 255;
    reset();
}


void RotatingColor::reset()
{
    for( nextColor = 1; nextColor <= static_cast< unsigned int >( initialColor ); ++( *this ) );
}


RotatingColor& RotatingColor::operator++()
{
    /* Skip color 'black' for it is hard to see.
     */
    if( ( nextColor & 7 ) == 0 )
    {
        ++nextColor;
    }

    /* Determine color components.
     */
    const unsigned int nR = ( nextColor & ( 1<<2 ) ) ? 1 : 0;
    const unsigned int nG = ( nextColor & ( 1<<1 ) ) ? 1 : 0;
    const unsigned int nB = ( nextColor & ( 1<<0 ) ) ? 1 : 0;

    this->color.r = 255 * nR;
    this->color.g = 255 * nG;
    this->color.b = 255 * nB;

    /* Increment color index.
     */
    ++nextColor;

    return *this;
}



}  // namespace LibCarna :: base

}  // namespace LibCarna
