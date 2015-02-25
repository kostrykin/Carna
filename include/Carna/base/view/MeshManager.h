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
#include <Carna/base/view/VideoResourcesManager.h>
#include <memory>

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
    
/** \brief
  * Abstract base \ref VideoResourcesManager for \ref MeshBase instances.
  *
  * Only \ref createMesh must be implemented s.t. it creates a new mesh instance.
  *
  * \author Leonid Kostrykin
  * \date 25.2.2015
  */
class CARNA_LIB MeshManager : public VideoResourcesManager
{

    std::unique_ptr< MeshBase > myMesh;

public:

    virtual void uploadResources() override;

    virtual void deleteResources() override;

    const MeshBase& mesh() const;

protected:

    virtual MeshBase* createMesh() = 0;

}; // MeshManager



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // MESHMANAGER_H_6014714286
