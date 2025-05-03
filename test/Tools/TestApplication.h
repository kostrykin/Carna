/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#pragma once

#include <LibCarna/LibCarna.h>
#include <QApplication>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// TestApplication
// ----------------------------------------------------------------------------------

class TestApplication : public QApplication
{

public:

    TestApplication( int &argc, char **argv );
    virtual ~TestApplication();
    
    virtual bool notify( QObject* receiver, QEvent* ev ) override;

}; // TestApplication



}  // namespace testing

}  // namespace Carna
