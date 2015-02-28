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

#ifndef MESHMANAGER_H_6014714286
#define MESHMANAGER_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/view/VideoResourceManager.h>
#include <memory>
#include <functional>

/** \file   MeshManager.h
  * \brief  Defines \ref Carna::base::view::MeshManager.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// MeshManager
// ----------------------------------------------------------------------------------

class CARNA_LIB MeshManager : public VideoResourceManager< MeshBase, MeshManager, std::function< MeshBase*() > >
{

protected:

    friend class VideoResourceManagerBase;

    MeshManager( const std::function< MeshBase*() >& createMesh );

    virtual MeshBase* createResource() override;

}; // MeshManager



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // MESHMANAGER_H_6014714286
