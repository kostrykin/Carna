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

#ifndef PARALLELIZATIONSETTINGS_H_6014714286
#define PARALLELIZATIONSETTINGS_H_6014714286

/** \file ParallelizationSettings.h
  * \brief Defines \ref Carna::base::ParallelizationSettings.
  */

#include <Carna/Carna.h>
#include <QThread>

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// ParallelizationSettings
// ----------------------------------------------------------------------------------

/** \brief  Holds \ref Parallelization "parallelization" settings.
  *
  * \ingroup    ParallelizationTools
  *
  * \author Leonid Kostrykin
  * \date   2011
  */
class CARNA_EXPORT ParallelizationSettings
{

public:

    /** \brief  Instantiates.
      */
    explicit ParallelizationSettings
        ( unsigned int threads = defaultThreadCount
        , QThread::Priority priority = defaultPriority );


    /** \brief  Holds number of threads to use.
      */
    unsigned int threads;

    /** \brief  Holds the thread priority.
      */
    QThread::Priority priority;


    /** \brief  Holds the default number of threads to use.
      */
    const static unsigned int defaultThreadCount;

    /** \brief  Holds the default thread priority.
      */
    const static QThread::Priority defaultPriority;

}; // ParallelizationSettings



}  // namespace Carna :: base

}  // namespace Carna

#endif PARALLELIZATIONSETTINGS_H_6014714286
