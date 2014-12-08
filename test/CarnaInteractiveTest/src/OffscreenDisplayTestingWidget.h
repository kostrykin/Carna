#pragma once

#include <Carna/Carna.h>
#include <QWidget>

class QLabel;

class OffscreenDisplayTestingWidget : public QWidget
{

    Q_OBJECT

public:

    OffscreenDisplayTestingWidget
        ( Carna::base::OffscreenVisualizationEnvironment& environment
        , Carna::base::Visualization* module
        , Carna::base::view::SceneProvider& provider
        , QWidget* parent = nullptr );

    virtual ~OffscreenDisplayTestingWidget();


protected:

    virtual void resizeEvent( QResizeEvent* ) override;

    virtual void wheelEvent( QWheelEvent* ev ) override;
    
    virtual void mousePressEvent( QMouseEvent* ev ) override;
    
    virtual void mouseMoveEvent( QMouseEvent* ev ) override;
    
    virtual void mouseReleaseEvent( QMouseEvent* ev ) override;

    
private slots:

    void updateRendering();


private:

    Carna::base::OffscreenVisualizationEnvironment& environment;
    Carna::base::Visualization* const module;
    Carna::base::view::SceneProvider& provider;

    QLabel* const window;
    std::unique_ptr< Carna::base::OffscreenDisplay > display;

};
