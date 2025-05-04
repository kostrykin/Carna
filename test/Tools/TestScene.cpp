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

#include <TestScene.h>
#include <HUGZSceneFactory.h>
#include <LibCarna/base/Node.h>
#include <LibCarna/base/math.h>
#include <LibCarna/base/Camera.h>
#include <LibCarna/base/Geometry.h>
#include <LibCarna/base/BufferedVectorFieldTexture.h>
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
    huVolume.reset( HUGZSceneFactory::importVolume( std::string( SOURCE_PATH ) + "/res/pelves_reduced.hugz", spacings, stretchIntensities ) );

    /* Configure geometry node for volume data.
     */
    base::BufferedVectorFieldTexture< base::HUVolumeUInt16 >& volumeTexture
        = base::BufferedVectorFieldTexture< base::HUVolumeUInt16 >::create( *huVolume );
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
        ( ( huVolume->size.x() - 1 ) * spacings.x()
        , ( huVolume->size.y() - 1 ) * spacings.y()
        , ( huVolume->size.z() - 1 ) * spacings.z() );
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


const base::HUVolumeUInt16& TestScene::volume() const
{
    return *huVolume;
}


const base::math::Vector3f& TestScene::getSpacings() const
{
    return spacings;
}



}  // namespace LibCarna :: testing

}  // namespace LibCarna
