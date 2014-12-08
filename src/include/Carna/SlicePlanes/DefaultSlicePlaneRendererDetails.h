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

#ifndef SLICEPLANES_DEFAULTSLICEPLANERENDERER_DETAILS_H_6014714286
#define SLICEPLANES_DEFAULTSLICEPLANERENDERER_DETAILS_H_6014714286

/** \file   DefaultSlicePlaneRendererDetails.h
  * \brief  Defines \ref Carna::SlicePlanes::DefaultSlicePlaneRenderer::Details.
  */

#include <Carna/SlicePlanes/DefaultSlicePlaneRenderer.h>

namespace Carna
{

namespace SlicePlanes
{



// ----------------------------------------------------------------------------------
// DefaultSlicePlaneRenderer :: Details
// ----------------------------------------------------------------------------------

class DefaultSlicePlaneRenderer :: Details : public QObject
{

    Q_OBJECT

    NON_COPYABLE

    DefaultSlicePlaneRenderer& self;

public:

    /** \brief  Instantiates.
      */
    explicit Details( DefaultSlicePlaneRenderer& );

    /** \brief  Releases acquired GL resources.
      */
    virtual ~Details();


    std::unique_ptr< SlicePlane > plane;

    std::unique_ptr< base::view::ShaderBundle > shader;


    base::Vector normal;

    double distance;

    float border;

    const base::Vector proportions;

    const base::Vector scale;


public slots:

    void rebuildTexture();

    void rebuildTextureAndUpdate();

}; // DefaultSlicePlaneRenderer :: Details



}  // namespace Carna :: SlicePlanes

}  // namespace Carna

#endif // SLICEPLANES_DEFAULTSLICEPLANERENDERER_DETAILS_H_6014714286
