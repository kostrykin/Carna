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

#ifndef POINT3D_DETAILS_H_6014714286
#define POINT3D_DETAILS_H_6014714286

#include <Carna/base/view/Point3D.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Point3D :: Details
// ----------------------------------------------------------------------------------

class Point3D :: Details
{

    Point3D& self;

public:

    Details( Point3D& self, float size, const QColor& color );


    static qt::RotatingColor nextColor;

    void rotateColor();


    mutable QColor iconColor;

    mutable QIcon icon;
    

    QColor color;

    float size;

}; // Point3D :: Details



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // POINT3D_DETAILS_H_6014714286
