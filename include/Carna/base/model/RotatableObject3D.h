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

#ifndef ROTATABLEOBJECT3D_H_6014714286
#define ROTATABLEOBJECT3D_H_6014714286

/** \file   RotatableObject3D.h
  * \brief  Defines \ref Carna::base::model::RotatableObject3D.
  */

#include <Carna/base/model/Object3D.h>
#include <Carna/base/qt/RotatingColor.h>

namespace Carna
{

namespace base
{

namespace model
{



// ----------------------------------------------------------------------------------
// RotatableObject3D
// ----------------------------------------------------------------------------------

/** \brief  Abstract \ref Object3D implementation with rotation support.
  *
  * \author Leonid Kostrykin
  * \date   30.3.12 - 21.3.13
  */
class CARNA_EXPORT RotatableObject3D : public Object3D
{

public:

    /** \brief  Instantiates at position \f$ (0,0,0)^\mathrm T \f$.
      */
    explicit RotatableObject3D( Scene&, const std::string& name = createDefaultName() );

    /** \brief  Instantiates at given position.
      */
    RotatableObject3D( Scene&, const Position& position, const std::string& name = createDefaultName() );

    /** \brief  Destroys itself.
      */
    virtual ~RotatableObject3D();

    
    /** \brief  Tells the local coordinate system of this object.
      *
      * The value returned depends on the value passed to \ref setRotation priorly.
      * The initial value is identity.
      */
    virtual const Transformation& rotation() const override;

    /** \brief  Defines the local coordinate system of this object.
      *
      * Emits the \ref rotated signal.
      *
      * Also the \ref Scene::objectsChanged signal is emitted, though maybe delayed.
      */
    virtual void setRotation( const Transformation& ) override;


private:

    class Details;
    const std::unique_ptr< Details > pimpl;

}; // RotatableObject3D



}  // namesapce Carna :: base :: model

}  // namespace Carna :: base

}  // namespace Carna

#endif // ROTATABLEOBJECT3D_H_6014714286
