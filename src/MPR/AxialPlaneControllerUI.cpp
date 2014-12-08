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

#include <Carna/MPR/AxialPlaneControllerUI.h>
#include <Carna/MPR/AxialPlaneControllerUIDetails.h>
#include <Carna/MPR/AxialPlaneRenderer.h>
#include <Carna/base/model/Object3D.h>
#include <Carna/base/model/Object3DEvent.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/qt/Object3DChooser.h>
#include <Carna/base/CarnaException.h>
#include <QSlider>
#include <QFormLayout>
#include <QLabel>

namespace Carna
{

namespace MPR
{



// ----------------------------------------------------------------------------------
// MPR :: AxialPlaneControllerUI :: Details
// ----------------------------------------------------------------------------------

AxialPlaneControllerUI::Details::Details( AxialPlaneControllerUI& self )
    : self( self )
    , slHuv0( new QSlider( Qt::Horizontal ) )
    , laHuv0( new QLabel() )
    , slHuv1( new QSlider( Qt::Horizontal ) )
    , laHuv1( new QLabel() )
    , cbFollowedObject( new base::qt::Object3DChooser( self.model ) )
    , deletedWhenEmpty( false )
{
}


void AxialPlaneControllerUI::Details::occasionallyFollow( const base::model::Object3DEvent& ev )
{
    if( ev.mightAffect( base::model::Object3DEvent::position ) )
    {
        follow();
    }
}


void AxialPlaneControllerUI::Details::setHuv0( int huv )
{
    laHuv0->setText( QString::number( slHuv0->value() ) );

    for( Renderers::iterator it = renderers.begin(); it != renderers.end(); ++it )
    {
        ( **it ).setMaskMinHuv( huv );
    }
}


void AxialPlaneControllerUI::Details::setHuv1( int huv )
{
    laHuv1->setText( QString::number( slHuv1->value() ) );

    for( Renderers::iterator it = renderers.begin(); it != renderers.end(); ++it )
    {
        ( **it ).setMaskMaxHuv( huv );
    }
}


void AxialPlaneControllerUI::Details::follow()
{
    if( cbFollowedObject->isObject3DSelected() )
    {
        const base::model::Object3D& obj = cbFollowedObject->selectedObject3D();

        for( Renderers::iterator it = renderers.begin(); it != renderers.end(); ++it )
        {
            ( **it ).setIntersection( obj.position().toVolumeUnits() );
        }
    }
}


void AxialPlaneControllerUI::Details::removeRenderer( QObject* object )
{
    self.removeRenderer( *static_cast< AxialPlaneRenderer* >( object ) );
}



// ----------------------------------------------------------------------------------
// AxialPlane :: AxialPlaneControllerUI
// ----------------------------------------------------------------------------------

AxialPlaneControllerUI::AxialPlaneControllerUI( base::model::Scene& model )
    : model( model )
    , pimpl( new Details( *this ) )
{
    QFormLayout* const layout = new QFormLayout();

 // min slider

    QWidget* const sliderParent0 = new QWidget();
    QHBoxLayout* const sliderParentLayout0 = new QHBoxLayout();
    sliderParent0->setLayout( sliderParentLayout0 );

    sliderParentLayout0->addWidget( pimpl->slHuv0 );
    sliderParentLayout0->addWidget( pimpl->laHuv0 );

    layout->addRow( "&Mask Min. HUV:", sliderParent0 );

    pimpl->slHuv0->setMinimum( -1024 );
    pimpl->slHuv0->setMaximum(  3071 );
    pimpl->slHuv0->setValue( 0 );
    pimpl->laHuv0->setText( QString().setNum( pimpl->slHuv0->value() ) );
    pimpl->laHuv0->setMinimumWidth( 30 );
    pimpl->laHuv0->setAlignment( Qt::AlignRight );

    pimpl->slHuv0->setTickPosition( QSlider::TicksAbove );
    pimpl->slHuv0->setSingleStep( 50 );
    pimpl->slHuv0->setPageStep( 200 );

    QObject::connect( pimpl->slHuv0, SIGNAL( valueChanged( int ) ), pimpl.get(), SLOT( setHuv0( int ) ) );

 // max slider

    QWidget* const sliderParent1 = new QWidget();
    QHBoxLayout* const sliderParentLayout1 = new QHBoxLayout();
    sliderParent1->setLayout( sliderParentLayout1 );

    sliderParentLayout1->addWidget( pimpl->slHuv1 );
    sliderParentLayout1->addWidget( pimpl->laHuv1 );

    layout->addRow( "&Mask Max. HUV:", sliderParent1 );

    pimpl->slHuv1->setMinimum( -1024 );
    pimpl->slHuv1->setMaximum(  3071 );
    pimpl->slHuv1->setValue( 0 );
    pimpl->laHuv1->setText( QString().setNum( pimpl->slHuv1->value() ) );
    pimpl->laHuv1->setMinimumWidth( 30 );
    pimpl->laHuv1->setAlignment( Qt::AlignRight );

    pimpl->slHuv1->setTickPosition( QSlider::TicksAbove );
    pimpl->slHuv1->setSingleStep( 50 );
    pimpl->slHuv1->setPageStep( 200 );

    QObject::connect( pimpl->slHuv1, SIGNAL( valueChanged( int ) ), pimpl.get(), SLOT( setHuv1( int ) ) );

 // other UI elements

    layout->addRow( "&Follow:", pimpl->cbFollowedObject );

    this->setLayout( layout );

    QObject::connect( &model, SIGNAL( objectsChanged( const Carna::base::model::Object3DEvent& ) ), pimpl.get(), SLOT( occasionallyFollow( const Carna::base::model::Object3DEvent& ) ) );
    QObject::connect( pimpl->cbFollowedObject, SIGNAL( selectionChanged() ), pimpl.get(), SLOT( follow() ) );

}


AxialPlaneControllerUI::~AxialPlaneControllerUI()
{
}


void AxialPlaneControllerUI::addRenderer( AxialPlaneRenderer& renderer )
{
    if( pimpl->renderers.empty() )
    {
        pimpl->slHuv0->setValue( renderer.minimalMaskHuv() );
        pimpl->slHuv1->setValue( renderer.maximalMaskHuv() );
    }
    else
    {
        renderer.setMaskMinHuv( pimpl->slHuv0->value() );
        renderer.setMaskMaxHuv( pimpl->slHuv1->value() );
    }
    pimpl->renderers.insert( &renderer );

    connect( &renderer, SIGNAL( destroyed( QObject* ) ), pimpl.get(), SLOT( removeRenderer( QObject* ) ) );
}


void AxialPlaneControllerUI::removeRenderer( AxialPlaneRenderer& renderer )
{
    pimpl->renderers.erase( &renderer );

    if( isDeletedWhenEmpty() && pimpl->renderers.empty() )
    {
        this->deleteLater();
    }
}


bool AxialPlaneControllerUI::isDeletedWhenEmpty() const
{
    return pimpl->deletedWhenEmpty;
}


void AxialPlaneControllerUI::setDeletedWhenEmpty( bool deletedWhenEmpty )
{
    pimpl->deletedWhenEmpty = deletedWhenEmpty;

    CARNA_ASSERT( isDeletedWhenEmpty() == deletedWhenEmpty );
}



}  // namespace MPR

}  // namespace Carna
