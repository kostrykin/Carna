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

#include <Carna/base/qt/RotatingColor.h>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// RotatingColor
// ----------------------------------------------------------------------------------

RotatingColor::RotatingColor( Color initialColor )
{
    for( nextColor = 1; nextColor <= static_cast< unsigned int >( initialColor ); ++( *this ) );
}


RotatingColor& RotatingColor::operator++()
{

 // skip color 'black' for it is hard to see

    if( (nextColor & 7) == 0 )
    {
        ++nextColor;
    }

 // determine color components

    const unsigned int nR = (nextColor & (1<<2)) ? 1 : 0;
    const unsigned int nG = (nextColor & (1<<1)) ? 1 : 0;
    const unsigned int nB = (nextColor & (1<<0)) ? 1 : 0;

    this->color = qRgb( 255 * nR, 255 * nG, 255 * nB );

 // increment color index

    ++nextColor;

    return *this;

}



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna
