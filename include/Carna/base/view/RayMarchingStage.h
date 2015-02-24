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

#ifndef RAYMARCHINGSTAGE_H_6014714286
#define RAYMARCHINGSTAGE_H_6014714286

#include <Carna/base/view/GeometryStage.h>

/** \file   RayMarchingStage.h
  * \brief  Defines \ref Carna::base::view::RayMarchingStage.
  */

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// RayMarchingStage
// ----------------------------------------------------------------------------------

class CARNA_LIB RayMarchingStage : public GeometryStage< Renderable::DepthOrder< Renderable::ORDER_BACK_TO_FRONT > >
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    const static int GEOMETRY_TYPE = 2;

    const static unsigned int DEFAULT_SAMPLE_RATE = 100;

    RayMarchingStage();

    /** \brief
      * Sets number of samples per pixel and per segment.
      */
    void setSampleRate( unsigned int sampleRate );
    
    /** \brief
      * Tells number of samples per pixel and per segment.
      */
    unsigned int sampleRate() const;

    virtual void renderPass( RenderTask& rt, const Viewport& vp ) override;

protected:

    virtual void render( const Renderable& ) override;

    virtual void beginPass() = 0;

    virtual void finishPass() = 0;

}; // RayMarchingStage



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // RAYMARCHINGSTAGE_H_6014714286
