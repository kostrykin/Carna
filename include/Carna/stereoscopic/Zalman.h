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

#ifndef ZALMAN_H_6014714286
#define ZALMAN_H_6014714286

/** \file   Zalman.h
  * \brief  Defines \ref Carna::stereoscopic::Zalman.
  */

#include <Carna/base/view/Renderer.h>

namespace Carna
{

namespace stereoscopic
{



// ----------------------------------------------------------------------------------
// Zalman
// ----------------------------------------------------------------------------------

/** \brief  Configures associating view to Zalman-compatible stereo mode.
  *
  * \author Leonid Kostrykin
  * \date   2011
  */
class CARNA_EXPORT Zalman : public base::view::Renderer::RenderMode
{

public:

    /** \brief  Instantiates.
      */
    explicit Zalman( base::view::Renderer& );

    /** \brief	Releases acquired resources.
      */
    virtual ~Zalman();


    virtual void paint() override;

    virtual void resizeBuffers
        ( unsigned int width
        , unsigned int height ) override;


protected:

    virtual void postInitialize() override;


private:

    struct Details;

    std::unique_ptr< Details > pimpl;

}; // Zalman



}  // namespace Carna :: stereoscopic

}  // namespace Carna

#endif // ZALMAN_H_6014714286
