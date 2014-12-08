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

#ifndef INCREMENTALCOLORPAINTINGROUTINE_H_6014714286
#define INCREMENTALCOLORPAINTINGROUTINE_H_6014714286

/** \file   IncrementalColorPaintingRoutine.h
  *
  * \brief  Defines \ref Carna::base::view::IncrementalColorPaintingRoutine.
  *
  * \author Leonid Kostrykin
  * \date   2011-2013
  */

#include <Carna/base/view/Object3DPaintingRoutine.h>
#include <Carna/base/noncopyable.h>
#include <map>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// IncrementalColorPaintingRoutine
// ----------------------------------------------------------------------------------

/** \brief  \ref model::Object3D "Object3D" painting routing, drawing each in an unique color.
  *
  * Each painted object is assumed to have an ID which is unique within a single
  * \ref Object3D_Rendering "painting routine" execution. To achieve this, the ID is
  * simply incremented for every invocation. The first ID is \c 1.
  *
  * The unique incremental color is extracted directly from the ID using bit
  * operations. The red component is determined by the bits of the ID which are
  * masked by \c 0x000000FF. The green component is determined by those which are
  * masked by \c 0x0000FF00. And the blue component is determined by those which
  * are masked by \c 0x00FF0000. The color code \c 0x00000000 is never used, for it
  * denotes areas where no object was painted. This implies that
  * \f$ 2^{24} - 1 \f$ uniquely colored objects can be rendered at the most.
  *
  * \author Leonid Kostrykin
  * \date   26.9.11 - 21.3.13
  */
class CARNA_EXPORT IncrementalColorPaintingRoutine : public Object3DPaintingRoutine
{

    NON_COPYABLE

public:

    /** \brief  Maps Object3D to integer.
      */
    typedef std::map< const model::Object3D*, unsigned int > ObjectToIdMap;

    /** \brief  Maps integer to Object3D.
      */
    typedef std::map< unsigned int, const model::Object3D* > IdToObjectMap;


    /** \brief  Instantiates.
      */
    IncrementalColorPaintingRoutine();


    /** \brief  Accesses the mapping, which tells the ID of a rendered object.
      */
    const ObjectToIdMap& idByObjectMap() const
    {
        return idByObject;
    }
    
    /** \brief  Accesses the mapping, which tells the object an ID belongs to.
      */
    const IdToObjectMap& objectByIdMap() const
    {
        return objectById;
    }


    /** \brief  Paints the \a object by invoking it's
      *         \ref model::Object3D::paintFalseColor "paintFalseColor" and increments
      *         the ID used for painting next.
      */
    virtual void operator()( const Renderer& renderer, const model::Object3D& object ) override;


private:

    /** \brief  Holds the ID which will be assigned to the next painted object.
      */
    unsigned int nextID;


    /** \brief  Holds the mapping, which tells the ID of a rendered object.
      */
    ObjectToIdMap idByObject;

    /** \brief  Holds the mapping, which tells the object an ID belongs to.
      */
    IdToObjectMap objectById;

}; // IncrementalColorPaintingRoutine



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // INCREMENTALCOLORPAINTINGROUTINE_H_6014714286
