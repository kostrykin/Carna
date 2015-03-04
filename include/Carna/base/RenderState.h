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

#ifndef RENDERSTATE_H_6014714286
#define RENDERSTATE_H_6014714286

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <memory>

/** \file   RenderState.h
  * \brief  Defines \ref Carna::base::RenderState.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// RenderState
// ----------------------------------------------------------------------------------

class CARNA_LIB RenderState
{

    NON_COPYABLE

    struct Details;
    const std::unique_ptr< Details > pimpl;

protected:

    friend class GLContext;

    RenderState();

    void commit() const;

public:

    enum CullFace
    {

        /** \brief
          * All polygons will pass the cull test.
          */
        cullNone,
        
        /** \brief
          * Polygons recognized as back-faces will fail the cull test.
          */
        cullBack,
        
        /** \brief
          * Polygons recognized as front-faces will fail the cull test.
          */
        cullFront

    }; // CullFace

    RenderState( GLContext& );

    ~RenderState();

    void setDepthTest( bool dt );

    void setDepthWrite( bool dw );

    void setDepthTestFunction( int dtf );

    void setBlend( bool b );

    void setBlendFunction( const BlendFunction& bf );

    void setBlendEquation( int be );

    void setCullFace( CullFace cf );

    void setFrontFace( bool ccw );

private:

    void commitDepthTest() const;

    void commitDepthWrite() const;

    void commitDepthTestFunction() const;

    void commitBlend() const;

    void commitBlendFunction() const;

    void commitBlendEquation() const;

    void commitCullFace() const;

    void commitFrontFace() const;

}; // RenderState



}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERSTATE_H_6014714286
