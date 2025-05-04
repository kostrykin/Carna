/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include <LibCarna/base/Stopwatch.h>
#include <climits>
#include <omp.h>

namespace LibCarna
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


double Stopwatch::result() const
{
    const double time1 = omp_get_wtime();
    const double t = time1 - time0;
    return t;
}



}  // namespace LibCarna :: base

}  // namespace LibCarna
