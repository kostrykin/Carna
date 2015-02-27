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

#ifndef TEXTURE3DCONTROL_H_6014714286
#define TEXTURE3DCONTROL_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/view/VideoResourcesControl.h>

/** \file   Texture3DControl.h
  * \brief  Defines \ref Carna::base::view::Texture3DControl.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Texture3DControl
// ----------------------------------------------------------------------------------

class CARNA_LIB Texture3DControl : public VideoResourcesControl
{

public:

    virtual const Texture3D& texture() const = 0;

}; // Texture3DControl



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // TEXTURE3DCONTROL_H_6014714286
