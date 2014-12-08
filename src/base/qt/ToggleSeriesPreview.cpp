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

#include <Carna/base/qt/ToggleSeriesPreview.h>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// Types & Globals
// ----------------------------------------------------------------------------------

const static QString NOT_TOGGLED_CSS = "Carna--ToggleSeriesPreview{ border: 1px solid #AAAAAA; background-color: #000000; }";
const static QString TOGGLED_CSS     = "Carna--ToggleSeriesPreview{ border: 1px solid #FFFFFF; background-color: #404040; }";



// ----------------------------------------------------------------------------------
// ToggleSeriesPreview
// ----------------------------------------------------------------------------------

ToggleSeriesPreview::ToggleSeriesPreview( QWidget* parent )
    : SeriesPreview( parent )
    , amToggled( false )
{
    this->setStyleSheet( NOT_TOGGLED_CSS );
}


ToggleSeriesPreview::~ToggleSeriesPreview()
{
}


void ToggleSeriesPreview::setToggled( bool amToggled )
{
    if( this->amToggled != amToggled )
    {
        this->amToggled = amToggled;

        this->setStyleSheet( amToggled ? TOGGLED_CSS : NOT_TOGGLED_CSS );

        emit toggled( *this );
        emit toggled( amToggled );
        emit toggled();
    }
}


void ToggleSeriesPreview::toggle()
{
    setToggled( !isToggled() );
}


bool ToggleSeriesPreview::isToggled() const
{
    return amToggled;
}


void ToggleSeriesPreview::mousePressEvent( QMouseEvent* ev )
{
    this->toggle();
}


void ToggleSeriesPreview::mouseDoubleClickEvent( QMouseEvent* ev )
{
    emit doubleClicked( *this );
    emit doubleClicked();
}



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna
