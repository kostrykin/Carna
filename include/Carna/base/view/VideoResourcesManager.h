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

#ifndef VIDEORESOURCESMANAGER_H_6014714286
#define VIDEORESOURCESMANAGER_H_6014714286

/** \file   VideoResourcesManager.h
  * \brief  Defines \ref Carna::base::view::VideoResourcesManager.
  */

#include <Carna/base/noncopyable.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// VideoResourcesManager
// ----------------------------------------------------------------------------------

class CARNA_LIB VideoResourcesManager
{

    NON_COPYABLE

public:

    virtual ~VideoResourcesManager();

    virtual void uploadResources() = 0;

    virtual void deleteResources() = 0;

}; // VideoResourcesManager



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // VIDEORESOURCESMANAGER_H_6014714286
