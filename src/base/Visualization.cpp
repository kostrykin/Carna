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

#include <Carna/base/Visualization.h>
#include <Carna/base/VisualizationDetails.h>
#include <Carna/base/VisualizationEnvironment.h>
#include <Carna/base/CarnaException.h>
#include <QEvent>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Visualization :: Details :: FilteringInteractionStrategy
// ----------------------------------------------------------------------------------

class Visualization :: Details :: FilteringInteractionStrategy : public controller::InteractionStrategy
{

    NON_COPYABLE

public:

    FilteringInteractionStrategy( Visualization& );


    virtual void event( Visualization&, QEvent* ) override;


private:

    Visualization& self;

}; // Visualization :: Details :: FilteringInteractionStrategy


Visualization::Details::FilteringInteractionStrategy::FilteringInteractionStrategy( Visualization& self )
    : self( self )
{
}


void Visualization::Details::FilteringInteractionStrategy::event( Visualization& sourceModule, QEvent* event )
{
    event->setAccepted( false );

    if( self.pimpl->controllerFilters.empty() )
    {
        self.pimpl->controller->event( sourceModule, event );
    }
    else
    {
        for( auto it = self.pimpl->controllerFilters.begin(); it != self.pimpl->controllerFilters.end(); ++it )
        {
            controller::InteractionStrategy& filter = **it;

            filter.event( sourceModule, event );

            if( event->isAccepted() )
            {
                return;
            }
        }

        self.pimpl->controller->event( sourceModule, event );
    }
}



// ----------------------------------------------------------------------------------
// Visualization :: Details
// ----------------------------------------------------------------------------------

Visualization::Details::Details( Visualization& self, controller::InteractionStrategy* initialController )
    : currentEnvironment( nullptr )
    , controller( initialController )
    , filteredController( new FilteringInteractionStrategy( self ) )
{
}



// ----------------------------------------------------------------------------------
// Visualization
// ----------------------------------------------------------------------------------

Visualization::Visualization()
    : pimpl( new Details( *this, new controller::DefaultInteractionStrategy( *this ) ) )
{
}


Visualization::Visualization( controller::InteractionStrategy* initialController )
    : pimpl( new Details( *this, initialController ) )
{
    CARNA_ASSERT( initialController != nullptr );
}


Visualization::~Visualization()
{
    if( pimpl->currentEnvironment != nullptr )
    {
        environment().activateContext();
    }
}


void Visualization::initialize( view::SceneProvider& provider, VisualizationEnvironment& environment )
{
    CARNA_ASSERT( !this->isInitialized() );

    environment.activateContext();

    pimpl->currentEnvironment = &environment;
    pimpl->renderer.reset( createRenderer( provider ) );

    CARNA_ASSERT( this->isInitialized() );

 // invoke post-initialization callbacks

    for( auto callback_itr  = pimpl->postInitializationCallbacks.begin();
              callback_itr != pimpl->postInitializationCallbacks.end();
            ++callback_itr )
    {
        const PostInitializationCallback& invokeCallback = *callback_itr;
        invokeCallback();
    }

    pimpl->postInitializationCallbacks.clear();
}


bool Visualization::isInitialized() const
{
    return pimpl->renderer.get() != nullptr;
}


view::Renderer& Visualization::renderer()
{
    CARNA_ASSERT( this->isInitialized() );

    return *pimpl->renderer;
}


const view::Renderer& Visualization::renderer() const
{
    CARNA_ASSERT( this->isInitialized() );

    return *pimpl->renderer;
}


controller::InteractionStrategy& Visualization::controller()
{
    return *pimpl->filteredController;
}


const controller::InteractionStrategy& Visualization::controller() const
{
    return *pimpl->filteredController;
}


controller::InteractionStrategy& Visualization::rawController()
{
    return *pimpl->controller;
}


const controller::InteractionStrategy& Visualization::rawController() const
{
    return *pimpl->controller;
}


VisualizationEnvironment& Visualization::environment()
{
    CARNA_ASSERT( pimpl->currentEnvironment != nullptr );

    return *pimpl->currentEnvironment;
}


const VisualizationEnvironment& Visualization::environment() const
{
    CARNA_ASSERT( pimpl->currentEnvironment != nullptr );

    return *pimpl->currentEnvironment;
}


void Visualization::setController( controller::InteractionStrategy* newController )
{
    CARNA_ASSERT( newController != nullptr );

    pimpl->controller.reset( newController );

    CARNA_ASSERT( newController == &rawController() );
    CARNA_ASSERT( newController != &controller() );

    emit controllerExchanged();
    emit controllerExchanged( *newController );
}


void Visualization::doAfterInitialization( const Visualization::PostInitializationCallback& invokeCallback )
{
    if( pimpl->renderer.get() == nullptr )
    {
        pimpl->postInitializationCallbacks.push_back( invokeCallback );
    }
    else
    {
        invokeCallback();
    }
}


void Visualization::addControllerFilter( controller::InteractionStrategy& filter )
{
    pimpl->controllerFilters.push_front( &filter );
}


void Visualization::removeControllerFilter( controller::InteractionStrategy& filter )
{
    const auto it = std::find
        ( pimpl->controllerFilters.begin()
        , pimpl->controllerFilters.end()
        , &filter );

    CARNA_ASSERT( it != pimpl->controllerFilters.end() );

    pimpl->controllerFilters.erase( it );
}



}  // namespace Carna :: base

}  // namespace Carna
