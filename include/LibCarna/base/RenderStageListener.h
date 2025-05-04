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

#ifndef RENDERSTAGELISTENER_H_6014714286
#define RENDERSTAGELISTENER_H_6014714286

#include <LibCarna/LibCarna.h>

/** \file   RenderStageListener.h
  * \brief  Defines \ref Carna::base::RenderStageListener.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// RenderStageListener
// ----------------------------------------------------------------------------------

/** \brief
  * Defines callbacks for \ref RenderStage lifetime events.
  *
  * \author Leonid Kostrykin
  * \date   2.4.15
  */
class LIBCARNA RenderStageListener
{

public:

    /** \brief
      * Deletes.
      */
    virtual ~RenderStageListener();
    
    /** \brief
      * Invoked right after \a rs has been initialized.
      */
    virtual void onRenderStageInitialized( RenderStage& rs ) = 0;
    
    /** \brief
      * Invoked right before \a rs is deleted.
      */
    virtual void onRenderStageDelete( RenderStage& rs ) = 0;

}; // RenderStageListener



}  // namespace Carna :: base

}  // namespace Carna

#endif // RENDERSTAGELISTENER_H_6014714286
