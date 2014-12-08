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

#ifndef OBJECT3DEVENT_H_6014714286
#define OBJECT3DEVENT_H_6014714286

/** \file   Object3DEvent.h
  * \brief  Defines \ref Carna::base::model::Object3DEvent.
  */

#include <Carna/base/Flags.h>
#include <Carna/Carna.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// Object3DEvent
// ----------------------------------------------------------------------------------

/** \brief  Describes an event concerning the \ref Object3D stock.
  *
  * \author Leonid Kostrykin
  * \date   20.9.2011
  */
class CARNA_EXPORT Object3DEvent
{

public:

    /** \brief  Lists possible \ref Object3D stock changes.
      */
    enum Feature
    {
        existence = ( 1 << 0 ), ///< \brief When flag is set, objects might have been added or deleted.
        name      = ( 1 << 1 ), ///< \brief When flag is set, an object's name might have changed.
        position  = ( 1 << 2 ), ///< \brief When flag is set, an object's position might have changed.
        rotation  = ( 1 << 3 ), ///< \brief When flag is set, an object's rotation might have changed.
        shape     = ( 1 << 4 )  ///< \brief When flag is set, an object's color, visual representation or icon might have changed.
    };

    /** \brief  Represents changes to any feature.
      */
    const static Flags< Feature > any;


    /** \brief  Instantiates.
      *
      * \param  changes flags which features are to be invalidated by this event.
      */
    explicit Object3DEvent( Flags< Feature > changes = any )
        : changes( changes )
    {
    }


    /** Tells whether this event might invalidate any \a features.
      */
    bool mightAffect( Flags< Feature > features ) const
    {
        return changes & features;
    }


    /** \brief  References the underlying flags.
      *
      * \since  \ref v_2_2_2
      */
    const Flags< Feature >& flags() const
    {
        return changes;
    }


private:

    /** \brief  Flags which features are to be invalidated by this event.
      */
    Flags< Feature > changes;

}; // Object3DEvent



}  // namesapce Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // OBJECT3DEVENT_H_6014714286
