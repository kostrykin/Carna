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

#ifndef DISPLAY_DETAILS_H_6014714286
#define DISPLAY_DETAILS_H_6014714286

/** \file   DisplayDetails.h
  * \brief  Defines \ref Carna::base::qt::Display::Details.
  */

#include <Carna/base/qt/Display.h>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// Display :: Details
// ----------------------------------------------------------------------------------

/** \brief  PIMPL-implementation of \ref Display
  *
  * \author Leonid Kostrykin
  * \date   12.9.12 - 5.3.13
  */
class Display::Details : public QObject
{

    Q_OBJECT

    NON_COPYABLE

public:

    Details( Visualization* module, view::SceneProvider& provider, Display& self );

    ~Details();


    view::SceneProvider& provider;

    const std::unique_ptr< VisualizationEnvironment > enviroment;

    std::unique_ptr< Visualization > module;

    bool initialized;


    void initializeModule();

    void updateTargetSize( int w, int h );


private:

    Display& self;


public slots:

    void updateRendering();

}; // Display :: Details



}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna

#endif // DISPLAY_DETAILS_H_6014714286
