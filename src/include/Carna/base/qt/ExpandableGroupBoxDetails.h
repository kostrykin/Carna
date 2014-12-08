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

#ifndef EXPANDABLEGROUPBOX_DETAILS_H_6014714286
#define EXPANDABLEGROUPBOX_DETAILS_H_6014714286

/** \file   ExpandableGroupBoxDetails.h
  * \brief  Defines \ref Carna::base::qt::ExpandableGroupBox::Details.
  */

#include <Carna/base/qt/ExpandableGroupBox.h>
#include <Carna/base/noncopyable.h>
#include <QLabel>
#include <QVBoxLayout>
#include <QMargins>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ExpandableGroupBox :: Details
// ----------------------------------------------------------------------------------

/** \brief  PIMPL-implementation of \ref ExpandableGroupBox
  *
  * \author Leonid Kostrykin
  * \date   12.9.12
  */
class CARNA_EXPORT ExpandableGroupBox::Details
{

    NON_COPYABLE

public:

    Details();


    QLabel* const caption;

    QString text;

    bool expended;

    QVBoxLayout* const global;

    QWidget* child;


    void updateCaption();


private:

    QMargins expendedCaptionMargins, collapsedCaptionMargins;

}; // ExpandableGroupBox :: Details


ExpandableGroupBox::Details::Details()
    : caption( new QLabel() )
    , expended( false )
    , global( new QVBoxLayout() )
    , child( nullptr )
{
    expendedCaptionMargins = collapsedCaptionMargins = caption->contentsMargins();
    expendedCaptionMargins.setBottom( collapsedCaptionMargins.bottom() + 5 );
}


void ExpandableGroupBox::Details::updateCaption()
{
    const QString prefix = expended ? "&#9660;" : "&#9658;";
    caption->setText( prefix + " <a href=\"#\">" + text + "</a>" );
    caption->setContentsMargins( expended ? expendedCaptionMargins : collapsedCaptionMargins );
}



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // EXPANDABLEGROUPBOX_DETAILS_H_6014714286
