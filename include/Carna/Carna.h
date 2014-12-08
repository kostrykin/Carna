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

#ifndef CARNA_GLOBAL_H_6014714286
#define CARNA_GLOBAL_H_6014714286

#include <QtCore/qglobal.h>

#ifdef CARNA_LIB
#   define CARNA_EXPORT Q_DECL_EXPORT
#else
#   define CARNA_EXPORT Q_DECL_IMPORT
#endif



// ----------------------------------------------------------------------------------
// Forward Declarations
// ----------------------------------------------------------------------------------

namespace Carna
{

    /** \brief  Contains the core framework components.
      */
    namespace base
    {
    
        class Application;
        class AssertionFailure;
        class CarnaException;
        class OffscreenDisplay;
        class OffscreenVisualizationEnvironment;
        class ParallelizationUI;
        class ParallelizationSettings;
        class VisualizationEnvironment;
        class Visualization;
        
        /** \brief  Contains the controller components of the core framework.
          */
        namespace controller
        {
            class DefaultCameraController;
            class DefaultInteractionStrategy;
            class InteractionStrategy;
            class MovementIntent;
            class MovementIntentProcessor;
            class NullInteractionStrategy;
            class Object3DMovementIntentProcessor;
            class VolumeSpaceMovementIntentProcessor;
        }
        
        /** \brief  Contains the model components of the core framework.
          */
        namespace model
        {
            class BufferedMaskAdapter;
            class Object3D;
            class Object3DEvent;
            class Object3DFilter;
            class Object3DSelectiveFilter;
            class Object3DUniformFilter;
            class Position;
            class RotatableObject3D;
            class Scene;
            class SceneFactory;
            class Series;
            class SeriesElement;
            class SelectSeries;
            class SeriesLoadingRequest;
            class SeriesLoadingWorker;
            class UInt16VolumeComputationWorker;
            class Volume;
            class VolumeData;
        }
        
        /** \brief  Contains the view components of the core framework.
          */
        namespace view
        {
            class Camera;
            class DefaultCamera;
            class DefaultObject3DPainter;
            class DefaultObject3DPaintingRoutine;
            class FragmentShader;
            class FramebufferObject;
            class IncrementalColorPaintingRoutine;
            class Monoscopic;
            class NullCamera;
            class Object3DPainter;
            class Object3DPaintingRoutine;
            class Point3D;
            class Polyline;
            class Polyline3DVertex;
            class RayMarching;
            class Renderer;
            class RenderTexture;
            class SceneProvider;
            class Shader;
            class ShaderBundle;
            class ShaderCompilationError;
            class ShaderProgram;
            class UInt16TextureUploader;
            class VertexShader;
        }
        
        /** \brief  Contains the Qt components of the core framework.
          *         These components are mostly user interface related.
          */
        namespace qt
        {
            class CarnaProgressDialog;
            class ColorPicker;
            class ColorPickerPainter;
            class DicomController;
            class Display;
            class DisplayVisualizationEnvironment;
            class ExpandableGroupBox;
            class MultiSpanSlider;
            class MultiSpanSliderModelViewMapping;
            class MultiSpanSliderTracker;
            class NullIntSpanPainter;
            class Object3DChooser;
            class QtDicomSeriesFileIterator;
            class QtDicomSeriesOpeningController;
			class RotatingColor;
            class RotationEdit;
            class RotationView;
            class IntSpanPainter;
            class WideColorPicker;
        }
        
    }
    
    /** \brief  Additional framework module, that provides multiplanar-reformat-alike visualization, displaying a slice plane along each axis.
      *
      * \see    There is an \ref MPRExample "example on how to utilize the multiplanar reformat visualization".
      */
    namespace MPR
    {
        class AxialPlaneVisualization;
        class AbstractSliderPainter;
        class SliderType;
        class IntersectionMovementIntentProcessor;
        class SliderPainter;
        class NullSliderPainter;
        class DefaultAxialPlaneRenderer;
        class DefaultAxialPlaneVisualization;
        class DefaultSliderPainter;
        class PartialSliderPainter;
        class SliderScreenSpaceBuffer;
        class NullSliderScreenSpaceBuffer;
        class AxialSliderScreenSpaceBuffer;
        class AxialPlaneController;
        class AxialPlaneRenderer;
        class AxialPlaneVisualization;
        class AxialPlaneControllerUI;
        class IntersectionSynchronisationFactory;
    }

    /** \brief  Additional framework module, that visualizes custom slice planes.
      */
    namespace SlicePlanes
    {
        class DefaultSlicePlaneRenderer;
        class DefaultSlicePlaneVisualization;
        class SlicePlane;
        class SlicePlaneControllerUI;
        class SlicePlaneRenderer;
        class SlicePlaneVisualization;
    }
    
    /** \brief  Additional framework module, that provides classes for stereoscopic rendering support for certain 3D displays.
      */
    namespace stereoscopic
    {
        class Philips;
        class Zalman;
    }
    
    /** \brief  Additional framework module, that visualizes CT volumes using GPU-accelerated ray marching.
      */
    namespace VolumeRenderings
    {
        class DefaultVolumeRenderer;
        class DefaultVolumeRendererModeFrame;
        class DefaultVolumeVisualization;
        class RayMarching;
        class VolumeControllerUI;
        class VolumeRenderer;
        class VolumeRendererMode;
        class VolumeRendererSinglePassMode;
        class VolumeVisualization;
        class VolumeVisualizationCameraController;
        
        /** \brief  Additional \ref VolumeRenderings sub-module, that implements the CT volume visualization as an evaluation of light absorption.
          */
        namespace DVR
        {
            class ColorMap;
            class ColorMapEditor;
            class ColorMapSpanPainter;
            class ColorMapTracker;
            class ColorMapTrackerEditor;
            class ColorMapEditor;
            class DirectVolumeRendering;
            class DirectVolumeRenderingControllerUI;
        }
        
        /** \brief  Additional \ref VolumeRenderings sub-module, that implements the CT volume visualization as a digital radiograph reconstruction.
          */
        namespace DRR
        {
            class XRay;
            class XRayControllerUI;
        }
        
        /** \brief  Additional \ref VolumeRenderings sub-module, that implements the CT volume visualization as a multi-channel maximum intensity projection.
          */
        namespace MIP
        {
            class Channel;
            class ChannelFunctionFactory;
            class MaximumIntensityProjection;
            class MaximumIntensityProjectionControllerUI;
            class ChannelFunction;
            class ChannelFunctionFactory;
            class ChannelReplaceFunction;
            class ChannelAddFunction;
            class Channel;
            class ChannelEditor;
            class ChannelUiAnchor;
        }
    }

}  // namespace Carna

#endif // CARNA_GLOBAL_H_6014714286
