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

#ifndef RENDERSTAGESEQUENCE_H_0874895466
#define RENDERSTAGESEQUENCE_H_0874895466

#include <LibCarna/LibCarna.hpp>
#include <LibCarna/base/Aggregation.hpp>
#include <LibCarna/base/noncopyable.hpp>
#include <memory>

/** \file   RenderStageSequence.h
  * \brief  Defines \ref LibCarna::base::RenderStageSequence.
  */

namespace LibCarna
{

namespace base
{



// ----------------------------------------------------------------------------------
// RenderStageSequence
// ----------------------------------------------------------------------------------

/** \brief
  * Represents a \ref RenderStage "rendering stages" sequence.
  *
  * \author Leonid Kostrykin
  */
class LIBCARNA RenderStageSequence
{

    NON_COPYABLE
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Instantiates.
      */
    RenderStageSequence();

    /** \brief
      * Deletes all stages contained by the sequence.
      */
    virtual ~RenderStageSequence();

    /** \brief
      * Tells number of stages contained by the sequence.
      */
    std::size_t stages() const;
    
    /** \brief
      * Appends \a stage to the rendering stages sequence.
      */
    void appendStage( RenderStage* stage );
    
    /** \brief
      * Deletes all stages from the rendering stages sequence.
      */
    virtual void clearStages();
    
    /** \brief
      * References the stage at \a position within the rendering stages sequence.
      */
    RenderStage& stageAt( std::size_t position ) const;

    /** \brief
      * References the first \a RenderStage within the rendering stages sequence.
      * This method performs a linear search. Returns \ref Aggregation::NULL_PTR if
      * \a RenderStage is not found withing hte rendering stages sequence.
      */
    template< typename RenderStage >
    Aggregation< RenderStage > findStage() const;
    
protected:

    /** \brief
      * Assumes that the ownership over all stages has been taken over by someone
      * else. Clears the stages sequence without deleting the stages.
      */
    void releaseStages();

}; // RenderStageSequence


template< typename RenderStage >
Aggregation< RenderStage > RenderStageSequence::findStage() const
{
    for( std::size_t index = 0; index < stages(); ++index )
    {
        RenderStage* const rs = dynamic_cast< RenderStage* >( &stageAt( index ) );
        if( rs != nullptr )
        {
            return Aggregation< RenderStage >( *rs );
        }
    }
    return Aggregation< RenderStage >::NULL_PTR;
}



}  // namespace LibCarna :: base

}  // namespace LibCarna

#endif // RENDERSTAGESEQUENCE_H_0874895466
