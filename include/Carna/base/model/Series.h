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

#ifndef SERIES_H_6014714286
#define SERIES_H_6014714286

/** \file   Series.h
  * \brief  Defines \ref Carna::base::model::Series.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <set>

namespace Carna
{

namespace base
{

namespace model
{

class SeriesElement;



// ----------------------------------------------------------------------------------
// Series
// ----------------------------------------------------------------------------------

/** \brief  Represents a z-ordered set of DICOM files.
  *
  * \see    \ref SeriesElement, \ref SeriesLoadingRequest
  *
  * \author Leonid Kostrykin
  * \date   1.10.12 - 8.5.13
  */
class CARNA_EXPORT Series
{

    NON_COPYABLE

public:

    /** \brief  Defines \ref SeriesElement "series elements" ordering.
      */
    struct IsCloser
    {
        /** \brief  Tells whether \c zPosition of \a e1 is less than \c zPosition of \a e2.
          *
          * If the \c zPosition attribute of both elements is equal, the value returned tells
          * whether \c fileName of \a e1 is ordered lower than the \c fileName of \a e2.
          */
        bool operator()( SeriesElement* e1, SeriesElement* e2 ) const;
    };

    /** \brief  Defines set of z-ordered \ref SeriesElement "series elements".
      */
    typedef std::set< SeriesElement*, IsCloser > OrderedElements;


    /** \brief  Minimal \ref SeriesElement "elements" number any series must have in order to work properly.
      */
    const static unsigned int MIN_ELEMENTS_COUNT = 2;


    /** \brief  Instantiates.
      */
    explicit Series( const std::string& name );

    /** \brief  Releases acquired resources.
      */
    ~Series();


    /** \brief  Holds the name of this series.
      */
    const std::string name;


    /** \brief  References z-ordered set of \ref SeriesElement "series elements".
      */
    const OrderedElements& getElements() const;

    /** \brief  References the \ref SeriesElement "series element" which is most closest to the volume's center.
      *
      * \pre    <code>getElements().size() >= MIN_ELEMENTS_COUNT</code>
      */
    const SeriesElement& getCentralElement() const;

    /** \brief  Makes the given \ref SeriesElement "series elements" become part of this series.
      */
    void put( SeriesElement* );

    /** \brief  Tells the z-spacing between two neighbored \ref SeriesElement "series elements".
      *
      * \pre    <code>getElements().size() >= MIN_ELEMENTS_COUNT</code>
      */
    double getSpacingZ() const;

    /** \brief  Tells whether this series already has an \ref SeriesElement "elements" whose DICOM image file is located at the given path.
      */
    bool contains( const std::string& fileName ) const;


private: // TODO: implement PIMPL

    OrderedElements elements;

    mutable double zSpacing;

}; // Series



}  // namesapce Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // SERIES_H_6014714286
