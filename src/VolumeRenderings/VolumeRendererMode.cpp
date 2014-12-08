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

#include <Carna/VolumeRenderings/VolumeRendererMode.h>
#include <Carna/VolumeRenderings/VolumeRenderer.h>
#include <Carna/base/view/glew.h>
#include <Carna/base/view/SceneProvider.h>
#include <Carna/base/CarnaException.h>
#include <QWidget>
#include <QLayout>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// VolumeRendererMode
// ----------------------------------------------------------------------------------

VolumeRendererMode::VolumeRendererMode( const std::string& name )
    : QObject()
    , name( name )
    , currentRenderer( 0 )
{
}


VolumeRendererMode::~VolumeRendererMode()
{
}


void VolumeRendererMode::paintObjects3D() const
{
    using namespace base;

    CARNA_ASSERT( isInitialized() );

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();

    glLoadMatrix( currentRenderer->getObjectsModelView() );
    currentRenderer->paintObjects3D();

    glPopMatrix();
}


void VolumeRendererMode::prepareForRenderer( VolumeRenderer& renderer )
{
    CARNA_ASSERT_EX
        ( !isInitialized()
        , "VolumeRendererMode is already associated with "
            << ( &renderer == currentRenderer ? "this" : "another" )
            << " VolumeRenderer." );

    this->currentRenderer = &renderer;
}


void VolumeRendererMode::start()
{
    CARNA_ASSERT( isInitialized() );
}


void VolumeRendererMode::stop()
{
    CARNA_ASSERT( isInitialized() );
}


void VolumeRendererMode::resizeBuffers( int width, int height )
{
    CARNA_ASSERT( isInitialized() );
}


VolumeRenderer& VolumeRendererMode::renderer()
{
    CARNA_ASSERT_EX( isInitialized(), "VolumeRendererMode has not been installed yet" );

    return *currentRenderer;
}


void VolumeRendererMode::invalidate()
{
    if( isInitialized() )
    {
        renderer().invalidate();
    }
}



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
