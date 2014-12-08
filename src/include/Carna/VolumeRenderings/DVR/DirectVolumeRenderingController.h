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

#ifndef DVR_DIRECTVOLUMERENDERING_DETAILS_CONTROLLER_H_6014714286
#define DVR_DIRECTVOLUMERENDERING_DETAILS_CONTROLLER_H_6014714286

/** \file   DirectVolumeRenderingController.h
  * \brief  Defines \ref Carna::VolumeRenderings::DVR::DirectVolumeRenderingController
  */

#include <Carna/VolumeRenderings/DVR/DirectVolumeRenderingDetails.h>
#include <Carna/base/controller/InteractionStrategy.h>

namespace Carna
{

namespace VolumeRenderings
{

namespace DVR
{



// ----------------------------------------------------------------------------------
// DVR :: Details :: Controller
// ----------------------------------------------------------------------------------

class DirectVolumeRendering :: Details :: Controller : public base::controller::InteractionStrategy
{

    NON_COPYABLE

public:

    explicit Controller( DirectVolumeRendering& );


    virtual void event( base::Visualization& sourceModule, QEvent* event ) override;


private:

    DirectVolumeRendering& self;

}; // DVR :: DirectVolumeRendering :: Details :: Controller



}  // namespace Carna :: VolumeRenderings :: DVR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna

#endif // DVR_DIRECTVOLUMERENDERING_DETAILS_CONTROLLER_H_6014714286
