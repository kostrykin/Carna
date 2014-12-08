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

#include <Carna/base/qt/Object3DChooser.h>
#include <Carna/base/qt/Object3DChooserDetails.h>
#include <Carna/base/model/Scene.h>
#include <Carna/base/model/Object3D.h>
#include <Carna/base/model/Object3DEvent.h>
#include <Carna/base/CarnaException.h>
#include <QHBoxLayout>
#include <QComboBox>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// Object3DChooser :: Details
// ----------------------------------------------------------------------------------

Object3DChooser::Details::Details( Object3DChooser& self )
    : self( self )
    , cbObject3D( new QComboBox() )
    , selectedObject3D( nullptr )
    , ignoreSelectionChanges( false )
{
}


void Object3DChooser::Details::occasionallyRebuildList( const model::Object3DEvent& ev )
{
    if( ev.mightAffect( Flags< model::Object3DEvent::Feature >
           ( model::Object3DEvent::existence )
        .or( model::Object3DEvent::name )
        .or( model::Object3DEvent::shape ) ) )
    {
        rebuildList();
    }
}


void Object3DChooser::Details::rebuildList()
{
    ignoreSelectionChanges = true;

    cbObject3D->clear();

    cbObject3D->addItem( "( none )" );

    unsigned int prev_selected_object_new_index = 0;

    for( unsigned int i = 0; i < self.model.objectsCount(); ++i )
    {
        if( &( self.model.objectByIndex( i ) ) == selectedObject3D )
        {
            prev_selected_object_new_index = i + 1;
        }
        cbObject3D->addItem( self.model.objectByIndex( i ).icon()
                           , QString::fromStdString( self.model.objectByIndex( i ).name() ) );
    }

    cbObject3D->setCurrentIndex( prev_selected_object_new_index );

    ignoreSelectionChanges = false;

    processSelectionUpdate( prev_selected_object_new_index );
}


void Object3DChooser::Details::processSelectionUpdate( int item_index )
{
    if( ignoreSelectionChanges )
    {
        return;
    }

    if( item_index == 0 )
    {
        if( selectedObject3D )
        {
            selectedObject3D = 0;

            emit self.selectionDissolved();
        }
    }
    else
    {
        model::Object3D* const newSelectedObject3D = &( self.model.objectByIndex( item_index - 1 ) );
        if( selectedObject3D != newSelectedObject3D )
        {
            selectedObject3D = newSelectedObject3D;

            emit self.selectionChanged( *selectedObject3D );
            emit self.selectionChanged();
        }
    }
}



// ----------------------------------------------------------------------------------
// Object3DChooser
// ----------------------------------------------------------------------------------

Object3DChooser::Object3DChooser( model::Scene& model, QWidget* parent )
    : QWidget( parent )
    , model( model )
    , pimpl( new Details( *this ) )
{
    QHBoxLayout* const layout = new QHBoxLayout();
    this->setLayout( layout );

    pimpl->cbObject3D->setEditable( false );

    layout->addWidget( pimpl->cbObject3D );
    layout->setContentsMargins( 0, 0, 0, 0 );

    connect
        ( &model
        , SIGNAL( objectsChanged( const Carna::base::model::Object3DEvent& ) )
        , pimpl.get()
        , SLOT( occasionallyRebuildList( const Carna::base::model::Object3DEvent& ) ) );

    connect
        ( pimpl->cbObject3D
        , SIGNAL( currentIndexChanged( int ) )
        , pimpl.get()
        , SLOT( processSelectionUpdate( int ) ) );

    pimpl->rebuildList();
}


Object3DChooser::~Object3DChooser()
{
}


bool Object3DChooser::isObject3DSelected() const
{
    return pimpl->selectedObject3D != nullptr;
}


model::Object3D& Object3DChooser::selectedObject3D() const
{
    CARNA_ASSERT_EX( isObject3DSelected(), "nothing is selected" );

    return *pimpl->selectedObject3D;
}


void Object3DChooser::selectNone()
{
    pimpl->cbObject3D->setCurrentIndex( 0 );
}


void Object3DChooser::selectObject( model::Object3D& requestedObject )
{
    for( unsigned int objectIndex = 0; objectIndex < model.objectsCount(); ++objectIndex )
    {
        if( &model.objectByIndex( objectIndex ) == &requestedObject )
        {
            pimpl->cbObject3D->setCurrentIndex( objectIndex + 1 );
            return;
        }
    }

    CARNA_FAIL( "Object3D instance is not part of associated model" );

    CARNA_ASSERT( isObject3DSelected() );
    CARNA_ASSERT( &selectedObject3D() == &requestedObject );
}



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna
