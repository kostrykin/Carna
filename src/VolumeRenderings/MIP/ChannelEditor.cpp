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

#include <Carna/VolumeRenderings/MIP/ChannelEditor.h>
#include <Carna/VolumeRenderings/MIP/Channel.h>
#include <Carna/VolumeRenderings/MIP/ChannelFunction.h>
#include <Carna/VolumeRenderings/MIP/ChannelFunctionFactory.h>
#include <Carna/base/qt/ColorPicker.h>
#include <QSpinBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// MIP :: ChannelEditor
// ----------------------------------------------------------------------------------

ChannelEditor::ChannelEditor( Channel& channel )
    : channel( channel )
    , sbMin( new QSpinBox() )
    , sbMax( new QSpinBox() )
    , sbOpacity( new QSpinBox() )
    , cbSyncColors( new QCheckBox( "Sync" ) )
    , color_picker_min( new base::qt::ColorPicker( channel.getMinColor() ) )
    , color_picker_max( new base::qt::ColorPicker( channel.getMaxColor() ) )
    , syncColors ( true )
{
    this->setFrameStyle( QFrame::Panel | QFrame::Raised );
    this->setLineWidth( 1 );
    this->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

    QVBoxLayout* master = new QVBoxLayout();

    QHBoxLayout* general = new QHBoxLayout();

    QComboBox* function_selector = new QComboBox();

    for( std::vector< ChannelFunctionFactory* >::const_iterator
                it  = ChannelFunction::factories.begin();
                it != ChannelFunction::factories.end();
              ++it )
    {
        function_selector->addItem( QString::fromStdString( ( **it ).getName() ) );
        if( channel.getFunction().isInstanceOf( **it ) )
        {
            function_selector->setCurrentIndex( it - ChannelFunction::factories.begin() );
        }
    }

    general->addWidget( function_selector );

    connect( function_selector, SIGNAL( activated( int ) ), this, SLOT( functionSelected( int ) ) );

    master->addLayout( general );

    QGridLayout* bottom = new QGridLayout();

    sbMin->setMinimum( -1024 );
    sbMin->setMaximum(  3071 );
    sbMin->setValue( channel.getHuvRange().first );

    connect( sbMin, SIGNAL( valueChanged( int ) ), &channel, SLOT( setMinHuv( int ) ) );

    sbMax->setMinimum( -1024 );
    sbMax->setMaximum(  3071 );
    sbMax->setValue( channel.getHuvRange().last );

    connect( sbMax, SIGNAL( valueChanged( int ) ), &channel, SLOT( setMaxHuv( int ) ) );

    sbOpacity->setMinimum( 0 );
    sbOpacity->setMaximum( 100 );
    sbOpacity->setValue( unsigned int( channel.getOpacity() * 100 + 0.5f ) );

    connect( sbOpacity, SIGNAL( valueChanged( int ) ), this, SLOT( opacityChanged( int ) ) );

    QLabel* const laOpacity = new QLabel( "Opacity:" );
    QLabel* const laMin     = new QLabel( "Min:" );
    QLabel* const laMax     = new QLabel( "Max:" );

    bottom->addWidget( laMin    , 0, 0 );
    bottom->addWidget( sbMin    , 1, 0 );
    bottom->addWidget( laMax    , 0, 1 );
    bottom->addWidget( sbMax    , 1, 1 );
    bottom->addWidget( laOpacity, 0, 2 );
    bottom->addWidget( sbOpacity, 1, 2 );

    connect( color_picker_min, SIGNAL( colorChanged( const QColor& ) ), this, SLOT( setMinColor( const QColor& ) ) );
    connect( color_picker_max, SIGNAL( colorChanged( const QColor& ) ), this, SLOT( setMaxColor( const QColor& ) ) );

    bottom->addWidget( color_picker_min, 2, 0 );
    bottom->addWidget( color_picker_max, 2, 1 );
    bottom->addWidget( cbSyncColors    , 2, 2 );

    cbSyncColors->setCheckState( Qt::Checked );
    cbSyncColors->setToolTip( "Uses same color for minimum and maximum, keeping their individual alpha setting." );
    connect( cbSyncColors, SIGNAL( stateChanged( int ) ), this, SLOT( setColorSync( int ) ) );

    master->addLayout( bottom );

    this->setLayout( master );

    const QString tiny_style_sheet = "font-size: 10px;";
    sbMin->setStyleSheet( tiny_style_sheet );
    sbMax->setStyleSheet( tiny_style_sheet );
    sbOpacity->setStyleSheet( tiny_style_sheet );
    laOpacity->setStyleSheet( tiny_style_sheet );
    cbSyncColors->setStyleSheet( tiny_style_sheet );

    master ->setContentsMargins( 0, 0, 0, 0 );
    general->setContentsMargins( 5, 5, 5, 5 );
    bottom ->setContentsMargins( 5, 5, 5, 5 );
}


bool ChannelEditor::equalsRGB( const QColor& c1, const QColor& c2 )
{
    return c1.red() == c2.red() && c1.green() == c2.green() && c1.blue() == c2.blue();
}


void ChannelEditor::setMinColor( const QColor& color )
{
    channel.setMinColor( color );
    if( syncColors && !equalsRGB( color, color_picker_max->getColor() ) )
    {
        QColor c = color;
        c.setAlpha( color_picker_max->getColor().alpha() );
        color_picker_max->setColor( c );
    }
}


void ChannelEditor::setMaxColor( const QColor& color )
{
    channel.setMaxColor( color );
    if( syncColors && !equalsRGB( color, color_picker_min->getColor() ) )
    {
        QColor c = color;
        c.setAlpha( color_picker_min->getColor().alpha() );
        color_picker_min->setColor( c );
    }
}


void ChannelEditor::setColorSync( int check_state )
{
    syncColors = ( check_state == Qt::Checked );
}


void ChannelEditor::functionSelected( int index )
{
    channel.setFunction( ChannelFunction::factories[ index ]->create() );
}


void ChannelEditor::opacityChanged( int opacity )
{
    channel.setOpacity( opacity / 100.f );
}


QSize ChannelEditor::sizeHint() const
{
    const unsigned int w = sbMin->sizeHint().width() + sbMax->sizeHint().width() + sbOpacity->sizeHint().width();
    const unsigned int h = unsigned int( sbMin->sizeHint().height() * 4.5f );
    return QSize( w, h );
}



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
