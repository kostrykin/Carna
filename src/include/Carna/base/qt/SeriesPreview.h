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

#ifndef SERIESPREVIEW_H_6014714286
#define SERIESPREVIEW_H_6014714286

/** \file   SeriesPreview.h
  * \brief  Defines \ref Carna::base::qt::SeriesPreview.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <QFrame>

class QLabel;
class QImage;

namespace Carna
{

namespace base
{

namespace model
{
    class Series;
}

namespace qt
{



// ----------------------------------------------------------------------------------
// SeriesPreview
// ----------------------------------------------------------------------------------

class SeriesPreview : public QFrame
{

    NON_COPYABLE

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    explicit SeriesPreview( QWidget* parent = nullptr );

    /** \brief  Releases acquired resources.
      */
    virtual ~SeriesPreview();


    void setSeries( const model::Series& );

    const model::Series& getSeries() const;

    bool hasSeries() const;


private:

    const model::Series* series;

    QLabel* const image;
    QLabel* const caption;

}; // SeriesPreview



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // SERIESPREVIEW_H_6014714286
