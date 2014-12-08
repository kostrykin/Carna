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

#include <Carna/base/qt/RotationEdit.h>
#include <Carna/base/qt/RotationView.h>
#include <Carna/base/Transformation.h>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QResizeEvent>
#include <QApplication>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// RotationEdit
// ----------------------------------------------------------------------------------

RotationEdit::RotationEdit( QWidget* parent )
    : QWidget( parent )
    , view( new RotationView() )
    , sbPan( new QSpinBox() )
    , sbTilt( new QSpinBox() )
    , sbSpin( new QSpinBox() )
{
    QHBoxLayout* layout = new QHBoxLayout();

    layout->addWidget( view );
    
    QWidget* form_widget = new QWidget();

    QFormLayout* form_layout = new QFormLayout();

    sbPan ->setMinimum(   0 );
    sbPan ->setMaximum( 359 );
    sbTilt->setMinimum( -90 );
    sbTilt->setMaximum( +90 );
    sbSpin->setMinimum(   0 );
    sbSpin->setMaximum( 359 );

    sbPan ->setValue( 0 );
    sbTilt->setValue( 0 );
    sbSpin->setValue( 0 );

    sbPan ->setWrapping( true );
    sbSpin->setWrapping( true );

    connect( sbPan , SIGNAL( valueChanged( int ) ), this, SLOT( stateChanged( int ) ) );
    connect( sbTilt, SIGNAL( valueChanged( int ) ), this, SLOT( stateChanged( int ) ) );
    connect( sbSpin, SIGNAL( valueChanged( int ) ), this, SLOT( stateChanged( int ) ) );

    form_layout->addRow( "Pan:" , sbPan  );
    form_layout->addRow( "Tilt:", sbTilt );
    form_layout->addRow( "Spin:", sbSpin );

    form_widget->setLayout( form_layout );

    layout->addWidget( form_widget );

    this->setLayout( layout );

    layout->setContentsMargins( 0, 0, 0, 0 );
    form_layout->setContentsMargins( 0, 0, 0, 0 );
}


void RotationEdit::resizeEvent( QResizeEvent* ev )
{
    unsigned int h = ev->size().height();
    view->resize( h, h );
}


void RotationEdit::mousePressEvent( QMouseEvent* ev )
{
    lastMousePos = ev->pos();
    mousePressPos = QCursor::pos();
    QApplication::setOverrideCursor( Qt::BlankCursor );
}


void RotationEdit::mouseReleaseEvent( QMouseEvent* ev )
{
    QCursor::setPos( mousePressPos );
    QApplication::restoreOverrideCursor();
}


void RotationEdit::mouseMoveEvent( QMouseEvent* ev )
{
    const int dx = ( ev->x() - lastMousePos.x() )/* * ( Config::getInstance().invertXAxis ? -1 :  1 )*/;
    const int dy = ( ev->y() - lastMousePos.y() )/* * ( Config::getInstance().invertYAxis ?  1 : -1 )*/;

    lastMousePos = ev->pos();

    sbPan ->setValue( sbPan ->value() + dx );
    sbTilt->setValue( sbTilt->value() + dy );
}


void RotationEdit::wheelEvent( QWheelEvent* ev )
{
    sbSpin->setValue( sbSpin->value() + ( ev->delta() > 0 ? 1 : -1 ) );
}


void RotationEdit::stateChanged( int )
{
    Transformation rotation;

    rotation.rotateY( sbPan ->value(), Transformation::DEGREES )
            .rotateZ( sbTilt->value(), Transformation::DEGREES )
            .rotateX( sbSpin->value(), Transformation::DEGREES );

    view->setRotation( rotation );
    view->repaint();

    emit rotationChanged();
    emit rotationChanged( view->getRotation() );
}


int RotationEdit::getPan() const
{
    return sbPan->value();
}


int RotationEdit::getTilt() const
{
    return sbTilt->value();
}


int RotationEdit::getSpin() const
{
    return sbSpin->value();
}


void RotationEdit::setPan( int deg )
{
    sbPan->setValue( deg % 360 );
}


void RotationEdit::setTilt( int deg )
{
    sbTilt->setValue( ( deg + 90 ) % 180 - 90 );
}


void RotationEdit::setSpin( int deg )
{
    sbSpin->setValue( deg % 360 );
}



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna
