#include <QGLWidget>
#include <QGLFormat>
#include <QMouseEvent>
#include <QWheelEvent>

#include <Carna/Version.h>
#include <Carna/base/text.h>
#include <Carna/base/Log.h>
#include <Carna/base/GLContext.h>
#include <Carna/base/FrameRenderer.h>
#include <Carna/base/Color.h>
#include <Carna/base/Node.h>
#include <Carna/base/Camera.h>
#include <Carna/base/Geometry.h>
#include <Carna/base/GeometryFeature.h>
#include <Carna/base/BufferedVectorFieldTexture.h>
#include <Carna/base/Material.h>
#include <Carna/base/MeshFactory.h>
#include <Carna/base/Vertex.h>
#include <Carna/base/ShaderUniform.h>
#include <Carna/base/BufferedHUVolume.h>
#include <Carna/base/SpatialMovement.h>
#include <Carna/presets/CameraShowcaseControl.h>
#include <Carna/presets/OccludedRenderingStage.h>
#include <Carna/presets/OpaqueRenderingStage.h>
#include <Carna/presets/MeshColorCodingStage.h>
#include <Carna/presets/DVRStage.h>
#include <Carna/presets/DRRStage.h>
#include <Carna/presets/CuttingPlanesStage.h>
#include <Carna/helpers/VolumeGridHelper.h>

#include <TestApplication.h>
#include <HUGZSceneFactory.h>

#include <memory>
#include <iomanip>

// ----------------------------------------------------------------------------------

CARNA_ASSERT_API_VERSION( EXPECTED_MAJOR_VERSION, EXPECTED_MINOR_VERSION, EXPECTED_RELEASE_VERSION )

// ----------------------------------------------------------------------------------



namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// Demo
// ----------------------------------------------------------------------------------

class Demo : public QGLWidget
{

    const static int GEOMETRY_TYPE_VOLUMETRIC    = 0;
    const static int GEOMETRY_TYPE_OPAQUE        = 1;
    const static int GEOMETRY_TYPE_CUTTING_PLANE = 2;

    typedef Carna::helpers::VolumeGridHelper< Carna::base::HUVolumeUInt16, Carna::base::NormalMap3DInt8 > GridHelper;

    std::unique_ptr< GridHelper > gridHelper;
    std::unique_ptr< base::GLContext > glContext;
    std::unique_ptr< base::FrameRenderer > renderer;
    std::unique_ptr< base::Node > root;
    base::Camera* camera;
    presets::MeshColorCodingStage* mccs;
    std::unique_ptr< base::SpatialMovement > spatialMovement;

    presets::CameraShowcaseControl cameraControl;
    bool mouseInteraction;
    QPoint mousepos;

public:

    Demo();

    virtual ~Demo();

protected:

    virtual void initializeGL() override;

    virtual void resizeGL( int w, int h ) override;

    virtual void paintGL() override;
    
    virtual void mousePressEvent( QMouseEvent* ev ) override;
    
    virtual void mouseMoveEvent( QMouseEvent* ev ) override;
    
    virtual void mouseReleaseEvent( QMouseEvent* ev ) override;

    virtual void wheelEvent( QWheelEvent* ev ) override;

}; // Demo


Demo::Demo()
    : QGLWidget( Carna::base::QGLContextAdapter< QGLContext, QGLFormat >::desiredFormat() )
    , mouseInteraction( false )
{
    setMouseTracking( true );
}


Demo::~Demo()
{
    if( renderer.get() != nullptr )
    {
        renderer->glContext().makeCurrent();
    }
}


void Demo::mousePressEvent( QMouseEvent* ev )
{
    mouseInteraction = true;
    const base::Aggregation< const base::Geometry > picked = mccs->pick( ev->x(), ev->y() );
    if( picked.get() == nullptr )
    {
        mousepos = ev->pos();
    }
    else
    {
        base::Geometry& pickedGeometry = const_cast< base::Geometry& >( *picked );
        spatialMovement.reset( new base::SpatialMovement( pickedGeometry, ev->x(), ev->y(), renderer->viewport(), *camera ) );
    }
    ev->accept();
}


void Demo::mouseMoveEvent( QMouseEvent* ev )
{
    const static float ROTATION_SPEED = -1e-2f;
    if( mouseInteraction )
    {
        if( spatialMovement.get() == nullptr )
        {
            const int dx = ( ev->x() - mousepos.x() );
            const int dy = ( ev->y() - mousepos.y() );
            mousepos = ev->pos();

            if( dx != 0 || dy != 0 )
            {
                cameraControl.rotateHorizontally( dx * ROTATION_SPEED );
                cameraControl.rotateVertically  ( dy * ROTATION_SPEED );
                updateGL();
                ev->accept();
            }
        }
        else
        {
            if( spatialMovement->update( ev->x(), ev->y() ) )
            {
                updateGL();
                ev->accept();
            }
        }
    }
}


void Demo::mouseReleaseEvent( QMouseEvent* ev )
{
    mouseInteraction = false;
    spatialMovement.reset();
    ev->accept();
}


void Demo::wheelEvent( QWheelEvent* ev )
{
    const static float SPEED = -5e-2f;
    cameraControl.moveAxially( ev->delta() * SPEED );
    updateGL();
    ev->accept();
}


void Demo::initializeGL()
{
    glContext.reset( new base::QGLContextAdapter< QGLContext, QGLFormat >() );
    root.reset( new base::Node() );

    base::math::Vector3f spacing;
    std::unique_ptr< base::HUVolumeUInt16 > baseVolume
        ( HUGZSceneFactory::importVolume( std::string( SOURCE_PATH ) + "/../res/pelves_reduced.hugz", spacing ) );
    gridHelper.reset( new GridHelper
        ( baseVolume->size
        , baseVolume->size.x() * baseVolume->size.y() * baseVolume->size.z() * sizeof( base::HUVolumeUInt16::Voxel ) / 50 ) );
    gridHelper->loadData( *baseVolume );
    base::Node* const volumeNode = gridHelper->createNode( GEOMETRY_TYPE_VOLUMETRIC, GridHelper::Spacing( spacing ) );

    base::ManagedMeshBase& boxMesh = base::MeshFactory< base::VertexBase >::createBox( 10, 10, 10 );
    base::Material& boxMaterial = base::Material::create( "unshaded" );
    boxMaterial.setParameter( "color", base::math::Vector4f( 1, 0, 0, 1 ) );
    base::Geometry* const boxGeometry = new base::Geometry( GEOMETRY_TYPE_OPAQUE );
    boxGeometry->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MATERIAL, boxMaterial );
    boxGeometry->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MESH, boxMesh );
    boxGeometry->localTransform = base::math::translation4f( 0, 30, 50 );

    gridHelper->releaseGeometryFeatures();
    boxMaterial.release();
    boxMesh.release();

    camera = new base::Camera();
    camera->setProjection( base::math::frustum4f( 3.14f * 45 / 180.f, 1, 10, 2000 ) );
    camera->localTransform = base::math::translation4f( 0, 0, 350 );
    cameraControl.setCamera( *camera );
    root->attachChild( camera );
    root->attachChild( volumeNode );
    root->attachChild( boxGeometry );

    base::Geometry* const plane1 = new base::Geometry( GEOMETRY_TYPE_CUTTING_PLANE );
    plane1->localTransform = base::math::plane4f( base::math::Vector3f( 1, 1, 1 ).normalized(), 0 );
    root->attachChild( plane1 );
}


void Demo::resizeGL( int w, int h )
{
    const static bool fitSquare = true;
    if( renderer.get() == nullptr )
    {
        renderer.reset( new base::FrameRenderer( *glContext, static_cast< unsigned >( w ), static_cast< unsigned >( h ), fitSquare ) );

        /* Picking
         */
        mccs = new presets::MeshColorCodingStage();
        mccs->putGeometryType( GEOMETRY_TYPE_OPAQUE, presets::OpaqueRenderingStage::ROLE_DEFAULT_MESH );
        renderer->appendStage( mccs );

        /* Cutting Planes
         */
        presets::CuttingPlanesStage* const cuttingPlanes
            = new presets::CuttingPlanesStage( GEOMETRY_TYPE_VOLUMETRIC, GEOMETRY_TYPE_CUTTING_PLANE );
        cuttingPlanes->setWindowingWidth( 1000 );
        cuttingPlanes->setWindowingLevel( -100 );
        renderer->appendStage( cuttingPlanes );

        /* Occluded Renderer
         */
        presets::OccludedRenderingStage* const occluded = new presets::OccludedRenderingStage();
        renderer->appendStage( occluded );

        /* Opaque
         */
        presets::OpaqueRenderingStage* const opaque = new presets::OpaqueRenderingStage( GEOMETRY_TYPE_OPAQUE );
        renderer->appendStage( opaque );
        occluded->enableStage( *opaque );
#if 0
        /* DVR
         */
        presets::DVRStage* const dvr = new presets::DVRStage( GEOMETRY_TYPE_VOLUMETRIC );
        dvr->writeColorMap( -400,   0, base::Color:: BLUE_NO_ALPHA, base::Color:: BLUE );
        dvr->writeColorMap(    0, 400, base::Color::GREEN_NO_ALPHA, base::Color::GREEN );
        dvr->setSampleRate( 500 );
        dvr->setTranslucence( 2 );
        renderer->appendStage( dvr );
#else
        /* DRR
         */
        presets::DRRStage* const drr = new presets::DRRStage( GEOMETRY_TYPE_VOLUMETRIC );
        renderer->appendStage( drr );
#endif
    }
    else
    {
        renderer->reshape( static_cast< unsigned >( w ), static_cast< unsigned >( h ), fitSquare );
    }
}


void Demo::paintGL()
{
    renderer->render( *camera, *root );
    std::stringstream title;
    title
        << "Carna Demo ("
        << "FPS: " << std::setiosflags( std::ios::fixed ) << std::setprecision( 1 ) << renderer->framesPerSecond().mean
        << " +/- " << std::setiosflags( std::ios::fixed ) << std::setprecision( 1 ) << renderer->framesPerSecond().standardDeviation()
        << ")";
    setWindowTitle( QString::fromStdString( title.str() ) );
}


}  // namespace Carna :: testing

}  // namespace Carna



// ----------------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------------

int main( int argc, char** argv )
{
    Carna::testing::TestApplication app( argc, argv );
    Carna::testing::Demo demo;
    demo.show();
    return QApplication::exec();
}
