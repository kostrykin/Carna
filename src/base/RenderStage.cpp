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

#include <Carna/base/RenderStage.h>
#include <Carna/base/RenderStageListener.h>
#include <set>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// RenderStage :: Details
// ----------------------------------------------------------------------------------

struct RenderStage::Details
{
    Details();

    bool viewTransformFixed;
    bool enabled;
    FrameRenderer* fr;
    
    std::set< RenderStageListener* > listeners;
};


RenderStage::Details::Details()
    : viewTransformFixed( true )
    , enabled( true )
    , fr( nullptr )
{
}



// ----------------------------------------------------------------------------------
// RenderStage
// ----------------------------------------------------------------------------------

RenderStage::RenderStage()
    : pimpl( new Details() )
{
}


RenderStage::~RenderStage()
{
    /* Notify listeners that this stage is about to be deleted.
     */
    while( !pimpl->listeners.empty() )
    {
        RenderStageListener* const listener = *pimpl->listeners.begin();
        listener->onRenderStageDelete( *this );
    }
}

    
bool RenderStage::isInitialized() const
{
    return pimpl->fr != nullptr;
}


base::FrameRenderer& RenderStage::renderer()
{
    CARNA_ASSERT( isInitialized() );
    return *pimpl->fr;
}


const base::FrameRenderer& RenderStage::renderer() const
{
    CARNA_ASSERT( isInitialized() );
    return *pimpl->fr;
}


void RenderStage::reshape( FrameRenderer& fr, unsigned int width, unsigned int height )
{
    if( pimpl->fr == nullptr )
    {
        pimpl->fr = &fr;
        
        /* Notify listeners that this stage has been initialized. Iterate over a copy
         * of the listeners set to allow the listeners to detach themselves.
         */
        const std::set< RenderStageListener* > listeners( pimpl->listeners.begin(), pimpl->listeners.end() );
        for( auto itr = listeners.begin(); itr != listeners.end(); ++itr )
        {
            RenderStageListener* const listener = *itr;
            listener->onRenderStageInitialized( *this );
        }
    }
    else
    {
        CARNA_ASSERT( pimpl->fr == &fr );
    }
}


void RenderStage::setViewTransformFixed( bool viewTransformFixed )
{
    pimpl->viewTransformFixed = viewTransformFixed;
}


bool RenderStage::isViewTransformFixed() const
{
    return pimpl->viewTransformFixed;
}


void RenderStage::prepareFrame( Node& root )
{
    pimpl->viewTransformFixed = true;
}


bool RenderStage::isEnabled() const
{
    return pimpl->enabled;
}


void RenderStage::setEnabled( bool enabled )
{
    pimpl->enabled = enabled;
}


void RenderStage::addRenderStageListener( RenderStageListener& listener )
{
    pimpl->listeners.insert( &listener );
}


void RenderStage::removeRenderStageListener( RenderStageListener& listener )
{
    pimpl->listeners.erase( &listener );
}



}  // namespace Carna :: base

}  // namespace Carna
