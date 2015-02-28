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

#ifndef VIDEORESOURCECONTROL_H_6014714286
#define VIDEORESOURCECONTROL_H_6014714286

/** \file   VideoResourceControl.h
  * \brief  Defines \ref Carna::base::view::VideoResourceControl.
  */

#include <Carna/base/noncopyable.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// VideoResourceControl
// ----------------------------------------------------------------------------------

/** \brief
  * Controls the lifetime of a particular resource that resides in video memory.
  */
class CARNA_LIB VideoResourceControl
{

    NON_COPYABLE

public:

    virtual ~VideoResourceControl();

    /** \brief
      * Ensures that the resource is uploaded to video memory. There is no necessity
      * that the resource is uploaded again in case it has somehow been uploaded earlier already.
      */
    virtual void uploadResource() = 0;
    
    /** \brief
      * Denotes that the resource may be removed from video memory as soon as this object encounters it safe to do so.
      */
    virtual void deleteResource() = 0;

    virtual bool isSameResource( const VideoResourceControl& ) const = 0;

}; // VideoResourceControl



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // VIDEORESOURCESCONTROL_H_6014714286
