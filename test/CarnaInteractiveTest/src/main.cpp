#include <QDebug>
#include <QGLWidget>
#include <QApplication>
#include <QMouseEvent>

#include <Carna/Version.h>
#include <Carna/base/Log.h>
#include <Carna/base/view/GLContext.h>
#include <Carna/base/view/FrameRenderer.h>
#include <Carna/base/view/Node.h>
#include <Carna/base/view/Camera.h>
#include <Carna/base/view/Geometry.h>
#include <Carna/base/view/GeometryAggregate.h>
#include <Carna/base/view/BufferedHUVolumeControl.h>
#include <Carna/VolumeRenderings/MIP/MIPStage.h>
#include <Carna/VolumeRenderings/MIP/Channel.h>
#include <Carna/VolumeRenderings/DRR/DRRStage.h>

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

    std::unique_ptr< base::model::UInt16HUVolume > volume;
    std::unique_ptr< base::view::GLContext > glContext;
    std::unique_ptr< base::view::FrameRenderer > renderer;
    std::unique_ptr< base::view::Node > root;
    base::view::Camera* camera;

    bool mouseInteraction;
    QPoint mousepos;

public:

    Demo();

protected:

    virtual void initializeGL() override;

    virtual void resizeGL( int w, int h ) override;

    virtual void paintGL() override;
    
    virtual void mousePressEvent( QMouseEvent* ev ) override;
    
    virtual void mouseMoveEvent( QMouseEvent* ev ) override;
    
    virtual void mouseReleaseEvent( QMouseEvent* ev ) override;

}; // Demo


Demo::Demo()
    : mouseInteraction( false )
{
    setMouseTracking( true );
}


void Demo::mousePressEvent( QMouseEvent* ev )
{
    mouseInteraction = true;
    mousepos = ev->pos();
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
    glContext.reset( new base::view::QGLContextAdapter< QGLContext >() );
    root.reset( new base::view::Node() );

    base::math::Vector3f spacing;
    volume.reset( HUGZSceneFactory::importVolume( std::string( SOURCE_PATH ) + "/../res/pelves_reduced.hugz", spacing ) );
    const base::math::Vector3f scale
        ( ( volume->size.x() - 1 ) * spacing.x()
        , ( volume->size.y() - 1 ) * spacing.y()
        , ( volume->size.z() - 1 ) * spacing.z() );

    base::view::Geometry* const volumeGeometry = new base::view::Geometry( VolumeRenderings::MIP::MIPStage::GEOMETRY_TYPE );
    volumeGeometry->putAggregate
        ( base::view::GeometryAggregate::create( new base::view::BufferedHUVolumeControl< base::model::UInt16HUVolume >( *volume ) )
        , VolumeRenderings::MIP::MIPStage::ROLE_HU_VOLUME );

    base::view::Node* const volumePivot = new base::view::Node();
    volumePivot->attachChild( volumeGeometry );
    volumePivot->localTransform = base::math::scaling4f( scale );

    camera = new base::view::Camera();
    camera->setProjection( base::math::frustum4f( 3.14f * 45 / 180.f, 1, 10, 2000 ) );
    camera->localTransform = base::math::translation4f( 0, 0, 500 );
    root->attachChild( camera );
    root->attachChild( volumePivot );
}


void Demo::resizeGL( int w, int h )
{
    const static bool fitSquare = true;
    if( renderer.get() == nullptr )
    {
        renderer.reset( new base::view::FrameRenderer( *glContext, static_cast< unsigned >( w ), static_cast< unsigned >( h ), fitSquare ) );

#if 0
        VolumeRenderings::MIP::MIPStage* const mip = new VolumeRenderings::MIP::MIPStage();
        mip->appendChannel( new VolumeRenderings::MIP::Channel( -1024, 0, base::math::Vector4f( 0, 0, 1, 1 ) ) );
        mip->appendChannel( new VolumeRenderings::MIP::Channel( 0, 3071, base::math::Vector4f( 1, 1, 0, 1 ) ) );
        renderer->appendStage( mip );
#else
        VolumeRenderings::DRR::DRRStage* const drr = new VolumeRenderings::DRR::DRRStage();
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
    demo.show();
    return QApplication::exec();
}
