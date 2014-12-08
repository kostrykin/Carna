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

#include <Carna/base/Text.h>
#include <QFile>
#include <QTextStream>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Text
// ----------------------------------------------------------------------------------

std::string Text::cat( const std::string& fileName )
{
    QFile file( QString::fromStdString( fileName ) );
    
    file.open( QIODevice::ReadOnly | QIODevice::Text );
    return QTextStream( &file ).readAll().toStdString();
}



}  // namespace Carna :: base

}  // namespace Carna
