/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
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

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// TestScene
// ----------------------------------------------------------------------------------

class TestScene
{

    std::unique_ptr< base::HUVolumeUInt16 > huVolume;

    base::Geometry* const myVolumeGeometry;

    base::Camera* const myCam;

    base::math::Vector3f spacings;

public:

    const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = 0;

    const static unsigned int ROLE_HU_VOLUME = 0;

    TestScene();

    ~TestScene();

    const std::unique_ptr< base::Node > root;

    base::Geometry& volumeGeometry() const;

    base::Camera& cam() const;

    void resetCamTransform();

    const base::HUVolumeUInt16& volume() const;

    base::math::Vector3f scale() const;

    const base::math::Vector3f& getSpacings() const;

}; // TestScene



}  // namespace testing

}  // namespace Carna
