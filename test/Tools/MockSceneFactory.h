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

#pragma once

#include <Carna/Carna.h>
#include <Carna/base/Vector3.h>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// MockSceneFactory
// ----------------------------------------------------------------------------------

class MockSceneFactory 
{

public:

    struct Dimensions
    {
        double width;
        double height;
        double depth;

        Dimensions( double width, double height, double depth );

        static Dimensions fromCentimeters( double width, double height, double depth );
    };


    MockSceneFactory( const Dimensions& size, double voxelsPerMillimeter );

    MockSceneFactory( const Carna::base::Vector3ui& volumeSize, const double spacingX, const double spacingY, const double spacingZ );


    Carna::base::model::Scene* createMockScene() const;

    const Carna::base::Vector3ui volumeSize;

    const double spacingX;
    const double spacingY;
    const double spacingZ;

}; // MockSceneFactory



}  // namespace testing

}  // namespace Carna
