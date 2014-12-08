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

#ifndef TOGGLESERIESPREVIEW_H_6014714286
#define TOGGLESERIESPREVIEW_H_6014714286

/** \file   ToggleSeriesPreview.h
  * \brief  Defines \ref Carna::base::qt::ToggleSeriesPreview.
  */

#include <Carna/base/qt/SeriesPreview.h>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ToggleSeriesPreview
// ----------------------------------------------------------------------------------

class ToggleSeriesPreview : public SeriesPreview
{

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    explicit ToggleSeriesPreview( QWidget* parent = nullptr );

    /** \brief  Releases acquired resources.
      */
    virtual ~ToggleSeriesPreview();

    
    bool isToggled() const;


public slots:

    void setToggled( bool );

    void toggle();


signals:

    void toggled();

    void toggled( bool );

    void toggled( Carna::base::qt::ToggleSeriesPreview& );

    void doubleClicked();

    void doubleClicked( Carna::base::qt::ToggleSeriesPreview& );


protected:

    virtual void mousePressEvent( QMouseEvent* ) override;

    virtual void mouseDoubleClickEvent( QMouseEvent* ) override;


private:

    bool amToggled;

}; // ToggleSeriesPreview



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // TOGGLESERIESPREVIEW_H_6014714286
