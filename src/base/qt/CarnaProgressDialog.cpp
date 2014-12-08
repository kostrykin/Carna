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

#include <Carna/base/qt/CarnaProgressDialog.h>

namespace Carna
{

namespace base
{

namespace qt
{



// ----------------------------------------------------------------------------------
// CarnaProgressDialog
// ----------------------------------------------------------------------------------
    
CarnaProgressDialog::CarnaProgressDialog( const QString& labelText
                                        , const QString& cancelButtonText
                                        , int minimum
                                        , int maximum
                                        , QWidget* parent )
    : QProgressDialog( labelText
                     , cancelButtonText
                     , minimum
                     , maximum
                     , parent
                     , Qt::CustomizeWindowHint | Qt::WindowTitleHint )
                     , recursionLevel( 0 )
{
}


}  // namespace Carna :: base :: qt

}  // namespace Carna :: base

}  // namespace Carna
