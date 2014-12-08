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

#ifndef VISUALIZATION_DETAILS_H_6014714286
#define VISUALIZATION_DETAILS_H_6014714286

/** \file   VisualizationDetails.h
  * \brief  Defines \ref Carna::base::Visualization::Details.
  */

#include <Carna/base/Visualization.h>
#include <Carna/base/controller/DefaultInteractionStrategy.h>
#include <Carna/base/view/Renderer.h>
#include <vector>
#include <list>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Visualization :: Details
// ----------------------------------------------------------------------------------

class Visualization :: Details
{

    NON_COPYABLE

public:

    Details( Visualization& self, controller::InteractionStrategy* initialController );


    VisualizationEnvironment* currentEnvironment;

    std::list< controller::InteractionStrategy* > controllerFilters;

    std::unique_ptr< view::Renderer > renderer;

    std::unique_ptr< controller::InteractionStrategy > controller;

    std::vector< const Visualization::PostInitializationCallback > postInitializationCallbacks;


    class FilteringInteractionStrategy;
    const std::unique_ptr< FilteringInteractionStrategy > filteredController;

}; // Visualization :: Details



}  // namespace Carna :: base

}  // namespace Carna

#endif // VISUALIZATION_DETAILS_H_6014714286
