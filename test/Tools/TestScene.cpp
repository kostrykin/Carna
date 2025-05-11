/*
 *  Copyright (C) 2010 - 2016 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 * 
 * 
 *  Copyright (C) 2021 - 2025 Leonid Kostrykin
 * 
 */

#include <TestScene.hpp>
#include <HUGZSceneFactory.hpp>
#include <LibCarna/base/Node.hpp>
#include <LibCarna/base/math.hpp>
#include <LibCarna/base/Camera.hpp>
#include <LibCarna/base/Geometry.hpp>
#include <LibCarna/base/BufferedVectorFieldTexture.hpp>
#include <string>

namespace LibCarna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// TestScene
// ----------------------------------------------------------------------------------

TestScene::TestScene( bool stretchIntensities )
    : myVolumeGeometry( new base::Geometry( GEOMETRY_TYPE_VOLUMETRIC ) )
    , myCam( new base::Camera() )
    , root( new base::Node() )
{
    /* Load test volume data.
     */
    myVolume.reset( HUGZSceneFactory::importVolume( std::string( SOURCE_PATH ) + "/res/pelves_reduced.hugz", spacings, stretchIntensities ) );

    /* Configure geometry node for volume data.
     */
    base::BufferedVectorFieldTexture< base::IntensityVolumeUInt16 >& volumeTexture
        = base::BufferedVectorFieldTexture< base::IntensityVolumeUInt16 >::create( *myVolume );
    myVolumeGeometry->putFeature( ROLE_HU_VOLUME, volumeTexture );
    myVolumeGeometry->localTransform = base::math::scaling4f( scale() );
    root->attachChild( myVolumeGeometry );
    volumeTexture.release();

    /* Configure camera node.
     */
    myCam->setProjection( base::math::frustum4f( 3.14f * 90 / 180.f, 1, 10, 2000 ) );
    resetCamTransform();
    root->attachChild( myCam );
}


TestScene::~TestScene()
{
}


base::math::Vector3f TestScene::scale() const
{
    const base::math::Vector3f scale
        ( ( myVolume->size.x() - 1 ) * spacings.x()
        , ( myVolume->size.y() - 1 ) * spacings.y()
        , ( myVolume->size.z() - 1 ) * spacings.z() );
    return scale;
}


base::Geometry& TestScene::volumeGeometry() const
{
    return *myVolumeGeometry;
}


base::Camera& TestScene::cam() const
{
    return *myCam;
}


void TestScene::resetCamTransform()
{
    myCam->localTransform = base::math::translation4f( 0, 0, 350 );
}


const base::IntensityVolumeUInt16& TestScene::volume() const
{
    return *myVolume;
}


const base::math::Vector3f& TestScene::getSpacings() const
{
    return spacings;
}



}  // namespace LibCarna :: testing

}  // namespace LibCarna
