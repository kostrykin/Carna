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

#include <Carna/base/qt/SeriesPreview.h>
#include <Carna/base/model/Series.h>
#include <Carna/base/model/SeriesElement.h>
#include <Carna/base/CarnaException.h>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QVBoxLayout>
#include <dicom-interface/dicomImage.h>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// Types & Globals
// ----------------------------------------------------------------------------------

const static unsigned int PREVIEW_SIZE = 128;



// ----------------------------------------------------------------------------------
// SeriesPreview
// ----------------------------------------------------------------------------------

SeriesPreview::SeriesPreview( QWidget* parent )
    : QFrame( parent )
    , image( new QLabel() )
    , caption( new QLabel() )
{
    this->setLayout( new QVBoxLayout() );
    this->layout()->addWidget( image );
    this->layout()->addWidget( caption );
    this->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    this->setStyleSheet( "Carna--SeriesPreview{ border: 1px solid #FFFFFF; background-color: black; }" );

    image->setAlignment( Qt::AlignHCenter );
    caption->setAlignment( Qt::AlignHCenter );
    caption->setStyleSheet( "color: #FFFFFF; background: none;" );
}


SeriesPreview::~SeriesPreview()
{
}


void SeriesPreview::setSeries( const model::Series& series )
{
    this->series = &series;

    const DicomImage& dicomImage = series.getCentralElement().getDicomImage();
    const unsigned int dicomImageSize = std::max( dicomImage.getWidth(), dicomImage.getHeight() );
    const unsigned int previewWidth = ( dicomImage.getWidth() * PREVIEW_SIZE ) / dicomImageSize;
    const unsigned int previewHeight = ( dicomImage.getHeight() * PREVIEW_SIZE ) / dicomImageSize;

    std::unique_ptr< QImage > image( series.getCentralElement().createImage( previewWidth, previewHeight ) );

    QPixmap pixmap;
    pixmap.convertFromImage( *image );

    const static unsigned int max_caption_text_length = 20;
    const QString caption_text = series.name.length() < max_caption_text_length ? QString::fromStdString( series.name ) : QString::fromStdString( series.name.substr( 0, max_caption_text_length - 3 ) ) + "...";

    this->image->setPixmap( pixmap );
    this->caption->setText( QString::number( series.getElements().size() ) + " images\n" + caption_text );
    this->setToolTip( QString::fromStdString( series.name ) );
}


const model::Series& SeriesPreview::getSeries() const
{
    CARNA_ASSERT( series != nullptr );
    return *series;
}


bool SeriesPreview::hasSeries() const
{
    return series != nullptr;
}



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna
