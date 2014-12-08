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

#include <Carna/VolumeRenderings/MIP/ChannelUiAnchor.h>
#include <Carna/VolumeRenderings/MIP/ChannelEditor.h>
#include <Carna/VolumeRenderings/MIP/Channel.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

namespace Carna
{

namespace VolumeRenderings
{

namespace MIP
{



// ----------------------------------------------------------------------------------
// MIP :: ChannelUiAnchor
// ----------------------------------------------------------------------------------

ChannelUiAnchor::ChannelUiAnchor( Channel& channel )    // TODO: is the name of this class still appropriate?
    : channel( channel )
    , editor( new ChannelEditor( channel ) )
{
    QHBoxLayout* master = new QHBoxLayout();
    QVBoxLayout* buttons = new QVBoxLayout();

    QPushButton* buAscend  = new QPushButton( QChar(  8593 ) );
    QPushButton* buDelete  = new QPushButton( QChar( 10005 ) );
    QPushButton* buDescend = new QPushButton( QChar(  8595 ) );

    buAscend ->setFixedSize( 20, 20 );
    buDelete ->setFixedSize( 20, 20 );
    buDescend->setFixedSize( 20, 20 );

    buttons->addWidget( buAscend );
    buttons->addWidget( buDelete );
    buttons->addWidget( buDescend );

    connect( buAscend , SIGNAL( clicked() ), this, SLOT(  ascendMe() ) );
    connect( buDelete , SIGNAL( clicked() ), this, SLOT(  deleteMe() ) );
    connect( buDescend, SIGNAL( clicked() ), this, SLOT( descendMe() ) );

    master->addLayout( buttons );
    master->addWidget( editor );
    
    master ->setContentsMargins( 0, 0, 0, 0 );
    buttons->setContentsMargins( 0, 0, 0, 0 );

    this->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    this->setLayout( master );
}


ChannelUiAnchor::~ChannelUiAnchor()
{
}


QSize ChannelUiAnchor::sizeHint() const
{
    const unsigned int w = editor->sizeHint().width() + 20;
    const unsigned int h = editor->sizeHint().height();
    return QSize( w, h );
}


void ChannelUiAnchor::ascendMe()
{
    emit toBeAscended( *this );
}


void ChannelUiAnchor::descendMe()
{
    emit toBeDescended( *this );
}


void ChannelUiAnchor::deleteMe()
{
    emit toBeDeleted( *this );
}



}  // namespace Carna :: VolumeRenderings :: MIP

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
