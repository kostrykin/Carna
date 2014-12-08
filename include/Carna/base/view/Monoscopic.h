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

#ifndef MONOSCOPIC_H_6014714286
#define MONOSCOPIC_H_6014714286

/** \file   Monoscopic.h
  * \brief  Defines \ref Carna::base::view::Monoscopic.
  */

#include <Carna/base/view/Renderer.h>



namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// Monoscopic
// ----------------------------------------------------------------------------------

/** \brief  Configures associating view to perform simple monoscopic rendering.
  *
  * \author Leonid Kostrykin
  * \date   2011
  */
class CARNA_EXPORT Monoscopic : public Renderer::RenderMode
{

public:

    /** \brief  Instantiates.
      */
    explicit Monoscopic( Renderer& );

    /** \brief	Releases acquired resources.
      */
    virtual ~Monoscopic();


    virtual void paint() override;

    virtual void resizeBuffers
        ( unsigned int width
        , unsigned int height ) override;


protected:

    virtual void postInitialize() override;


private:

    struct Details;

    std::unique_ptr< Details > pimpl;

}; // Monoscopic



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // MONOSCOPIC_H_6014714286
