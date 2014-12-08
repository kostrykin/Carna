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

#include <Carna/base/qt/DisplayDetails.h>
#include <Carna/base/qt/DisplayVisualizationEnvironment.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/Visualization.h>
#include <QTimer>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// Display :: Details
// ----------------------------------------------------------------------------------

Display::Details::Details
    ( Visualization* module
    , view::SceneProvider& provider
    , qt::Display& self )

    : provider( provider )
    , enviroment( new DisplayVisualizationEnvironment( self ) )
    , module( module )
    , initialized( false )
    , self( self )
{
}


Display::Details::~Details()
{
}


void Display::Details::updateRendering()
{
    if( self.isVisible() )
    {
        self.update();
    }
}


void Display::Details::initializeModule()
{
    module->initialize( provider, *enviroment );

    connect
        ( &module->renderer()
        , SIGNAL( invalidated() )
        , this
        , SLOT( updateRendering() ) );

    CARNA_ASSERT( self.module().isInitialized() );
}


void Display::Details::updateTargetSize( int width, int height )
{
    CARNA_ASSERT( module->isInitialized() );

    width = std::max( 1, width );
    height = std::max( 1, height );

    module->renderer().setTargetSize( width, height );

    QTimer::singleShot( 0, &self, SLOT( updateGL() ) );
}



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna
