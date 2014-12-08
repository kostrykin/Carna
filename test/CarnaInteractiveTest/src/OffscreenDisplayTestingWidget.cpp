#include "OffscreenDisplayTestingWidget.h"

#include <Carna/base/OffscreenDisplay.h>
#include <Carna/base/view/Renderer.h>
#include <Carna/base/Visualization.h>
#include <Carna/base/controller/InteractionStrategy.h>

#include <QVBoxLayout>
#include <QLabel>
#include <QResizeEvent>


OffscreenDisplayTestingWidget::OffscreenDisplayTestingWidget
    ( Carna::base::OffscreenVisualizationEnvironment& environment
    , Carna::base::Visualization* module
    , Carna::base::view::SceneProvider& provider
    , QWidget* parent )

    : QWidget( parent )
    , environment( environment )
    , module( module )
    , provider( provider )
    , window( new QLabel() )
{
    window->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

    QVBoxLayout* const layout = new QVBoxLayout();
    this->setLayout( layout );
    layout->addWidget( window );
    layout->setContentsMargins( 0, 0, 0, 0 );
}


OffscreenDisplayTestingWidget::~OffscreenDisplayTestingWidget()
{
}


void OffscreenDisplayTestingWidget::resizeEvent( QResizeEvent* ev )
{
    const int width = ev->size().width();
    const int height = ev->size().height();

    if( width <= 0 || height <= 0 )
    {
        return;
    }

    if( display.get() == nullptr )
    {
        display.reset( new Carna::base::OffscreenDisplay( environment, width, height, module, provider ) );

        connect( &module->renderer(), SIGNAL( invalidated() ), this, SLOT( updateRendering() ) );
    }
    else
    {
        display->resize( width, height );
    }
}


void OffscreenDisplayTestingWidget::updateRendering()
{
    display->invalidate(); //< schedules repaint

    window->setPixmap( QPixmap::fromImage( display->framebuffer() ) );
}


void OffscreenDisplayTestingWidget::mousePressEvent( QMouseEvent* ev )
{
    ev->ignore();

    module->controller().event( *module, ev );
}


void OffscreenDisplayTestingWidget::mouseMoveEvent( QMouseEvent* ev )
{
    ev->ignore();

    module->controller().event( *module, ev );
}


void OffscreenDisplayTestingWidget::mouseReleaseEvent( QMouseEvent* ev )
{
    ev->ignore();

    module->controller().event( *module, ev );
}


void OffscreenDisplayTestingWidget::wheelEvent( QWheelEvent* ev )
{
    ev->ignore();

    module->controller().event( *module, ev );
}
