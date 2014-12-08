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

#include <Carna/base/model/Position.h>
#include <Carna/base/model/Volume.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Math.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// Position
// ----------------------------------------------------------------------------------

Position::Position( const Scene& model )
    : model( model )
{
    const static Vector origin( 0, 0, 0 );

    coordinates.volumeUnits = origin;
    coordinates.millimeters  = origin;
}


const Vector& Position::toVolumeUnits() const
{
    return coordinates.volumeUnits;
}


const Vector& Position::toMillimeters() const
{
    return coordinates.millimeters;
}


Position& Position::setVolumeUnits( const Vector& arg )
{
    CARNA_ASSERT( arg.size() == 3 );

    coordinates.volumeUnits = arg;

    computeMillimeters();

    return *this;
}


Position& Position::setMillimeters( const Vector& arg )
{
    CARNA_ASSERT( arg.size() == 3 );

    coordinates.millimeters = arg;

    computeVolumeUnits();

    return *this;
}


void Position::computeMillimeters()
{
    coordinates.millimeters = coordinates.volumeUnits
                            * Vector
                                 ( model.spacingX()
                                 , model.spacingY()
                                 , model.spacingZ() )
                            * Vector
                                 ( model.volume().size.x - 1
                                 , model.volume().size.y - 1
                                 , model.volume().size.z - 1 );
}


void Position::computeVolumeUnits()
{
    coordinates.volumeUnits = coordinates.millimeters /
                            ( Vector
                                 ( model.spacingX()
                                 , model.spacingY()
                                 , model.spacingZ() )
                            * Vector
                                 ( model.volume().size.x - 1
                                 , model.volume().size.y - 1
                                 , model.volume().size.z - 1 ) );
}


Transformation Position::toMillimetersTransformation( const Scene& model )
{
    Transformation result;

    result.scale( ( model.volume().size.x - 1 ) * model.spacingX()
                , ( model.volume().size.y - 1 ) * model.spacingY()
                , ( model.volume().size.z - 1 ) * model.spacingZ() );

    return result;
}


Transformation Position::toVolumeUnitsTransformation( const Scene& model )
{
    Transformation result;

    result.scale( 1. / ( ( model.volume().size.x - 1 ) * model.spacingX() )
                , 1. / ( ( model.volume().size.y - 1 ) * model.spacingY() )
                , 1. / ( ( model.volume().size.z - 1 ) * model.spacingZ() ) );

    return result;
}


bool Position::operator==( const Position& other ) const
{
    return &model == &other.model && Math::isEqual( toVolumeUnits(), other.toVolumeUnits() );
}



}  // namespace Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna
