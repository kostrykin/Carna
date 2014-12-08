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

#ifndef DEFAULTOBJECT3DPAINTER_H_6014714286
#define DEFAULTOBJECT3DPAINTER_H_6014714286

/** \file   DefaultObject3DPainter.h
  * \brief  Defines \ref Carna::base::view::DefaultObject3DPainter.
  */

#include <Carna/base/view/Object3DPainter.h>
#include <Carna/base/model/Object3DUniformFilter.h>
#include <memory>

namespace Carna
{

namespace base
{

namespace view
{



// ----------------------------------------------------------------------------------
// DefaultObject3DPainter
// ----------------------------------------------------------------------------------

/** \brief  Paints all Object3D instances associated with a \ref model::Scene "data model".
  *
  * \author Leonid Kostrykin
  * \date   14.12.11 - 15.12.11
  */
class CARNA_EXPORT DefaultObject3DPainter : public Object3DPainter
{

public:

    /** \brief  Instantiates.
      */
    explicit DefaultObject3DPainter
        ( const model::Scene& model
        , model::Object3DFilter* filter = new model::Object3DUniformFilter( true ) )

        : model( model )
        , filter( filter )
    {
    }


    /** \brief  Paints all 3D objects.
      */
    virtual void paint( const Renderer&, Object3DPaintingRoutine& ) const override;

    
    /** \brief  Associated data model.
      */
    const model::Scene& model;


private:

    const std::unique_ptr< model::Object3DFilter > filter;

}; // DefaultObject3DPainter



}  // namespace Carna :: base :: view

}  // namespace Carna :: base

}  // namespace Carna

#endif // DEFAULTOBJECT3DPAINTER_H_6014714286
