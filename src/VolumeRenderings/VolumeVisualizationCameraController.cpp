/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/VolumeRenderings/VolumeVisualizationCameraController.h>
#include <Carna/VolumeRenderings/VolumeVisualization.h>
#include <Carna/base/CarnaException.h>
#include <QEvent>

namespace Carna
{

namespace VolumeRenderings
{



// ----------------------------------------------------------------------------------
// VolumeVisualizationCameraController :: Details
// ----------------------------------------------------------------------------------

class VolumeVisualizationCameraController :: Details
{

    VolumeVisualizationCameraController& self;

public:

    Details( VolumeVisualizationCameraController& );

    double qualityReduction;

    int originalSamplesPerPixel;


    VolumeRenderer& renderer();


    void onMousePress();

    void onMouseRelease();

}; // VolumeVisualizationCameraController :: Details


VolumeVisualizationCameraController::Details::Details( VolumeVisualizationCameraController& self )
    : self( self )
    , qualityReduction( DEFAULT_QUALITY_REDUCTION )
{
}


VolumeRenderer& VolumeVisualizationCameraController::Details::renderer()
{
    return static_cast< VolumeRenderer& >( self.module.renderer() );
}


void VolumeVisualizationCameraController::Details::onMousePress()
{
    const static unsigned int minSamplesPerPixels = 100;

    originalSamplesPerPixel = renderer().sampleCount();
    if( originalSamplesPerPixel > minSamplesPerPixels )
    {
        const unsigned int reducedSamplesPerPixels =
            std::max(
            static_cast< unsigned int >( originalSamplesPerPixel * qualityReduction + 0.5 ),
            minSamplesPerPixels );

        renderer().setSampleCount( reducedSamplesPerPixels );
    }
}


void VolumeVisualizationCameraController::Details::onMouseRelease()
{
    renderer().setSampleCount( originalSamplesPerPixel );
}



// ----------------------------------------------------------------------------------
// VolumeVisualizationCameraController
// ----------------------------------------------------------------------------------

const double VolumeVisualizationCameraController::NO_QUALITY_REDUCTION = 1.0;

const double VolumeVisualizationCameraController::DEFAULT_QUALITY_REDUCTION = 0.33;


VolumeVisualizationCameraController::VolumeVisualizationCameraController
    ( VolumeVisualization& module
    , QObject* parent )

    : DefaultCameraController( parent )
    , module( module )
    , pimpl( new Details( *this ) )
{
}


VolumeVisualizationCameraController::~VolumeVisualizationCameraController()
{
}


void VolumeVisualizationCameraController::event( base::Visualization& sourceModule, QEvent* event )
{
    CARNA_ASSERT( !event->isAccepted() );

    DefaultCameraController::event( sourceModule, event );

    if( event->isAccepted() )
    {
        switch( event->type() )
        {

            case QEvent::MouseButtonPress:
            {
                pimpl->onMousePress();
                break;
            }

            case QEvent::MouseButtonRelease:
            {
                pimpl->onMouseRelease();
                break;
            }

        }
    }
}


double VolumeVisualizationCameraController::qualityReduction() const
{
    CARNA_ASSERT( pimpl->qualityReduction >= 0.0 && pimpl->qualityReduction <= 1.0 );

    return pimpl->qualityReduction;
}


void VolumeVisualizationCameraController::setQualityReduction( double qualityReduction )
{
    CARNA_ASSERT( qualityReduction >= 0.0 && qualityReduction <= 1.0 );

    pimpl->qualityReduction = qualityReduction;
}



}  // namespace Carna :: VolumeRenderings

}  // namespace Carna
