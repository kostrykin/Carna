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

#include <Carna/base/controller/InteractionStrategy.h>
#include <Carna/base/model/Object3D.h>
#include <Carna/base/model/Volume.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/view/glew.h>
#include <Carna/base/view/FramebufferObject.h>
#include <Carna/base/view/Renderer.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/view/ShaderBundle.h>
#include <Carna/base/view/Shader.h>
#include <Carna/base/qt/Display.h>
#include <Carna/base/qt/DisplayDetails.h>
#include <Carna/base/Visualization.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Singleton.h>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTimer>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// DisplaysPool
// ----------------------------------------------------------------------------------

struct DisplaysPool : public base::Singleton< DisplaysPool >
{

    Display* enter( Display* instance )
    {
        Display* const existingInstance =
            ( instances.empty() ? nullptr : *instances.begin() );

        instances.insert( instance );

        return existingInstance;
    }

    void leave( Display* instance )
    {
        instances.erase( instance );
    }


private:

    std::set< Display* > instances;

}; // DisplaysPool



// ----------------------------------------------------------------------------------
// initializeDisplay
// ----------------------------------------------------------------------------------

static void initializeDisplay( Display* display )
{
    display->setMouseTracking( true );
}



// ----------------------------------------------------------------------------------
// Display
// ----------------------------------------------------------------------------------


Display::Display
    ( Visualization* module
    , view::SceneProvider& provider
    , QWidget* parent )

    : QGLWidget( parent, DisplaysPool::instance().enter( this ) )
    , pimpl( new Details( module, provider, *this ) )
{
    CARNA_ASSERT( !module->isInitialized() );

    initializeDisplay( this );
}

Display::Display
    ( view::SceneProvider& provider
    , QWidget* parent )
    
    : QGLWidget( parent, DisplaysPool::instance().enter( this ) )
    , pimpl( new Details( nullptr, provider, *this ) )
{
    initializeDisplay( this );
}


Display::~Display()
{
    DisplaysPool::instance().leave( this );
}


bool Display::hasModule() const
{
    return pimpl->module.get() != nullptr;
}


void Display::setModule( Visualization* module )
{
    CARNA_ASSERT( module != nullptr );
    CARNA_ASSERT( !module->isInitialized() );

    pimpl->module.reset( module );

    if( pimpl->initialized )
    {
        pimpl->initializeModule();
        pimpl->updateTargetSize( width(), height() );
    }
}


Visualization& Display::module()
{
    return *pimpl->module;
}


const Visualization& Display::module() const
{
    return *pimpl->module;
}


void Display::initializeGL()
{
    CARNA_GLEW_INIT;

    pimpl->initialized = true;
    if( hasModule() )
    {
        pimpl->initializeModule();
    }
}


void Display::resizeGL( int width, int height )
{
    if( hasModule() )
    {
        pimpl->updateTargetSize( width, height );
    }
}


void Display::paintGL()
{
    CARNA_ASSERT( module().isInitialized() );

    pimpl->module->renderer().paint();
}


void Display::mousePressEvent( QMouseEvent* ev )
{
    if( hasModule() )
    {
        ev->ignore();
        pimpl->module->controller().event( *pimpl->module, ev );
    }
}


void Display::mouseMoveEvent( QMouseEvent* ev )
{
    if( hasModule() )
    {
        ev->ignore();
        pimpl->module->controller().event( *pimpl->module, ev );
    }
}


void Display::mouseReleaseEvent( QMouseEvent* ev )
{
    if( hasModule() )
    {
        ev->ignore();
        pimpl->module->controller().event( *pimpl->module, ev );
    }
}


void Display::wheelEvent( QWheelEvent* ev )
{
    if( hasModule() )
    {
        ev->ignore();
        pimpl->module->controller().event( *pimpl->module, ev );
    }
}



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna
