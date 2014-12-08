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

#ifndef OBJECT3D_DETAILS_H_6014714286
#define OBJECT3D_DETAILS_H_6014714286

#include <Carna/base/model/Object3D.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// Object3D :: Details
// ----------------------------------------------------------------------------------

class Object3D :: Details
{

public:

    Details( Scene& model, const std::string& name );

    Details( const Position& position, const std::string& name );


    Position position;

    std::string name;

}; // Object3D :: Details



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // OBJECT3D_DETAILS_H_6014714286
