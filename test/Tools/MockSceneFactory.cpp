#include "MockSceneFactory.h"
#include <Carna/base/model/Scene.h>
#include <Carna/base/model/UInt16Volume.h>
#include <Carna/base/Composition.h>



// ----------------------------------------------------------------------------------
// MockSceneFactory :: Dimensions
// ----------------------------------------------------------------------------------

MockSceneFactory::Dimensions::Dimensions( double width, double height, double depth )
    : width( width )
    , height( height )
    , depth( depth )
{
}


MockSceneFactory::Dimensions MockSceneFactory::Dimensions::fromCentimeters( double width, double height, double depth )
{
    return Dimensions( width * 10, height * 10, depth * 10 );
}



// ----------------------------------------------------------------------------------
// MockSceneFactory
// ----------------------------------------------------------------------------------

MockSceneFactory::MockSceneFactory( const Dimensions& dimensions, double voxelsPerMillimeter )
    : volumeSize( [&]()->Carna::base::Vector3ui
            {
                Carna::base::Vector3ui size;

                size.x = static_cast< unsigned int >( dimensions.width  * voxelsPerMillimeter + 0.5 );
                size.y = static_cast< unsigned int >( dimensions.height * voxelsPerMillimeter + 0.5 );
                size.z = static_cast< unsigned int >( dimensions.depth  * voxelsPerMillimeter + 0.5 );

                return size;
            }
        () )
    , spacingX( 1 / voxelsPerMillimeter )
    , spacingY( 1 / voxelsPerMillimeter )
    , spacingZ( 1 / voxelsPerMillimeter )
{
}


MockSceneFactory::MockSceneFactory( const Carna::base::Vector3ui& volumeSize, const double spacingX, const double spacingY, const double spacingZ )
    : volumeSize( volumeSize )
    , spacingX( spacingX )
    , spacingY( spacingY )
    , spacingZ( spacingZ )
{
}


Carna::base::model::Scene* MockSceneFactory::createMockScene() const
{
    Carna::base::model::UInt16Volume* const volume = new Carna::base::model::UInt16Volume( volumeSize );
    const Carna::base::Vector3ui half_volume_size( volumeSize.x / 2, volumeSize.y / 2, volumeSize.z / 2 );
    for( unsigned int z = 0; z < volumeSize.z; ++z )
    {
        const float center_distance_z = Carna::base::Math::clamp
            ( ( float( z ) - half_volume_size.z ) / half_volume_size.z
            , -1.f
            , +1.f );

        for( unsigned int y = 0; y < volumeSize.y; ++y )
        {
            const float center_distance_y = Carna::base::Math::clamp
                ( ( float( y ) - half_volume_size.y ) / half_volume_size.y
                , -1.f
                , +1.f );

            for( unsigned int x = 0; x < volumeSize.x; ++x )
            {
                const float center_distance_x = Carna::base::Math::clamp
                    ( ( float( x ) - half_volume_size.x ) / half_volume_size.x
                    , -1.f
                    , +1.f );

                const float center_distance = std::sqrt
                    ( center_distance_x * center_distance_x
                    + center_distance_y * center_distance_y
                    + center_distance_z * center_distance_z );

                const signed short huv = static_cast< signed short >( center_distance * 4095 ) - 1024;
                volume->setVoxel( x, y, z, huv );
            }
        }
    }

    Carna::base::model::Scene* const model = new Carna::base::model::Scene
        ( new Carna::base::Composition< Carna::base::model::Volume >( volume )
        , spacingX
        , spacingY
        , spacingZ );

    return model;
}
