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

#include <Carna/VolumeRenderings/MIP/MaximumIntensityProjectionControllerUI.h>
#include <Carna/VolumeRenderings/MIP/MaximumIntensityProjection.h>
#include <Carna/VolumeRenderings/MIP/Channel.h>
#include <Carna/VolumeRenderings/MIP/ChannelUiAnchor.h>
#include <Carna/base/CarnaException.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDomDocument>
#include <QDomElement>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// MIP :: MaximumIntensityProjectionControllerUI
// ----------------------------------------------------------------------------------

MaximumIntensityProjectionControllerUI::MaximumIntensityProjectionControllerUI
    ( MaximumIntensityProjection& mode
    , QWidget* parent )

    : QWidget( parent )
    , mode( mode )
    , channels( new QVBoxLayout() )
{
    QVBoxLayout* layout = new QVBoxLayout();

    QPushButton* buAddChannel = new QPushButton( "&Add Channel" );
    connect( buAddChannel, SIGNAL( clicked() ), this, SLOT( addChannel() ) );
    layout->addWidget( buAddChannel );

    channels->addStretch();
    layout->addLayout( channels );
    
    layout->setContentsMargins( 0, 0, 0, 0 );
    channels->setContentsMargins( 0, 0, 0, 0 );

    QHBoxLayout* const bottom_buttons = new QHBoxLayout();
    QPushButton* const buSaveColorConfig = new QPushButton( "Save" );
    QPushButton* const buLoadColorConfig = new QPushButton( "Load" );
    bottom_buttons->addWidget( buSaveColorConfig );
    bottom_buttons->addWidget( buLoadColorConfig );
    bottom_buttons->addStretch( 1 );

    QVBoxLayout* master = new QVBoxLayout();
    master->addLayout( layout );
    master->addLayout( bottom_buttons );

    QObject::connect( buSaveColorConfig, SIGNAL( clicked() ), this, SLOT( saveColorConfig() ) );
    QObject::connect( buLoadColorConfig, SIGNAL( clicked() ), this, SLOT( loadColorConfig() ) );

    this->setLayout( master );

    // ----------------------------------------------------------------------------------

    MaximumIntensityProjection::ChannelVisitor addChannelAnchor =
        [&]( Channel& channel )->bool
    {
        this->addChannelAnchor( channel );
        return true;
    };

    mode.visitChannels( addChannelAnchor );
}


void MaximumIntensityProjectionControllerUI::addChannel()
{
    addChannel( -1024, 3071 );
}


void MaximumIntensityProjectionControllerUI::addChannel
    ( int min
    , int max
    , float opacity
    , ChannelFunction* const function )
{
    CARNA_ASSERT( function != nullptr );

    QColor minColor( nextColor );
    minColor.setAlpha( 0 );
    addChannel( min, max, minColor, nextColor++, opacity, function );
}


void MaximumIntensityProjectionControllerUI::addChannelAnchor( Channel& channel )
{
    ChannelUiAnchor* const anchor = new ChannelUiAnchor( channel );

    connect
        ( anchor
        , SIGNAL( toBeDeleted( Carna::VolumeRenderings::MIP::ChannelUiAnchor& ) )
        , this
        , SLOT( deleteChannel( Carna::VolumeRenderings::MIP::ChannelUiAnchor& ) ) );

    connect
        ( anchor
        , SIGNAL( toBeAscended( Carna::VolumeRenderings::MIP::ChannelUiAnchor& ) )
        , this
        , SLOT( ascendChannel( Carna::VolumeRenderings::MIP::ChannelUiAnchor& ) ) );

    connect
        ( anchor
        , SIGNAL( toBeDescended( Carna::VolumeRenderings::MIP::ChannelUiAnchor& ) )
        , this
        , SLOT( descendChannel( Carna::VolumeRenderings::MIP::ChannelUiAnchor& ) ) );

    channels->insertWidget( 0, anchor );
}


void MaximumIntensityProjectionControllerUI::addChannel
    ( int min, int max
    , const QColor& c0, const QColor& c1
    , float opacity
    , ChannelFunction* const function )
{
    CARNA_ASSERT( function != nullptr );

    Channel* const channel = new Channel
        ( base::IntSpan( min, max )
        , c0
        , c1
        , opacity
        , function );

    addChannelAnchor( *channel );

    mode.addChannel( channel );
}


void MaximumIntensityProjectionControllerUI::removeAllChannels()
{
    CARNA_ASSERT( channels->count() >= 0 );

    for( unsigned int i = 0; i < unsigned( channels->count() ); )
    {
        QLayoutItem* item = channels->itemAt( i );
        QWidget* widget = item->widget();

        ChannelUiAnchor* const ui_anchor = dynamic_cast< ChannelUiAnchor* >( widget );
        if( ui_anchor )
        {
            deleteChannel( *ui_anchor );
        }
        else
        {
            ++i;
        }
    }
}


void MaximumIntensityProjectionControllerUI::deleteChannel( ChannelUiAnchor& anchor )
{
    mode.removeChannel( anchor.channel );

    anchor.hide();
    channels->removeWidget( &anchor );
}


void MaximumIntensityProjectionControllerUI::ascendChannel( ChannelUiAnchor& anchor )
{
    CARNA_ASSERT( channels->count() >= 0 );

    for( unsigned int i = 0; i < unsigned( channels->count() ); ++i )
    {
        QLayoutItem* item = channels->itemAt( i );
        QWidget* widget = item->widget();

        if( widget == &anchor )
        {
            if( i >= 1 )
            {
                channels->removeWidget( widget );
                channels->insertWidget( i - 1, widget );
                
                mode.ascendChannel( anchor.channel );
            }
            break;
        }
    }
}


void MaximumIntensityProjectionControllerUI::descendChannel( ChannelUiAnchor& ch )
{
    CARNA_ASSERT( channels->count() >= 0 );

    for( unsigned int i = 0; i < unsigned( channels->count() ); ++i )
    {
        QLayoutItem* item = channels->itemAt( i );
        QWidget* widget = item->widget();

        if( widget == &ch )
        {
            if( i + 1 < unsigned( channels->count() ) )
            {
                QLayoutItem* next_item = channels->itemAt( i + 1 );
                QWidget* next_widget = next_item->widget();
                if( next_widget )
                {
                    channels->removeWidget( next_widget );
                    channels->insertWidget( i, next_widget );

                    ChannelUiAnchor& anchor = static_cast< ChannelUiAnchor& >( *next_widget );

                    mode.ascendChannel( anchor.channel );
                }
            }
            break;
        }
    }
}


void MaximumIntensityProjectionControllerUI::saveColorConfig()
{
    const QString filename = QFileDialog::getSaveFileName
        ( this
        , "Save Color Map"
        , ""
        , "XML-Files (*.XML)"
        , 0
        , QFileDialog::DontResolveSymlinks
        | QFileDialog::HideNameFilterDetails );

    if( filename.isEmpty() )
    {
        return;
    }

    QDomDocument dom;
    dom.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"utf-8\"" );

    QDomElement root = dom.createElement( "MaximumIntensityColorTable" );
    dom.appendChild( root );

 // see: http://www.digitalfanatics.org/projects/qt_tutorial/chapter09.html

    MaximumIntensityProjection::ConstChannelVisitor writeChannelXml
        = [&]( const Channel& channel )->bool
    {
        QDomElement node = dom.createElement( "Channel" );

        node.setAttribute( "minHuv", channel.getHuvRange().first );
        node.setAttribute( "maxHuv", channel.getHuvRange().last );

        node.setAttribute( "minColorR", channel.getMinColor().red  () );
        node.setAttribute( "minColorG", channel.getMinColor().green() );
        node.setAttribute( "minColorB", channel.getMinColor().blue () );
        node.setAttribute( "minColorA", channel.getMinColor().alpha() );

        node.setAttribute( "maxColorR", channel.getMaxColor().red  () );
        node.setAttribute( "maxColorG", channel.getMaxColor().green() );
        node.setAttribute( "maxColorB", channel.getMaxColor().blue () );
        node.setAttribute( "maxColorA", channel.getMaxColor().alpha() );

        node.setAttribute( "opacity", channel.getOpacity() );
        node.setAttribute( "function", QString::fromStdString( channel.getFunction().getName() ) );

        root.appendChild( node );

        return true;
    };

    mode.visitChannels( writeChannelXml );

    QFile file( filename );
    if( !file.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
    {
        throw base::CarnaException
            ( "Save Color Map"
            , "Failed opening file for writing."
            , ( "Filename: '" + filename + "'" ).toStdString() );
    }

    QTextStream stream( &file );
    stream << dom.toString();

    file.close();
}


void MaximumIntensityProjectionControllerUI::loadColorConfig()
{
    const QString filename = QFileDialog::getOpenFileName
        ( this
        , "Load Color Map"
        , ""
        , "XML-Files (*.xml)"
        , 0
        , QFileDialog::ReadOnly
        | QFileDialog::HideNameFilterDetails );

    if( filename.isEmpty() )
    {
        return;
    }

    QFile file( filename );
    if( !file.open( QIODevice::ReadOnly ) )
    {
        throw base::CarnaException
            ( "Load Color Map"
            , "Failed opening file for reading."
            , ( "Filename: '" + filename + "'" ).toStdString() );
    }

    this->removeAllChannels();

    struct unexpected_format_exception { };
    try
    {

        QDomDocument dom;
        if( !dom.setContent( &file ) )
        {
            throw unexpected_format_exception();
        }

        QDomElement root = dom.documentElement();
        if( root.tagName() != "MaximumIntensityColorTable" )
        {
            throw unexpected_format_exception();
        }

        QDomNode n = root.firstChild();
        while( !n.isNull() )
        {
            QDomElement e = n.toElement();
            if( !e.isNull() )
            {
                if( e.tagName() != "Channel" )
                {
                    throw unexpected_format_exception();
                }

                const int huv0 = e.attribute( "minHuv" ).toInt();
                const int huv1 = e.attribute( "maxHuv" ).toInt();

                const int r0 = e.attribute( "minColorR" ).toInt();
                const int g0 = e.attribute( "minColorG" ).toInt();
                const int b0 = e.attribute( "minColorB" ).toInt();
                const int a0 = e.attribute( "minColorA" ).toInt();

                const int r1 = e.attribute( "maxColorR" ).toInt();
                const int g1 = e.attribute( "maxColorG" ).toInt();
                const int b1 = e.attribute( "maxColorB" ).toInt();
                const int a1 = e.attribute( "maxColorA" ).toInt();

                const float opacity = e.attribute( "opacity" ).toFloat();
                const std::string functionName = e.attribute( "function" ).toStdString();

                const QColor color0 = QColor( r0, g0, b0, a0 );
                const QColor color1 = QColor( r1, g1, b1, a1 );

                ChannelFunction* const function
                    = ChannelFunction::getFactory( functionName ).create();

                this->addChannel( huv0, huv1, color0, color1, opacity, function );
            }

            n = n.nextSibling();
        }
    }
    catch( const unexpected_format_exception& )
    {
        throw base::CarnaException
            ( "Load Color Map"
            , "Unexpected file format."
            , ( "Filename: '" + filename + "'" ).toStdString() );
    }
}



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
