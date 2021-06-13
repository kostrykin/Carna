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

#include <TestScene.h>
#include <HUGZSceneFactory.h>
#include <Carna/base/Node.h>
#include <Carna/base/math.h>
#include <Carna/base/Camera.h>
#include <Carna/base/Geometry.h>
#include <Carna/base/BufferedVectorFieldTexture.h>
#include <string>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// TestScene
// ----------------------------------------------------------------------------------

TestScene::TestScene()
    : myVolumeGeometry( new base::Geometry( GEOMETRY_TYPE_VOLUMETRIC ) )
    , myCam( new base::Camera() )
    , root( new base::Node() )
{
    /* Load test volume data.
     */
    huVolume.reset( HUGZSceneFactory::importVolume( std::string( SOURCE_PATH ) + "/res/pelves_reduced.hugz", spacings ) );

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



}  // namespace Carna :: testing

}  // namespace Carna
