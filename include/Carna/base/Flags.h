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

#ifndef FLAGS_H_6014714286
#define FLAGS_H_6014714286

/** \file   Flags.h
  * \brief  Defines Carna::base::Flags.
  */

namespace Carna
{

namespace base
{



// ----------------------------------------------------------------------------------
// Flags
// ----------------------------------------------------------------------------------

/** \brief  Allows type-safe bit operations on enums.
  *
  * Example:
  *
  * \code
  * using Carna::base::Flags;
  *
  * enum Capabilities
  * {
  *     cap1 = ( 1 << 0 ),
  *     cap2 = ( 1 << 1 ),
  *     cap3 = ( 1 << 2 )
  * }
  *
  * Flags< Capabilities >& mode1 = Flags< Capabilities >( cap1 ).or( cap2 );
  * Flags< Capabilities >& mode2 = mode1 | cap3;
  * \endcode
  *
  * Inspired by http://code.google.com/p/bitwise-enum/
  *
  * \author Leonid Kostrykin
  * \date   20.9.2011 - 30.7.2012
  */
template< typename Enum >
class Flags
{

public:

    /** \brief	Instantiates with state \c 0.
      */
    Flags()
        : state( 0 )
    {
    }

    /** \brief	Instantiates with given state.
      */
    Flags( Enum state )
        : state( state )
    {
    }


    /** \brief	Returns the result of the bitwise \c OR operation of itself and the given state.
      */
    Flags< Enum > or( Enum state ) const
    {
        return ( *this ) | Flags< Enum >( state );
    }


    /** \brief  Returns whether current state is not equal to \c 0.
      */
    operator bool() const
    {
        return state != 0;
    }

    /** \brief  Bitwise \c OR operation
      */
    Flags< Enum >& operator|=( Flags< Enum > e )
    {
        state |= e.state;
        return *this;
    }

    /** \brief  Bitwise \c OR operation
      */
    Flags< Enum > operator|( Flags< Enum > e ) const
    {
        Flags< Enum > r( *this );
        r |= e;
        return r;
    }

    /** \brief  Bitwise \c AND operation
      */
    Flags< Enum >& operator&=( Flags< Enum > e )
    {
        state &= e.state;
        return *this;
    }

    /** \brief  Bitwise \c AND operation
      */
    Flags< Enum > operator&( Flags< Enum > e ) const
    {
        Flags< Enum > r( *this );
        r &= e;
        return r;
    }

    /** \brief	Tells whether the specified flag is set
      */
    bool isSet( Enum flag ) const
    {
        return this->operator&( Flags< Enum >( flag ) );
    }


private:

    /** \brief  Holds current state.
      */
    int state;

}; // Flags



}  // namespace Carna :: base

}  // namespace Carna

#endif // FLAGS_H_6014714286
