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

#ifndef POSITION_H_6014714286
#define POSITION_H_6014714286

/** \file   Position.h
  * \brief  Defines \ref Carna::base::model::Position
  */

#include <Carna/base/Transformation.h>
#include <Carna/base/model/Scene.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// Position
// ----------------------------------------------------------------------------------

/** \brief  Defines a position in \ref CoordinateSystems "model- or volume space".
  *
  * \attention
  * Priorly to 1.9, <i>volume space</i> and <i>volume units</i> were called <i>model
  * space</i> and <i>model units</i>, whereas <i>model unit</i> is now an equivalent
  * term for <i>millimeters</i> since 1.9.
  *
  * \author Leonid Kostrykin
  * \date   1.12.11 - 21.8.12
  */
class CARNA_EXPORT Position
{

public:

    /** \brief  Instantiates.
      */
    explicit Position( const Scene& );

    /** \brief  Copy constructor
      */
    Position( const Position& rval )
        : model( rval.model )
    {
        operator=( rval );
    }


    /** \brief  Returns the matrix of the \ref toVolumeUnits operation.
      *
      * The returned matrix is \f$M_0\f$ from:
      *
      * \f[ v_\text{modelspace} = M_0 \cdot v_\text{volumespace} \f]
      *
      * \see    \ref CoordinateSystems, \ref toVolumeUnitsTransformation
      */
    static Transformation toMillimetersTransformation( const Scene& );

    /** \brief  Returns the matrix of the \ref toMillimeters operation.
      *
      * The returned matrix is \f$M_0^{-1}\f$ from:
      *
      * \f[ v_\text{volumespace} = M_0^{-1} \cdot v_\text{modelspace} \f]
      *
      * \see    \ref CoordinateSystems, \ref toMillimetersTransformation
      */
    static Transformation toVolumeUnitsTransformation( const Scene& );


    /** \brief  Tells the coordinates in volume space.
      */
    const Vector& toVolumeUnits() const;

    /** \brief  Tells the coordinates in millimeters.
      */
    const Vector& toMillimeters() const;


    /** \brief  Sets the coordinates in volume space.
      */
    Position& setVolumeUnits( const Vector& );

    /** \brief  Sets the coordinates in millimeters.
      */
    Position& setMillimeters( const Vector& );


    /** \brief  References the data model.
      */
    const Scene& model;


    /** \brief  Instantiates from volume space coordinates.
      */
    static Position fromVolumeUnits( const Scene& m, const Vector& arg )
    {
        Position position( m );
        position.setVolumeUnits( arg );
        return position;
    }

    /** \brief  Instantiates from volume space coordinates.
      */
    static Position fromVolumeUnits( const Scene& m, double x, double y, double z )
    {
        return fromVolumeUnits( m, Vector( x, y, z ) );
    }

    /** \brief  Instantiates from millimeters.
      */
    static Position fromMillimeters( const Scene& m, const Vector& arg )
    {
        Position position( m );
        position.setMillimeters( arg );
        return position;
    }

    /** \brief  Instantiates from millimeters.
      */
    static Position fromMillimeters( const Scene& m, double x, double y, double z )
    {
        return fromMillimeters( m, Vector( x, y, z ) );
    }


    /** \brief  Shifts the position.
      */
    Position& operator+=( const Position& delta )
    {
        setVolumeUnits( toVolumeUnits() + delta.toVolumeUnits() );
        return *this;
    }

    /** \brief  Returns shifted position.
      */
    Position operator+( const Position& delta )
    {
        Position result( *this );
        result += delta;
        return result;
    }

    /** \brief  Sets another position.
      */
    Position& operator=( const Position& rval )
    {
        coordinates = rval.coordinates;
        return *this;
    }

    /** \brief  Returns whether both positions are equal.
      */
    bool operator==( const Position& ) const;


private:

    struct
    {
        /** \private
          */
        Vector volumeUnits;
        
        /** \private
          */
        Vector millimeters;
    }
    coordinates;


    void computeMillimeters();

    void computeVolumeUnits();

}; // Position



}  // namesapce Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // POSITION_H_6014714286
