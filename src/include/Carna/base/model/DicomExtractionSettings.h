/*
 *  Copyright (C) 2010 - 2013 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#ifndef DICOMEXTRACTIONSETTINGS_H_6014714286
#define DICOMEXTRACTIONSETTINGS_H_6014714286

/** \file   DicomExtractionSettings.h
  * \brief  Defines \ref Carna::DicomExtractionSettings
  */

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// DicomExtractionSettings
// ----------------------------------------------------------------------------------

class DicomExtractionSettings
{

public:

    DicomExtractionSettings()
    {
    }

    DicomExtractionSettings( const QString& destination
                           , const std::set< const Series* >& series )
        : destination( destination )
        , series( series )
    {
    }


    QString destination;

    std::set< const Series* > series;

}; // DicomExtractionSettings



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // DICOMEXTRACTIONSETTINGS_H_6014714286
