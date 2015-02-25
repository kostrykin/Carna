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

#ifndef TEXTURE3DMANAGER_H_6014714286
#define TEXTURE3DMANAGER_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/view/VideoResourcesManager.h>

/** \file   Texture3DManager.h
  * \brief  Defines \ref Carna::base::view::Texture3DManager.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Texture3DManager
// ----------------------------------------------------------------------------------

class CARNA_LIB Texture3DManager : public VideoResourcesManager
{

public:

    virtual const Texture3D& texture() const = 0;

}; // Texture3DManager



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // TEXTURE3DMANAGER_H_6014714286
