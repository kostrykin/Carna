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

#include <Carna/VolumeRenderings/DVR/ColorMapSpanPainter.h>
#include <Carna/VolumeRenderings/DVR/ColorMapEditor.h>
#include <Carna/base/qt/MultiSpanSliderTracker.h>
#include <QPainter>
#include <QLinearGradient>

namespace Carna
{

namespace VolumeRenderings
{

namespace DVR
{



// ----------------------------------------------------------------------------------
// DVR :: ColorMapSpanPainter
// ----------------------------------------------------------------------------------

void ColorMapSpanPainter::paint( QPainter& painter, const base::IntSpan&, int span_index )
{
    const QColor& color0 = widget->getFirstColor( span_index );
    const QColor& color1 = widget->getLastColor( span_index );

    QLinearGradient gradient( 0, 0, 0, 1 );
    gradient.setColorAt( 0, color0 );
    gradient.setColorAt( 1, color1 );

    const QRect rect( base::qt::MultiSpanSliderTracker::trackerWidth / 3, 0, base::qt::MultiSpanSliderTracker::trackerWidth / 2, 1 );

    painter.setPen( Qt::NoPen );
    painter.setBrush( QBrush( gradient ) );
    painter.drawRect( rect );
}



}  // namespace Carna :: VolumeRenderings :: DVR

}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
