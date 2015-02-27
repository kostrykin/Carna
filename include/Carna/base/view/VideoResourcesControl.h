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

#ifndef VIDEORESOURCESCONTROL_H_6014714286
#define VIDEORESOURCESCONTROL_H_6014714286

/** \file   VideoResourcesControl.h
  * \brief  Defines \ref Carna::base::view::VideoResourcesControl.
  */

#include <Carna/base/noncopyable.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// VideoResourcesControl
// ----------------------------------------------------------------------------------

class CARNA_LIB VideoResourcesControl
{

    NON_COPYABLE

public:

    virtual ~VideoResourcesControl();

    virtual void uploadResources() = 0;

    virtual void deleteResources() = 0;

}; // VideoResourcesControl



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // VIDEORESOURCESCONTROL_H_6014714286
