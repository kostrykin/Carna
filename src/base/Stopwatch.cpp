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

#include <Carna/base/Stopwatch.h>
#include <climits>
#include <omp.h>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Stopwatch
// ----------------------------------------------------------------------------------

Stopwatch::Stopwatch()
{
    restart();
}


void Stopwatch::restart()
{
    time0 = omp_get_wtime();
}


double Stopwatch::result()
{
    const double time1 = omp_get_wtime();
    const double t = time1 - time0;
    return t;
}



}  // namespace Carna :: base

}  // namespace Carna
