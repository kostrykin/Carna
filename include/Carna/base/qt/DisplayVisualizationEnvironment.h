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

#ifndef DISPLAYVISUALIZATIONENVIRONMENT_H_6014714286
#define DISPLAYVISUALIZATIONENVIRONMENT_H_6014714286

/** \file   DisplayVisualizationEnvironment.h
  * \brief  Defines \ref Carna::base::qt::DisplayVisualizationEnvironment.
  */

#include <Carna/base/VisualizationEnvironment.h>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// DisplayVisualizationEnvironment
// ----------------------------------------------------------------------------------

/** \brief
  * \ref Display adapter for the \ref VisualizationEnvironment interface.
  *
  * \author Leonid Kostrykin
  * \date   5.3.13 - 18.3.13
  */
class CARNA_EXPORT DisplayVisualizationEnvironment : public VisualizationEnvironment
{

public:

    /** \brief  Instantiates.
      */
    explicit DisplayVisualizationEnvironment( Display& );

    /** \brief  References the display, this environment represents.
      */
    Display& display;

    // ------------------------------------------------------------------------------
    // Core Interface
    // ------------------------------------------------------------------------------

    virtual MenuBuilder* createMenu() override;

    virtual void setViewCursor( Qt::CursorShape ) override;

    virtual QString promptText
        ( const QString& title
        , const QString& text
        , QLineEdit::EchoMode echoMode
        , const QString& value
        , bool& ok ) override;

    virtual void activateContext() const override;

    virtual void paintText( const Vector& position, const std::string& text ) override;

}; // DisplayVisualizationEnvironment



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // DISPLAYVISUALIZATIONENVIRONMENT_H_6014714286
