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

#ifndef COMPOSITIONSTAGE_H_6014714286
#define COMPOSITIONSTAGE_H_6014714286

#include <Carna/base/RenderStage.h>

/** \file   CompositionStage.h
  * \brief  Defines \ref Carna::presets::CompositionStage.
  */

namespace Carna
{

namespace presets
{



// ----------------------------------------------------------------------------------
// CompositionStage
// ----------------------------------------------------------------------------------

class CARNA_LIB CompositionStage : public base::RenderStage
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    enum CompositionMode
    {
        interleave,
        aside
    };

    CompositionStage( CompositionMode compositionMode );

    virtual ~CompositionStage();
    
    void setCompositionMode( CompositionMode compositionMode );
    
    CompositionMode compositionMode() const;
    
    void setCompositionSwap( bool );
    
    bool isCompositionSwapped() const;

    virtual void reshape( const base::FrameRenderer& fr, const base::Viewport& vp ) override;

    virtual bool isInitialized() const override;
    
    virtual void prepareFrame( base::Node& root ) override;

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;
        
protected:

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp
        , bool isFirstInvocation
        , bool isFirstSource ) = 0;

}; // CompositionStage



}  // namespace Carna :: presets

}  // namespace Carna

#endif // COMPOSITIONSTAGE_H_6014714286
