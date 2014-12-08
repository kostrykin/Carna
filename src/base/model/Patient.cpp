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

#include <Carna/base/model/Patient.h>
#include <Carna/base/model/Study.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// Patient
// ----------------------------------------------------------------------------------

Patient::Patient( const std::string& name )
    : name( name )
{
}


Patient::~Patient()
{
    std::for_each( studies.begin(), studies.end(), std::default_delete< Study >() );
}


const std::deque< Study* >& Patient::getStudies() const
{
    return studies;
}


void Patient::put( Study* study )
{
    studies.push_back( study );
}



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna
