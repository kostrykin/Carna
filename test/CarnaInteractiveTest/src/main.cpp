#include <QDebug>
#include <QGLWidget>
#include <QApplication>
#include <QMouseEvent>

#include <Carna/Version.h>
#include <Carna/base/Log.h>
#include <Carna/base/GLContext.h>
#include <Carna/base/FrameRenderer.h>
#include <Carna/base/Node.h>
#include <Carna/base/Camera.h>
#include <Carna/base/Geometry.h>
#include <Carna/base/GeometryFeature.h>
#include <Carna/base/BufferedHUVolumeTexture.h>
#include <Carna/base/MeshRenderingStage.h>
#include <Carna/base/MeshColorCodingStage.h>
#include <Carna/base/Material.h>
#include <Carna/base/MeshFactory.h>
#include <Carna/base/Vertex.h>
#include <Carna/base/ShaderUniform.h>
#include <Carna/base/BufferedHUVolume.h>
#include <Carna/VolumeRenderings/MIP/MIPStage.h>
#include <Carna/VolumeRenderings/MIP/Channel.h>
#include <Carna/VolumeRenderings/DRR/DRRStage.h>
#include <Carna/CuttingPlanes/CuttingPlanesStage.h>

#include <HUGZSceneFactory.h>

#include <memory>

// ----------------------------------------------------------------------------------

CARNA_ASSERT_API_VERSION( EXPECTED_MAJOR_VERSION, EXPECTED_MINOR_VERSION, EXPECTED_RELEASE_VERSION )

// ----------------------------------------------------------------------------------



namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// QDebugLogWriter
// ----------------------------------------------------------------------------------

class QDebugLogWriter : public base::Log::TextWriter
{

protected:

    virtual void writeFormatted( base::Log::Severity, const std::string& ) const override;

};


void QDebugLogWriter::writeFormatted( base::Log::Severity severity, const std::string& msg ) const
{
    switch( severity )
    {

    case base::Log::fatal:
    case base::Log::error:
        qFatal( "%s", msg.c_str() );
        break;

    case base::Log::warning:
        qWarning( "%s", msg.c_str() );
        break;

    case base::Log::debug:
        qDebug( "%s", msg.c_str() );
        break;

    }
}



// ----------------------------------------------------------------------------------
// Demo
// ----------------------------------------------------------------------------------

class Demo : public QGLWidget
{

    const static int GEOMETRY_TYPE_VOLUMETRIC    = 0;
    const static int GEOMETRY_TYPE_OPAQUE        = 1;
    const static int GEOMETRY_TYPE_CUTTING_PLANE = 2;

    const static std::string USER_DATA_EXAMPLE;

    std::unique_ptr< base::UInt16HUVolume > volume;
    std::unique_ptr< base::GLContext > glContext;
    std::unique_ptr< base::FrameRenderer > renderer;
    std::unique_ptr< base::Node > root;
    base::Camera* camera;
    base::MeshColorCodingStage* mccs;

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

}; // Demo


const std::string Demo::USER_DATA_EXAMPLE = "Test";


Demo::Demo()
    : mouseInteraction( false )
{
    setMouseTracking( true );
}


Demo::~Demo()
{
    if( renderer.get() != nullptr )
    {
        renderer->glContext().makeActive();
    }
}


void Demo::mousePressEvent( QMouseEvent* ev )
{
    const base::Aggregation< const base::Geometry > picked = mccs->pick( ev->x(), ev->y() );
    if( picked.get() == nullptr )
    {
        mouseInteraction = true;
        mousepos = ev->pos();
    }
    else
    {
        const std::string& userData = picked->userData< std::string >();
    }
    ev->accept();
}


void Demo::mouseMoveEvent( QMouseEvent* ev )
{
    const static float ROTATION_SPEED = 1e-2f;
    if( mouseInteraction )
    {
        const int dx = ( ev->x() - mousepos.x() );
        const int dy = ( ev->y() - mousepos.y() );
        mousepos = ev->pos();

        if( dx != 0 )
        {
            base::math::Matrix4f rotation = base::math::rotation4f( 0, 1, 0, dx * ROTATION_SPEED );
            camera->localTransform = rotation * camera->localTransform;
            updateGL();
            ev->accept();
        }
    }
}


void Demo::mouseReleaseEvent( QMouseEvent* ev )
{
    mouseInteraction = false;
    ev->accept();
}


void Demo::initializeGL()
{
    glContext.reset( new base::QGLContextAdapter< QGLContext >() );
    root.reset( new base::Node() );

    base::math::Vector3f spacing;
    volume.reset( HUGZSceneFactory::importVolume( std::string( SOURCE_PATH ) + "/../res/pelves_reduced.hugz", spacing ) );
    const base::math::Vector3f scale
        ( ( volume->size.x() - 1 ) * spacing.x()
        , ( volume->size.y() - 1 ) * spacing.y()
        , ( volume->size.z() - 1 ) * spacing.z() );

    auto& volumeTexture = base::BufferedHUVolumeTexture< base::UInt16HUVolume >::create( *volume );

    base::Geometry* const volumeGeometry1 = new base::Geometry( GEOMETRY_TYPE_VOLUMETRIC );
    volumeGeometry1->putFeature( VolumeRenderings::MIP::MIPStage::ROLE_HU_VOLUME, volumeTexture );

    base::Geometry* const volumeGeometry2 = new base::Geometry( GEOMETRY_TYPE_VOLUMETRIC );
    volumeGeometry2->putFeature( VolumeRenderings::MIP::MIPStage::ROLE_HU_VOLUME, volumeTexture );

    base::MeshBase& boxMesh = base::MeshFactory< base::VertexBase >::createBox( 10, 10, 10 );
    base::Material& boxMaterial = base::Material::create( "unshaded" );
    boxMaterial.addUniform( new base::ShaderUniform< base::math::Vector4f >( "color", base::math::Vector4f( 1, 0, 0, 1 ) ) );
    base::Geometry* const boxGeometry = new base::Geometry( GEOMETRY_TYPE_OPAQUE );
    boxGeometry->putFeature( base::OpaqueRenderingStage::ROLE_DEFAULT_MATERIAL, boxMaterial );
    boxGeometry->putFeature( base::OpaqueRenderingStage::ROLE_DEFAULT_MESH, boxMesh );
    boxGeometry->localTransform = base::math::translation4f( 0, 0, 50 );
    boxGeometry->setUserData( USER_DATA_EXAMPLE );

    volumeTexture.release();
    boxMaterial.release();
    boxMesh.release();

    base::Node* const volumePivot1 = new base::Node();
    volumePivot1->attachChild( volumeGeometry1 );
    volumePivot1->localTransform = base::math::translation4f( -scale.x() / 2, 0, 0 ) * base::math::scaling4f( scale );

    base::Node* const volumePivot2 = new base::Node();
    volumePivot2->attachChild( volumeGeometry2 );
    volumePivot2->localTransform = base::math::translation4f( +scale.x() / 2, 0, 0 ) * base::math::scaling4f( scale );

    camera = new base::Camera();
    camera->setProjection( base::math::frustum4f( 3.14f * 45 / 180.f, 1, 10, 2000 ) );
    camera->localTransform = base::math::translation4f( 0, 0, 500 );
    root->attachChild( camera );
    root->attachChild( volumePivot1 );
    root->attachChild( volumePivot2 );
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
        mccs = new base::MeshColorCodingStage();
        mccs->putGeometryType( GEOMETRY_TYPE_OPAQUE, base::OpaqueRenderingStage::ROLE_DEFAULT_MESH );
        renderer->appendStage( mccs );

        /* Opaque
         */
        base::OpaqueRenderingStage* const opaque = new base::OpaqueRenderingStage( GEOMETRY_TYPE_OPAQUE );
        renderer->appendStage( opaque );

        /* Cutting Planes
         */
        CuttingPlanes::CuttingPlanesStage* const cuttingPlanes
            = new CuttingPlanes::CuttingPlanesStage( GEOMETRY_TYPE_VOLUMETRIC, GEOMETRY_TYPE_CUTTING_PLANE );
        cuttingPlanes->setWindowingWidth( 1000 );
        cuttingPlanes->setWindowingLevel( -100 );
        renderer->appendStage( cuttingPlanes );
#if 0
        /* MIP
         */
        VolumeRenderings::MIP::MIPStage* const mip = new VolumeRenderings::MIP::MIPStage( GEOMETRY_TYPE_VOLUMETRIC );
        mip->appendChannel( new VolumeRenderings::MIP::Channel( -1024, 0, base::math::Vector4f( 0, 0, 1, 0.5f ) ) );
        mip->appendChannel( new VolumeRenderings::MIP::Channel( 0, 3071, base::math::Vector4f( 1, 1, 0, 0.5f ) ) );
        renderer->appendStage( mip );
#else
        /* DRR
         */
        VolumeRenderings::DRR::DRRStage* const drr = new VolumeRenderings::DRR::DRRStage( GEOMETRY_TYPE_VOLUMETRIC );
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
}


}  // namespace Carna :: testing

}  // namespace Carna



// ----------------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------------

int main( int argc, char** argv )
{
    Carna::base::Log::instance().setWriter( new Carna::testing::QDebugLogWriter() );
    QApplication app( argc, argv );
    Carna::testing::Demo demo;
    demo.resize( 400, 400 );
    demo.show();
    return QApplication::exec();
}
