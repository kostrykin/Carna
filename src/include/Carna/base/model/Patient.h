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

#ifndef PATIENT_H_6014714286
#define PATIENT_H_6014714286

/** \file   Patient.h
  * \brief  Defines \ref Carna::base::model::Patient.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <deque>

namespace Carna
{

namespace base
{

namespace model
{

class Study;



// ----------------------------------------------------------------------------------
// Patient
// ----------------------------------------------------------------------------------

/** \brief  Represents an unordered set of studies.
  *
  * \author Leonid Kostrykin
  * \date   1.10.12
  */
class Patient
{

    NON_COPYABLE

public:

    /** \brief  Instantiates.
      */
    explicit Patient( const std::string& name );

    /** \brief  Releases acquired resources.
      */
    ~Patient();


    const std::string name;


    const std::deque< Study* >& getStudies() const;

    void put( Study* );


private:

    std::deque< Study* > studies;

}; // Patient


inline Patient& operator<<( Patient& patient, Study* study )
{
    patient.put( study );
    return patient;
}



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // PATIENT_H_6014714286
