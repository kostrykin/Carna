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

#include <Carna/base/ParallelizationSettings.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ParallelizationSettings
// ----------------------------------------------------------------------------------

const unsigned int ParallelizationSettings::defaultThreadCount = QThread::idealThreadCount();

const QThread::Priority ParallelizationSettings::defaultPriority = QThread::InheritPriority;


ParallelizationSettings::ParallelizationSettings( unsigned int threads
                                                , QThread::Priority priority )
    : threads( threads )
    , priority( priority )
{
}



}  // namespace Carna :: base

}  // namespace Carna
