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
#include <Carna/base/view/VideoResourceManager.h>
#include <functional>

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

class Texture3DManager : public VideoResourceManager< Texture3D, Texture3DManager, std::function< Texture3D*() > >
{

protected:

    friend class VideoResourceManagerBase;

    Texture3DManager( const std::function< Texture3D*() >& createTexture );

    virtual Texture3D* createResource() override;

}; // Texture3DManager



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // TEXTURE3DMANAGER_H_6014714286
