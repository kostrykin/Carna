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

#include <Carna/base/model/SeriesElement.h>
#include <Carna/base/model/Series.h>
#include <Carna/base/Math.h>
#include <Carna/base/CarnaException.h>
#include <dicom-interface/dicomImage.h>
#include <QImage>
#include <QColor>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// SeriesElement
// ----------------------------------------------------------------------------------

SeriesElement::SeriesElement( const std::string& fileName, double zPosition )
    : fileName( fileName )
    , zPosition( zPosition )
    , series( nullptr )
{
}


SeriesElement::~SeriesElement()
{
}


const DicomImage& SeriesElement::getDicomImage() const
{
    CARNA_ASSERT( series != nullptr );

    if( dicomImage.get() == nullptr )
    {
        dicomImage.reset( new DicomImage( fileName ) );
        dicomImage->setPositionZ( this->zPosition );
        dicomImage->setSpacingZ( series->getSpacingZ() );
    }

    return *dicomImage;
}


QImage* SeriesElement::createImage( unsigned int max_width, unsigned int max_height ) const
{
    const DicomImage& dicomImage = getDicomImage();

    const unsigned int width = std::min( static_cast< unsigned >( dicomImage.getWidth() ), max_width );
    const unsigned int height = std::min( static_cast< unsigned >( dicomImage.getHeight() ), max_height );

    QImage* const image = new QImage( width, height, QImage::Format_Indexed8 );

    QVector< QRgb > colorTable( 256 );
    for( unsigned int i = 0; i < 256; ++i )
    {
        colorTable[ i ] = qRgb( i, i, i );
    }
    image->setColorCount( 256 );
    image->setColorTable( colorTable );

    double dx = static_cast< double >( dicomImage.getWidth() ) / width;
    double dy = static_cast< double >( dicomImage.getHeight() ) / height;

    double src_x = 0;
    for( unsigned int dst_x = 0; dst_x < width; ++dst_x, src_x += dx )
    {
        double src_y = 0;
        for( unsigned int dst_y = 0; dst_y < height; ++dst_y, src_y += dy )
        {
            const unsigned int src_x_int = std::min( static_cast< unsigned int >( src_x + 0.5 ), static_cast< unsigned int >( dicomImage.getWidth() - 1 ) );
            const unsigned int src_y_int = std::min( static_cast< unsigned int >( src_y + 0.5 ), static_cast< unsigned int >( dicomImage.getHeight() - 1 ) );

            const int huv = const_cast< DicomImage& >( dicomImage ).getPixel( src_x_int, src_y_int );
            const int i = base::Math::clamp( ( ( huv + 1024 ) * 256 ) / 4096, 0, 255 );
            
            image->setPixel( dst_x, dst_y, i );
        }
    }

    return image;
}


void SeriesElement::setSeries(  const Series& series )
{
    this->series = &series;
}



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna
