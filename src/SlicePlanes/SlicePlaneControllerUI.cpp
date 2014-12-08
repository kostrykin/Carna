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

#include <Carna/base/view/glew.h>
#include <Carna/base/Math.h>
#include <Carna/SlicePlanes/SlicePlaneRenderer.h>
#include <Carna/SlicePlanes/SlicePlaneControllerUI.h>
#include <Carna/SlicePlanes/SlicePlaneControllerUIDetails.h>
#include <QLabel>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWheelEvent>
#include <QDoubleSpinBox>
#include <QDebug>

namespace Carna
{

namespace SlicePlanes
{



// ----------------------------------------------------------------------------------
// SlicePlaneControllerUI :: Details
// ----------------------------------------------------------------------------------

SlicePlaneControllerUI::Details::Details( SlicePlaneControllerUI& self )
    : self( self )
{
}


SlicePlaneControllerUI::Details::~Details()
{
}


void SlicePlaneControllerUI::Details::commit()
{
    base::Vector normal( lineEdit_n1->text().toDouble()
        , lineEdit_n2->text().toDouble()
        , lineEdit_n3->text().toDouble() );

    const double distance = std::abs( sbDistance->value() );

    const double border = std::min( std::abs( lineEdit_border->text().toDouble() ), 1. );

    bool failure = false;

    if( !base::Math::isEqual( normal.norm(), 0. ) )
    {

     // normalize

        normal /= normal.norm();

        lineEdit_border ->setText( QString::number( border ) );

     // update view

        failure = !self.renderer.setPlane( normal, distance, border );

    }
    else
    {
        failure = true;
    }

    laFailure->setText( !failure ? "OK" : "Invalid" );
}



// ----------------------------------------------------------------------------------
// SlicePlaneControllerUI
// ----------------------------------------------------------------------------------

SlicePlaneControllerUI::SlicePlaneControllerUI( SlicePlaneRenderer& renderer, QWidget* parent )
    : QWidget( parent )
    , renderer( renderer )
    , pimpl( new Details( *this ) )
{
    QFormLayout* const general = new QFormLayout();

    pimpl->lineEdit_n1     = new QLineEdit( this );
    pimpl->lineEdit_n2     = new QLineEdit( this );
    pimpl->lineEdit_n3     = new QLineEdit( this );
    pimpl->lineEdit_border = new QLineEdit( this );

    pimpl->sbDistance = new QDoubleSpinBox( this );
    pimpl->sbDistance->setMinimum( 0.01 );
    pimpl->sbDistance->setMaximum( unsigned( sqrt( 3. ) * 100 ) / 100. );
    pimpl->sbDistance->setSingleStep( 0.01 );
    pimpl->sbDistance->setDecimals( 2 );

    pimpl->laFailure = new QLabel( "OK" );
    pimpl->laFailure->setStyleSheet( "font-weight: bold;" );

    pimpl->lineEdit_n1    ->setText( QString::number( renderer.getNormal()[ 0 ] ) );
    pimpl->lineEdit_n2    ->setText( QString::number( renderer.getNormal()[ 1 ] ) );
    pimpl->lineEdit_n3    ->setText( QString::number( renderer.getNormal()[ 2 ] ) );
    pimpl->lineEdit_border->setText( QString::number( renderer.getBorderWidth() ) );

    pimpl->sbDistance->setValue( renderer.getDistance() );

    general->addRow( tr( "Normal.x:" ), pimpl->lineEdit_n1  );
    general->addRow( tr( "Normal.y:" ), pimpl->lineEdit_n2 );
    general->addRow( tr( "Normal.z:" ), pimpl->lineEdit_n3  );

    general->addRow( new QLabel( "" ) );

    general->addRow( tr( "Distance:" ), pimpl->sbDistance );

    general->addRow( new QLabel( "" ) );

    general->addRow( tr( "Border width:" ), pimpl->lineEdit_border );

    general->addRow( new QLabel( "" ) );

    general->addRow( "Definition:", pimpl->laFailure );

 // ----------------------------------------------------------------------------------

    connect
        ( pimpl->lineEdit_n1
        , SIGNAL( editingFinished() )
        , pimpl.get()
        , SLOT( commit() ) );

    connect
        ( pimpl->lineEdit_n2
        , SIGNAL( editingFinished() )
        , pimpl.get()
        , SLOT( commit() ) );

    connect
        ( pimpl->lineEdit_n3
        , SIGNAL( editingFinished() )
        , pimpl.get()
        , SLOT( commit() ) );

    connect
        ( pimpl->lineEdit_border
        , SIGNAL( editingFinished() )
        , pimpl.get()
        , SLOT( commit() ) );

    connect
        ( pimpl->sbDistance
        , SIGNAL( valueChanged( double ) )
        , pimpl.get()
        , SLOT( commit( double ) ) );

    // ----------------------------------------------------------------------------------

    general->addRow( "Definition:", pimpl->laFailure );

    this->setLayout( general );

    connect
        ( &renderer
        , SIGNAL( destroyed() )
        , this
        , SLOT( deleteLater() ) );
}


SlicePlaneControllerUI::~SlicePlaneControllerUI()
{
}



}  // namespace Carna :: SlicePlanes

}  // namespace Carna
