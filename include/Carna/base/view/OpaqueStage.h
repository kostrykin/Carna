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

#ifndef OPAQUESTAGE_H_6014714286
#define OPAQUESTAGE_H_6014714286

#include <Carna/base/view/GeometryStage.h>
#include <Carna/base/view/Renderable.h>

/** \file   OpaqueStage.h
  * \brief  Defines \ref Carna::base::view::OpaqueStage.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// OpaqueStage
// ----------------------------------------------------------------------------------

class CARNA_LIB OpaqueStage : public GeometryStage< Renderable::ArbitraryOrder >
{

public:

    const static int GEOMETRY_TYPE = 1;

    const static unsigned int ROLE_DEFAULT_MESH = 0;

    OpaqueStage();

protected:

    virtual void render( const Renderable& ) override;

}; // OpaqueStage



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // OPAQUESTAGE_H_6014714286
