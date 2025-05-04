/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include <LibCarna/base/RenderStageSequence.h>
#include <LibCarna/base/RenderStage.h>
#include <vector>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// RenderStageSequence :: Details
// ----------------------------------------------------------------------------------

struct RenderStageSequence::Details
{
    std::vector< RenderStage* > stages;
};



// ----------------------------------------------------------------------------------
// RenderStageSequence
// ----------------------------------------------------------------------------------

RenderStageSequence::RenderStageSequence()
    : pimpl( new Details() )
{
}


RenderStageSequence::~RenderStageSequence()
{
    clearStages();
}


std::size_t RenderStageSequence::stages() const
{
    return pimpl->stages.size();
}


void RenderStageSequence::appendStage( RenderStage* stage )
{
    pimpl->stages.push_back( stage );
}


void RenderStageSequence::clearStages()
{
    std::for_each( pimpl->stages.begin(), pimpl->stages.end(), std::default_delete< RenderStage >() );
    pimpl->stages.clear();
}


RenderStage& RenderStageSequence::stageAt( std::size_t position ) const
{
    LIBCARNA_ASSERT( position < stages() );
    return *pimpl->stages[ position ];
}


void RenderStageSequence::releaseStages()
{
    pimpl->stages.clear();
}



}  // namespace Carna :: base

}  // namespace Carna
