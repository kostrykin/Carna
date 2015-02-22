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

#include <Carna/base/view/FrameRenderer.h>
#include <Carna/base/view/RenderTask.h>
#include <Carna/base/view/RenderStage.h>
#include <Carna/base/view/Node.h>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// FrameRenderer
// ----------------------------------------------------------------------------------

FrameRenderer::FrameRenderer( unsigned int width, unsigned int height )
    : myWidth( width )
    , myHeight( height )
    , reshaped( true )
{
}


FrameRenderer::~FrameRenderer()
{
    clearStages();
}


std::size_t FrameRenderer::stages() const
{
    return myStages.size();
}


void FrameRenderer::appendStage( RenderStage* rs )
{
    myStages.push_back( rs );
}

    
void FrameRenderer::clearStages()
{
    std::for_each( myStages.begin(), myStages.end(), std::default_delete< RenderStage >() );
    myStages.clear();
}


RenderStage& FrameRenderer::stageAt( std::size_t position ) const
{
    return *myStages[ position ];
}


unsigned int FrameRenderer::width() const
{
    return myWidth;
}


unsigned int FrameRenderer::height() const
{
    return myHeight;
}


void FrameRenderer::reshape( unsigned int width, unsigned int height )
{
    myWidth = width;
    myHeight = height;
    reshaped = true;
}


void FrameRenderer::render( Camera& cam, Node& root ) const
{
    // update world transforms
    root.updateWorldTransform();

    // reshape render stages' buffers
    for( auto rsItr = myStages.begin(); rsItr != myStages.end(); ++rsItr )
    {
        RenderStage& rs = **rsItr;
        
        // ensure buffers are properly sized
        if( reshaped || !rs.isInitialized() )
        {
            rs.reshape( myWidth, myHeight );
        }

        // notify stages of beginning frame
        rs.prepareFrame( root );
    }
    
    // mark that all buffer sizes have been established
    reshaped = false;
    
    // render frame
    RenderTask task( *this, cam.projection(), cam.viewTransform() );
    task.render();
}



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna
