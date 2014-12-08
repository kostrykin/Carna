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

#ifndef XRAY_DETAILS_H_6014714286
#define XRAY_DETAILS_H_6014714286

/** \file   XRayDetails.h
  * \brief  Defines \ref Carna::VolumeRenderings::DRR::XRay::Details.
  */

#include <Carna/VolumeRenderings/DRR/XRay.h>

namespace Carna
{

namespace VolumeRenderings
{

namespace DRR
{



// ----------------------------------------------------------------------------------
// XRay :: Details
// ----------------------------------------------------------------------------------

/** \brief  PIMPL-implementation of \ref XRay
  *
  * \author Leonid Kostrykin
  * \date   14.9.12 - 2.10.12
  */
class XRay::Details
{

    NON_COPYABLE

public:

    explicit Details( const base::model::Scene& model ) : model( model )
    {
    }


    const base::model::Scene& model;


    double mu_water;

    double brightness;

    int lower_threshold;

    int upper_threshold;

    double upper_multiplier;

    double window_level;

    double window_width;

    double mask_contrast;

}; // XRay :: Details



}  // namespace Carna :: VolumeRenderings :: DRR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // XRAY_DETAILS_H_6014714286
