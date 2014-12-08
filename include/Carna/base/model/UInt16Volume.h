/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#ifndef UINT16VOLUME_H_6014714286
#define UINT16VOLUME_H_6014714286

/** \file   UInt16Volume.h
  *
  * \brief  Defines \ref Carna::base::model::UInt16Volume.
  *
  * \author Leonid Kostrykin
  * \date   25.7.11
  */

#include <Carna/base/model/GenericVolume.h>
#include <Carna/base/view/UInt16TextureUploader.h>
#include <cstdint>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// UInt16Volume
// ----------------------------------------------------------------------------------

/** \brief  Defines 16bit unsigned integer volume data.
  */
typedef GenericVolume< uint16_t, view::UInt16TextureUploader > UInt16Volume;



}  // namesapce Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // UINT16VOLUME_H_6014714286
