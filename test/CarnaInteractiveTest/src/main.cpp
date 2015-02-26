#include <QDebug>
#include <QGLWidget>
#include <QApplication>

#include <Carna/Version.h>
#include <Carna/base/Log.h>
#include <Carna/base/view/GLContext.h>
#include <Carna/base/view/FrameRenderer.h>
#include <Carna/base/view/Node.h>
#include <Carna/base/view/Camera.h>
#include <Carna/base/view/Geometry.h>
#include <Carna/base/view/GeometryAggregate.h>
#include <Carna/base/view/BufferedHUVolumeManager.h>
#include <Carna/VolumeRenderings/MIP/MIPStage.h>
#include <Carna/VolumeRenderings/MIP/Channel.h>

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

public:

    virtual void initializeGL() override;

    virtual void resizeGL( int w, int h ) override;

    virtual void paintGL() override;

}; // Demo


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
        ( base::view::GeometryAggregate::create( new base::view::BufferedHUVolumeManager< base::model::UInt16HUVolume >( *volume ) )
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
        VolumeRenderings::MIP::MIPStage* const mip = new VolumeRenderings::MIP::MIPStage();
        mip->appendChannel( new VolumeRenderings::MIP::Channel( -1024, 0, base::math::Vector4f( 0, 0, 1, 1 ) ) );
        mip->appendChannel( new VolumeRenderings::MIP::Channel( 0, 3071, base::math::Vector4f( 1, 1, 0, 1 ) ) );
        renderer.reset( new base::view::FrameRenderer( *glContext, static_cast< unsigned >( w ), static_cast< unsigned >( h ), fitSquare ) );
        renderer->appendStage( mip );
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
