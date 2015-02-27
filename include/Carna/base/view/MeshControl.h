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

#ifndef MESHCONTROL_H_6014714286
#define MESHCONTROL_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/view/VideoResourcesControl.h>
#include <memory>

/** \file   MeshControl.h
  * \brief  Defines \ref Carna::base::view::MeshControl.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// MeshControl
// ----------------------------------------------------------------------------------
    
/** \brief
  * Abstract base \ref VideoResourcesControl for \ref MeshBase instances.
  *
  * Only \ref createMesh must be implemented s.t. it creates a new mesh instance.
  *
  * \author Leonid Kostrykin
  * \date 25.2.2015
  */
class CARNA_LIB MeshControl : public VideoResourcesControl
{

    std::unique_ptr< MeshBase > myMesh;

public:

    virtual void uploadResources() override;

    virtual void deleteResources() override;

    const MeshBase& mesh() const;

protected:

    virtual MeshBase* createMesh() = 0;

}; // MeshControl



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // MESHCONTROL_H_6014714286
