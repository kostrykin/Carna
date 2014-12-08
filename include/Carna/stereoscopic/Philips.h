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

#ifndef PHILIPS_H_6014714286
#define PHILIPS_H_6014714286

/** \file   Philips.h
  * \brief  Defines \ref Carna::stereoscopic::Philips
  */

#include <Carna/base/view/Renderer.h>

namespace Carna
{

namespace stereoscopic
{



// ----------------------------------------------------------------------------------
// Philips
// ----------------------------------------------------------------------------------

/** \brief  Configures associating view to Philips-compatible stereo mode.
  *
  * \author Leonid Kostrykin
  * \date   2011
  */
class CARNA_EXPORT Philips : public base::view::Renderer::RenderMode
{

public:

    /** \brief  Instantiates.
      */
    explicit Philips( base::view::Renderer& );

    /** \brief	Releases acquired resources.
      */
    virtual ~Philips();


    virtual void paint() override;

    virtual void resizeBuffers
        ( unsigned int width
        , unsigned int height ) override;


protected:

    virtual void postInitialize() override;


private:

    struct Details;

    std::unique_ptr< Details > pimpl;

}; // Philips



}  // namespace Carna :: stereoscopic

}  // namespace Carna

#endif // PHILIPS_H_6014714286
