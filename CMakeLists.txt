cmake_minimum_required(VERSION 2.8.7)

set( CMAKE_DEBUG_POSTFIX d )
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin )

############################################
# Library name
############################################
project( Carna )
############################################

############################################
# Version declaration
# Target file will be named with version suffix
############################################
set( MAJOR_VERSION		2 )
set( MINOR_VERSION		5 )
set( RELEASE_VERSION	0 )

set( FULL_VERSION		${MAJOR_VERSION}.${MINOR_VERSION}.${RELEASE_VERSION} )

set( TARGET_NAME		${PROJECT_NAME}-${FULL_VERSION} )

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/misc/configure.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/misc/configure.cmake @ONLY )
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/misc/install.bat.in ${CMAKE_CURRENT_BINARY_DIR}/misc/install.bat @ONLY )
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/misc/Version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/Version.h @ONLY )
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/src/doc/Doxyfile.in ${CMAKE_CURRENT_SOURCE_DIR}/src/doc/Doxyfile @ONLY )
############################################

############################################
# Options
# Set default options for this build
############################################
option(BUILD_DOC	"Build and install the API documentation"	OFF)
option(BUILD_TEST	"Build the unit tests"				OFF)
############################################

############################################
# Find dependencies and add them later to the linker (see below)
############################################

# Qt

set( QT_USE_QTMAIN		TRUE )
set( QT_USE_QTOPENGL	TRUE )
set( QT_USE_QTXML		TRUE )
find_package( Qt4 REQUIRED )
include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )

# GLEW

include( $ENV{MEDITEC_LIBS}/GLEW/1.7.0/configure.cmake )

# TRTK

include( $ENV{MEDITEC_LIBS}/TRTK/0.13.1/configure.cmake )

# dicom-interface

include( $ENV{MEDITEC_LIBS}/dicom-interface-carna/0.2.1/configure.cmake )

# Project

include_directories(${CMAKE_PROJECT_DIR}include)
set( PUBLIC_QOBJECT_HEADERS
		include/${PROJECT_NAME}/base/controller/DefaultCameraController.h
		include/${PROJECT_NAME}/base/controller/DefaultInteractionStrategy.h
		include/${PROJECT_NAME}/base/model/Scene.h
		include/${PROJECT_NAME}/base/model/SceneFactory.h
		include/${PROJECT_NAME}/base/model/Object3D.h
		include/${PROJECT_NAME}/base/qt/CarnaProgressDialog.h
		include/${PROJECT_NAME}/base/qt/ColorPicker.h
		include/${PROJECT_NAME}/base/qt/DicomController.h
		include/${PROJECT_NAME}/base/qt/Display.h
		include/${PROJECT_NAME}/base/qt/ExpandableGroupBox.h
		include/${PROJECT_NAME}/base/qt/MultiSpanSlider.h
		include/${PROJECT_NAME}/base/qt/Object3DChooser.h
		include/${PROJECT_NAME}/base/qt/RotationEdit.h
		include/${PROJECT_NAME}/base/qt/RotationView.h
		include/${PROJECT_NAME}/base/qt/WideColorPicker.h
		include/${PROJECT_NAME}/base/view/Camera.h
		include/${PROJECT_NAME}/base/view/NullCamera.h
		include/${PROJECT_NAME}/base/view/Point3D.h
		include/${PROJECT_NAME}/base/view/Polyline.h
		include/${PROJECT_NAME}/base/view/Renderer.h
		include/${PROJECT_NAME}/base/view/SceneProvider.h
		include/${PROJECT_NAME}/base/OffscreenDisplay.h
		include/${PROJECT_NAME}/base/Parallelization.h
		include/${PROJECT_NAME}/base/Visualization.h
		include/${PROJECT_NAME}/MPR/AbstractSliderPainter.h
		include/${PROJECT_NAME}/MPR/AxialPlaneController.h
		include/${PROJECT_NAME}/MPR/AxialPlaneControllerUI.h
		include/${PROJECT_NAME}/MPR/AxialPlaneRenderer.h
		include/${PROJECT_NAME}/MPR/DefaultAxialPlaneRenderer.h
		include/${PROJECT_NAME}/MPR/NullSliderPainter.h
		include/${PROJECT_NAME}/MPR/PartialSliderPainter.h
		include/${PROJECT_NAME}/MPR/SliderPainter.h
		include/${PROJECT_NAME}/SlicePlanes/DefaultSlicePlaneRenderer.h
		include/${PROJECT_NAME}/SlicePlanes/SlicePlaneControllerUI.h
		include/${PROJECT_NAME}/SlicePlanes/SlicePlaneRenderer.h
		include/${PROJECT_NAME}/VolumeRenderings/DVR/DirectVolumeRendering.h
		include/${PROJECT_NAME}/VolumeRenderings/DVR/ColorMap.h
		include/${PROJECT_NAME}/VolumeRenderings/DVR/ColorMapEditor.h
		include/${PROJECT_NAME}/VolumeRenderings/DVR/ColorMapTrackerEditor.h
		include/${PROJECT_NAME}/VolumeRenderings/DRR/XRay.h
		include/${PROJECT_NAME}/VolumeRenderings/MIP/Channel.h
		include/${PROJECT_NAME}/VolumeRenderings/MIP/ChannelEditor.h
		include/${PROJECT_NAME}/VolumeRenderings/MIP/ChannelUiAnchor.h
		include/${PROJECT_NAME}/VolumeRenderings/MIP/MaximumIntensityProjection.h
		include/${PROJECT_NAME}/VolumeRenderings/DefaultVolumeRenderer.h
		include/${PROJECT_NAME}/VolumeRenderings/VolumeRenderer.h
		include/${PROJECT_NAME}/VolumeRenderings/VolumeRendererMode.h
		include/${PROJECT_NAME}/VolumeRenderings/VolumeVisualizationCameraController.h
)
set( PUBLIC_HEADERS
		${PUBLIC_QOBJECT_HEADERS}
		include/${PROJECT_NAME}/base/controller/InteractionStrategy.h
		include/${PROJECT_NAME}/base/controller/MovementIntent.h
		include/${PROJECT_NAME}/base/controller/MovementIntentProcessor.h
		include/${PROJECT_NAME}/base/controller/NullInteractionStrategy.h
		include/${PROJECT_NAME}/base/controller/Object3DMovementIntentProcessor.h
		include/${PROJECT_NAME}/base/controller/VolumeSpaceMovementIntentProcessor.h
		include/${PROJECT_NAME}/base/model/BufferedMask.h
		include/${PROJECT_NAME}/base/model/BufferedMaskAdapter.h
		include/${PROJECT_NAME}/base/model/GenericVolume.h
		include/${PROJECT_NAME}/base/model/Histogram.h
		include/${PROJECT_NAME}/base/model/HistogramClassDetector.h
		include/${PROJECT_NAME}/base/model/Object3DEvent.h
		include/${PROJECT_NAME}/base/model/Object3DFilter.h
		include/${PROJECT_NAME}/base/model/Object3DSelectiveFilter.h
		include/${PROJECT_NAME}/base/model/Object3DUniformFilter.h
		include/${PROJECT_NAME}/base/model/Position.h
		include/${PROJECT_NAME}/base/model/RotatableObject3D.h
		include/${PROJECT_NAME}/base/model/ScalarField.h
		include/${PROJECT_NAME}/base/model/Series.h
		include/${PROJECT_NAME}/base/model/SeriesElement.h
		include/${PROJECT_NAME}/base/model/SeriesLoadingRequest.h
		include/${PROJECT_NAME}/base/model/UInt16Volume.h
		include/${PROJECT_NAME}/base/model/Volume.h
		include/${PROJECT_NAME}/base/qt/ColorPickerPainter.h
		include/${PROJECT_NAME}/base/qt/DisplayVisualizationEnvironment.h
		include/${PROJECT_NAME}/base/qt/IntSpanPainter.h
		include/${PROJECT_NAME}/base/qt/MultiSpanSliderModelViewMapping.h
		include/${PROJECT_NAME}/base/qt/MultiSpanSliderTracker.h
		include/${PROJECT_NAME}/base/qt/NullIntSpanPainter.h
		include/${PROJECT_NAME}/base/qt/RotatingColor.h
		include/${PROJECT_NAME}/base/view/DefaultCamera.h
		include/${PROJECT_NAME}/base/view/DefaultObject3DPainter.h
		include/${PROJECT_NAME}/base/view/DefaultObject3DPaintingRoutine.h
		include/${PROJECT_NAME}/base/view/FragmentShader.h
		include/${PROJECT_NAME}/base/view/FramebufferObject.h
		include/${PROJECT_NAME}/base/view/glError.h
		include/${PROJECT_NAME}/base/view/IncrementalColorPaintingRoutine.h
		include/${PROJECT_NAME}/base/view/Monoscopic.h
		include/${PROJECT_NAME}/base/view/Object3DPainter.h
		include/${PROJECT_NAME}/base/view/Object3DPaintingRoutine.h
		include/${PROJECT_NAME}/base/view/Polyline3DVertex.h
		include/${PROJECT_NAME}/base/view/RenderTexture.h
		include/${PROJECT_NAME}/base/view/Shader.h
		include/${PROJECT_NAME}/base/view/ShaderBundle.h
		include/${PROJECT_NAME}/base/view/ShaderCompilationError.h
		include/${PROJECT_NAME}/base/view/ShaderProgram.h
		include/${PROJECT_NAME}/base/view/UInt16TextureUploader.h
		include/${PROJECT_NAME}/base/view/VertexShader.h
		include/${PROJECT_NAME}/base/Aggregation.h
		include/${PROJECT_NAME}/base/Application.h
		include/${PROJECT_NAME}/base/Association.h
		include/${PROJECT_NAME}/base/CarnaException.h
		include/${PROJECT_NAME}/base/Composition.h
		include/${PROJECT_NAME}/base/Flags.h
		include/${PROJECT_NAME}/base/IntSpan.h
		include/${PROJECT_NAME}/base/noncopyable.h
		include/${PROJECT_NAME}/base/Math.h
		include/${PROJECT_NAME}/base/Memory.h
		include/${PROJECT_NAME}/base/OffscreenVisualizationEnvironment.h
		include/${PROJECT_NAME}/base/ParallelizationSettings.h
		include/${PROJECT_NAME}/base/Singleton.h
		include/${PROJECT_NAME}/base/Span.h
		include/${PROJECT_NAME}/base/Text.h
		include/${PROJECT_NAME}/base/Transformation.h
		include/${PROJECT_NAME}/base/Vector3.h
		include/${PROJECT_NAME}/base/VisualizationEnvironment.h
		include/${PROJECT_NAME}/MPR/AxialPlaneVisualization.h
		include/${PROJECT_NAME}/MPR/AxialSliderScreenSpaceBuffer.h
		include/${PROJECT_NAME}/MPR/DefaultAxialPlaneVisualization.h
		include/${PROJECT_NAME}/MPR/DefaultSliderPainter.h
		include/${PROJECT_NAME}/MPR/IntersectionMovementIntentProcessor.h
		include/${PROJECT_NAME}/MPR/IntersectionSynchronisationFactory.h
		include/${PROJECT_NAME}/MPR/NullSliderScreenSpaceBuffer.h
		include/${PROJECT_NAME}/MPR/SliderScreenSpaceBuffer.h
		include/${PROJECT_NAME}/MPR/SliderType.h
		include/${PROJECT_NAME}/SlicePlanes/DefaultSlicePlaneVisualization.h
		include/${PROJECT_NAME}/SlicePlanes/SlicePlane.h
		include/${PROJECT_NAME}/SlicePlanes/SlicePlaneVisualization.h
		include/${PROJECT_NAME}/VolumeRenderings/DVR/ColorMapSpanPainter.h
		include/${PROJECT_NAME}/VolumeRenderings/DVR/ColorMapTracker.h
		include/${PROJECT_NAME}/VolumeRenderings/MIP/ChannelAddFunction.h
		include/${PROJECT_NAME}/VolumeRenderings/MIP/ChannelFunction.h
		include/${PROJECT_NAME}/VolumeRenderings/MIP/ChannelFunctionFactory.h
		include/${PROJECT_NAME}/VolumeRenderings/MIP/ChannelReplaceFunction.h
		include/${PROJECT_NAME}/VolumeRenderings/MIP/GenericChannelFunction.h
		include/${PROJECT_NAME}/VolumeRenderings/DefaultVolumeVisualization.h
		include/${PROJECT_NAME}/VolumeRenderings/RayMarching.h
		include/${PROJECT_NAME}/VolumeRenderings/VolumeControllerUI.h
		include/${PROJECT_NAME}/VolumeRenderings/VolumeRendererSinglePassMode.h
		include/${PROJECT_NAME}/VolumeRenderings/VolumeVisualization.h
		include/${PROJECT_NAME}/stereoscopic/Philips.h
		include/${PROJECT_NAME}/stereoscopic/Zalman.h
		include/${PROJECT_NAME}/Carna.h
		include/${PROJECT_NAME}/Version.h
	)
include_directories(${CMAKE_PROJECT_DIR}src/include)
include_directories(${CMAKE_PROJECT_DIR}src/include/Carna/base/qt)	# required for /src/include/Carna/base/flowlayout.h
set( PRIVATE_QOBJECT_HEADERS
		src/include/Carna/base/controller/DefaultInteractionStrategyDetails.h
		src/include/Carna/base/model/DicomManager.h
		src/include/Carna/base/model/SceneDetails.h
		src/include/Carna/base/model/SeriesLoadingWorker.h
		src/include/Carna/base/model/VolumeDetails.h
		src/include/Carna/base/qt/DicomOpeningDialog.h
		src/include/Carna/base/qt/DisplayDetails.h
		src/include/Carna/base/qt/Object3DChooserDetails.h
		src/include/Carna/base/qt/QtDicomSeriesOpeningController.h
		src/include/Carna/base/qt/SeriesPreview.h
		src/include/Carna/base/qt/SeriesView.h
		src/include/Carna/base/qt/ToggleSeriesPreview.h
		src/include/Carna/base/view/RendererDetails.h
		src/include/Carna/base/view/SceneProviderDetails.h
		src/include/Carna/MPR/AxialPlaneControllerDetails.h
		src/include/Carna/MPR/AxialPlaneControllerUIDetails.h
		src/include/Carna/VolumeRenderings/DVR/DirectVolumeRenderingControllerUI.h
		src/include/Carna/VolumeRenderings/DVR/DirectVolumeRenderingDetails.h
		src/include/Carna/VolumeRenderings/DRR/XRayControllerUI.h
		src/include/Carna/VolumeRenderings/MIP/MaximumIntensityProjectionControllerUI.h
		src/include/Carna/VolumeRenderings/DefaultVolumeRendererDetails.h
		src/include/Carna/VolumeRenderings/VolumeControllerUIDetails.h
		src/include/Carna/SlicePlanes/DefaultSlicePlaneRendererDetails.h
		src/include/Carna/SlicePlanes/SlicePlaneControllerUIDetails.h
)
set( PRIVATE_HEADERS
		${PRIVATE_QOBJECT_HEADERS}
		src/include/Carna/base/model/BufferedMaskAdapterDetails.h
		src/include/Carna/base/model/Object3DDetails.h
		src/include/Carna/base/model/Patient.h
		src/include/Carna/base/model/RotatableObject3DDetails.h
		src/include/Carna/base/model/Study.h
		src/include/Carna/base/qt/ExpandableGroupBoxDetails.h
		src/include/Carna/base/qt/FlowLayout.h
		src/include/Carna/base/view/DefaultCameraDetails.h
		src/include/Carna/base/view/gl.h
		src/include/Carna/base/view/glew.h
		src/include/Carna/base/view/glu.h
		src/include/Carna/base/view/Point3DDetails.h
		src/include/Carna/base/view/PolylineDetails.h
		src/include/Carna/base/view/RendererSharedData.h
		src/include/Carna/base/VisualizationDetails.h
		src/include/Carna/VolumeRenderings/DVR/DirectVolumeRenderingController.h
		src/include/Carna/VolumeRenderings/DVR/ColorMapDetails.h
		src/include/Carna/VolumeRenderings/DRR/XRayDetails.h
		src/include/Carna/VolumeRenderings/MIP/MaximumIntensityProjectionDetails.h
		src/include/Carna/VolumeRenderings/DefaultVolumeRendererModeFrame.h
		src/include/Carna/MPR/DefaultAxialPlaneRendererDetails.h
		src/include/Carna/SlicePlanes/SlicePlaneDetails.h
	)
set( SRC
		src/base/controller/DefaultCameraController.cpp
		src/base/controller/DefaultInteractionStrategy.cpp
		src/base/controller/MovementIntent.cpp
		src/base/controller/Object3DMovementIntentProcessor.cpp
		src/base/controller/VolumeSpaceMovementIntentProcessor.cpp
		src/base/model/BufferedMaskAdapter.cpp
		src/base/model/DicomManager.cpp
		src/base/model/Scene.cpp
		src/base/model/SceneFactory.cpp
		src/base/model/Object3D.cpp
		src/base/model/Object3DEvent.cpp
		src/base/model/Patient.cpp
		src/base/model/Position.cpp
		src/base/model/RotatableObject3D.cpp
		src/base/model/Series.cpp
		src/base/model/SeriesElement.cpp
		src/base/model/SeriesLoadingWorker.cpp
		src/base/model/Study.cpp
		src/base/model/Volume.cpp
		src/base/model/VolumeDetails.cpp
		src/base/qt/CarnaProgressDialog.cpp
		src/base/qt/ColorPicker.cpp
		src/base/qt/ColorPickerPainter.cpp
		src/base/qt/DicomController.cpp
		src/base/qt/DicomOpeningDialog.cpp
		src/base/qt/Display.cpp
		src/base/qt/DisplayDetails.cpp
		src/base/qt/DisplayVisualizationEnvironment.cpp
		src/base/qt/ExpandableGroupBox.cpp
		src/base/qt/FlowLayout.cpp
		src/base/qt/MultiSpanSlider.cpp
		src/base/qt/MultiSpanSliderTracker.cpp
		src/base/qt/Object3DChooser.cpp
		src/base/qt/QtDicomSeriesOpeningController.cpp
		src/base/qt/RotatingColor.cpp
		src/base/qt/RotationEdit.cpp
		src/base/qt/RotationView.cpp
		src/base/qt/SeriesPreview.cpp
		src/base/qt/SeriesView.cpp
		src/base/qt/ToggleSeriesPreview.cpp
		src/base/qt/WideColorPicker.cpp
		src/base/view/DefaultCamera.cpp
		src/base/view/DefaultObject3DPainter.cpp
		src/base/view/DefaultObject3DPaintingRoutine.cpp
		src/base/view/FragmentShader.cpp
		src/base/view/FramebufferObject.cpp
		src/base/view/IncrementalColorPaintingRoutine.cpp
		src/base/view/Monoscopic.cpp
		src/base/view/Point3D.cpp
		src/base/view/Polyline.cpp
		src/base/view/Polyline3DVertex.cpp
		src/base/view/Renderer.cpp
		src/base/view/RendererDetails.cpp
		src/base/view/RendererSharedData.cpp
		src/base/view/RenderTexture.cpp
		src/base/view/SceneProvider.cpp
		src/base/view/SceneProviderDetails.cpp
		src/base/view/Shader.cpp
		src/base/view/ShaderBundle.cpp
		src/base/view/ShaderCompilationError.cpp
		src/base/view/ShaderProgram.cpp
		src/base/view/UInt16TextureUploader.cpp
		src/base/view/VertexShader.cpp
		src/base/Application.cpp
		src/base/CarnaException.cpp
		src/base/Memory.cpp
		src/base/OffscreenDisplay.cpp
		src/base/OffscreenVisualizationEnvironment.cpp
		src/base/Parallelization.cpp
		src/base/ParallelizationSettings.cpp
		src/base/Text.cpp
		src/base/Transformation.cpp
		src/base/Visualization.cpp
		src/base/VisualizationEnvironment.cpp
		src/MPR/AbstractSliderPainter.cpp
		src/MPR/AxialPlaneController.cpp
		src/MPR/AxialPlaneControllerUI.cpp
		src/MPR/AxialPlaneRenderer.cpp
		src/MPR/AxialPlaneVisualization.cpp
		src/MPR/AxialSliderScreenSpaceBuffer.cpp
		src/MPR/DefaultAxialPlaneRenderer.cpp
		src/MPR/DefaultAxialPlaneRendererDetails.cpp
		src/MPR/DefaultAxialPlaneVisualization.cpp
		src/MPR/DefaultSliderPainter.cpp
		src/MPR/IntersectionMovementIntentProcessor.cpp
		src/MPR/PartialSliderPainter.cpp
		src/MPR/SliderType.cpp
		src/SlicePlanes/DefaultSlicePlaneRenderer.cpp
		src/SlicePlanes/DefaultSlicePlaneVisualization.cpp
		src/SlicePlanes/SlicePlane.cpp
		src/SlicePlanes/SlicePlaneControllerUI.cpp
		src/SlicePlanes/SlicePlaneDetails.cpp
		src/SlicePlanes/SlicePlaneRenderer.cpp
		src/SlicePlanes/SlicePlaneVisualization.cpp
		src/VolumeRenderings/DVR/DirectVolumeRendering.cpp
		src/VolumeRenderings/DVR/DirectVolumeRenderingController.cpp
		src/VolumeRenderings/DVR/DirectVolumeRenderingControllerUI.cpp
		src/VolumeRenderings/DVR/ColorMap.cpp
		src/VolumeRenderings/DVR/ColorMapEditor.cpp
		src/VolumeRenderings/DVR/ColorMapSpanPainter.cpp
		src/VolumeRenderings/DVR/ColorMapTracker.cpp
		src/VolumeRenderings/DVR/ColorMapTrackerEditor.cpp
		src/VolumeRenderings/DRR/XRay.cpp
		src/VolumeRenderings/DRR/XRayControllerUI.cpp
		src/VolumeRenderings/MIP/Channel.cpp
		src/VolumeRenderings/MIP/ChannelAddFunction.cpp
		src/VolumeRenderings/MIP/ChannelEditor.cpp
		src/VolumeRenderings/MIP/ChannelFunction.cpp
		src/VolumeRenderings/MIP/ChannelReplaceFunction.cpp
		src/VolumeRenderings/MIP/ChannelUiAnchor.cpp
		src/VolumeRenderings/MIP/MaximumIntensityProjection.cpp
		src/VolumeRenderings/MIP/MaximumIntensityProjectionControllerUI.cpp
		src/VolumeRenderings/MIP/MaximumIntensityProjectionDetails.cpp
		src/VolumeRenderings/DefaultVolumeRenderer.cpp
		src/VolumeRenderings/DefaultVolumeRendererDetails.cpp
		src/VolumeRenderings/DefaultVolumeRendererModeFrame.cpp
		src/VolumeRenderings/DefaultVolumeVisualization.cpp
		src/VolumeRenderings/VolumeControllerUI.cpp
		src/VolumeRenderings/VolumeControllerUIDetails.cpp
		src/VolumeRenderings/VolumeRenderer.cpp
		src/VolumeRenderings/VolumeRendererMode.cpp
		src/VolumeRenderings/VolumeRendererSinglePassMode.cpp
		src/VolumeRenderings/VolumeVisualization.cpp
		src/VolumeRenderings/VolumeVisualizationCameraController.cpp
		src/stereoscopic/Philips.cpp
		src/stereoscopic/Zalman.cpp
	)
set( FORMS
		""
	)
set( RESOURCES
		src/shaders.qrc
	)
set( DOC_SRC
		src/doc/Doxyfile.in
		src/doc/doc_00style.css
		src/doc/doc_common.dox
		src/doc/doc_coordinate_systems.dox
		src/doc/doc_creating_views.dox
		src/doc/doc_displaying_custom_3d.dox
		src/doc/doc_from_1.x_to_2.x.dox
		src/doc/doc_main.dox
		src/doc/doc_quick_start.dox
		src/doc/doc_version_log.dox
	)
set( SHADERS_SRC
		src/res/absorption.frag
		src/res/absorption.vert
		src/res/absorption-depth.frag
		src/res/absorption-depth.vert
		src/res/mip.frag
		src/res/mip.vert
		src/res/mip-colorizer.frag
		src/res/mip-colorizer.vert
		src/res/philips.frag
		src/res/philips.vert
		src/res/sliceplane-view.frag
		src/res/sliceplane-view.vert
		src/res/sliceplane-windowing.frag
		src/res/sliceplane-windowing.vert
		src/res/volume-postpass.frag
		src/res/volume-postpass.vert
		src/res/volume-prepass.frag
		src/res/volume-prepass.vert
		src/res/xray.frag
		src/res/xray.vert
		src/res/zalman.frag
		src/res/zalman.vert
	)
	
QT4_WRAP_CPP( PUBLIC_HEADERS_MOC ${PUBLIC_QOBJECT_HEADERS} )
QT4_WRAP_CPP( PRIVATE_HEADERS_MOC ${PRIVATE_QOBJECT_HEADERS} )
QT4_WRAP_UI( FORMS_HEADERS ${FORMS} )
QT4_ADD_RESOURCES( RESOURCES_RCC ${RESOURCES} )
	
############################################
# Setup Visual Studio filters
############################################

SOURCE_GROUP( "Headers"
	REGULAR_EXPRESSION .*include/Carna/[^/]*\\.h )

SOURCE_GROUP( "Headers\\base"
	REGULAR_EXPRESSION .*include/Carna/base/[^/]*\\.h )

SOURCE_GROUP( "Headers\\base\\controller"
	REGULAR_EXPRESSION .*include/Carna/base/controller/[^/]*\\.h )

SOURCE_GROUP( "Headers\\base\\model"
	REGULAR_EXPRESSION .*include/Carna/base/model/[^/]*\\.h )

SOURCE_GROUP( "Headers\\base\\qt"
	REGULAR_EXPRESSION .*include/Carna/base/qt/[^/]*\\.h )

SOURCE_GROUP( "Headers\\base\\view"
	REGULAR_EXPRESSION .*include/Carna/base/view/[^/]*\\.h )

SOURCE_GROUP( "Headers\\MPR"
	REGULAR_EXPRESSION .*include/Carna/MPR/[^/]*\\.h )

SOURCE_GROUP( "Headers\\SlicePlanes"
	REGULAR_EXPRESSION .*include/Carna/SlicePlanes/[^/]*\\.h )

SOURCE_GROUP( "Headers\\stereoscopic"
	REGULAR_EXPRESSION .*include/Carna/stereoscopic/[^/]*\\.h )

SOURCE_GROUP( "Headers\\VolumeRenderings"
	REGULAR_EXPRESSION .*include/Carna/VolumeRenderings/[^/]*\\.h )

SOURCE_GROUP( "Headers\\VolumeRenderings\\DVR"
	REGULAR_EXPRESSION .*include/Carna/VolumeRenderings/DVR/[^/]*\\.h )

SOURCE_GROUP( "Headers\\VolumeRenderings\\DRR"
	REGULAR_EXPRESSION .*include/Carna/VolumeRenderings/DRR/[^/]*\\.h )

SOURCE_GROUP( "Headers\\VolumeRenderings\\MIP"
	REGULAR_EXPRESSION .*include/Carna/VolumeRenderings/MIP/[^/]*\\.h )

SOURCE_GROUP( "Sources"
	REGULAR_EXPRESSION src/[^/]*\\.cpp )

SOURCE_GROUP( "Sources\\base"
	REGULAR_EXPRESSION src/base/[^/]*\\.cpp )

SOURCE_GROUP( "Sources\\base\\controller"
	REGULAR_EXPRESSION src/base/controller/[^/]*\\.cpp )

SOURCE_GROUP( "Sources\\base\\model"
	REGULAR_EXPRESSION src/base/model/[^/]*\\.cpp )

SOURCE_GROUP( "Sources\\base\\qt"
	REGULAR_EXPRESSION src/base/qt/[^/]*\\.cpp )

SOURCE_GROUP( "Sources\\base\\view"
	REGULAR_EXPRESSION src/base/view/[^/]*\\.cpp )

SOURCE_GROUP( "Sources\\MPR"
	REGULAR_EXPRESSION src/MPR/[^/]*\\.cpp )

SOURCE_GROUP( "Sources\\SlicePlanes"
	REGULAR_EXPRESSION src/SlicePlanes/[^/]*\\.cpp )

SOURCE_GROUP( "Sources\\stereoscopic"
	REGULAR_EXPRESSION src/stereoscopic/[^/]*\\.cpp )

SOURCE_GROUP( "Sources\\VolumeRenderings"
	REGULAR_EXPRESSION src/VolumeRenderings/[^/]*\\.cpp )

SOURCE_GROUP( "Sources\\VolumeRenderings\\DVR"
	REGULAR_EXPRESSION src/VolumeRenderings/DVR/[^/]*\\.cpp )

SOURCE_GROUP( "Sources\\VolumeRenderings\\DRR"
	REGULAR_EXPRESSION src/VolumeRenderings/DRR/[^/]*\\.cpp )

SOURCE_GROUP( "Sources\\VolumeRenderings\\MIP"
	REGULAR_EXPRESSION src/VolumeRenderings/MIP/[^/]*\\.cpp )

SOURCE_GROUP( "Miscellaneous"
	REGULAR_EXPRESSION .*\\.\(\(in\)|\(txt\)|\(qrc\)\) )

SOURCE_GROUP( "Miscellaneous\\Documentation"
	REGULAR_EXPRESSION .*\\.\(\(css\)|\(dox\)\) )

SOURCE_GROUP( "Miscellaneous\\Shaders"
	REGULAR_EXPRESSION .*\\.\(\(frag\)|\(vert\)\) )

SOURCE_GROUP( "Miscellaneous\\Generated Files"
	REGULAR_EXPRESSION build/.*\\.\(\(cxx\)|\(cpp\)\) )
	
############################################
# Exclude all source files from build,
# since we are using unity build.
############################################
set_source_files_properties(	${SRC}
								${PUBLIC_HEADERS_MOC}
								${PRIVATE_HEADERS_MOC}
								${FORMS_HEADERS}
								${RESOURCES_RCC}
								${DOC_SRC}
								${SHADERS_SRC}
	PROPERTIES
	HEADER_FILE_ONLY TRUE )
############################################
	
############################################
# Write unity build file
############################################
set( UNITY_BUILD_FILE	${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}-unitybuild.cpp )

file( REMOVE	${UNITY_BUILD_FILE} )
file( WRITE		${UNITY_BUILD_FILE} "// This file is automatically generated by CMake.\n\n" )
file( APPEND	${UNITY_BUILD_FILE}	"#include <Carna/base/view/glew.h>\n" )

foreach( SOURCE_FILE ${SRC} )
	file( APPEND	${UNITY_BUILD_FILE}	"#include \"${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}\"\n" )
endforeach( SOURCE_FILE )

foreach( QT_GENERATED_SOURCE_FILE ${PUBLIC_HEADERS_MOC} ${PRIVATE_HEADERS_MOC} ${FORMS_HEADERS} ${RESOURCES_RCC} )
	file( APPEND	${UNITY_BUILD_FILE}	"#include \"${QT_GENERATED_SOURCE_FILE}\"\n" )
endforeach( QT_GENERATED_SOURCE_FILE )
############################################
	
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

add_library( ${TARGET_NAME} SHARED
			${SRC}
			${UNITY_BUILD_FILE}
			${PUBLIC_HEADERS}
			${PRIVATE_HEADERS}
			${PUBLIC_HEADERS_MOC}
			${PRIVATE_HEADERS_MOC}
			${FORMS_HEADERS}
			${RESOURCES_RCC}
			${DOC_SRC}
			${SHADERS_SRC}
		)

string( TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER )
add_definitions( -D${PROJECT_NAME_UPPER}_LIB -DNOMINMAX -D_NO_FRAMEBUFFER_DEBUG )

add_subdirectory( test/CarnaInteractiveTest )


############################################
# Add dependencies to the linker
############################################

target_link_libraries( ${TARGET_NAME}
			opengl32
			glu32
			${GLEW_LIBLIST}
			${QT_LIBRARIES}
			${TRTK_LIBLIST}
			${DICOM_INTERFACE_LIBLIST}
		)






#vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
##vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
### NO CHANGES EVER NECESSARY BELOW THIS POINT!!
##vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
#vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

string( REGEX REPLACE "\\\\" "/" INSTALL_DIR $ENV{MEDITEC_LIBS}/${PROJECT_NAME}/${FULL_VERSION} )

install(	TARGETS	${TARGET_NAME}
		RUNTIME	DESTINATION	$ENV{MEDITEC_LIBS}/bin
		)
install(	TARGETS	${TARGET_NAME}
		RUNTIME	DESTINATION	${INSTALL_DIR}/bin
		ARCHIVE	DESTINATION	${INSTALL_DIR}/bin
		LIBRARY	DESTINATION	${INSTALL_DIR}/bin
		)
install(	FILES	${CMAKE_CURRENT_BINARY_DIR}/misc/configure.cmake ${CMAKE_CURRENT_BINARY_DIR}/misc/install.bat
		DESTINATION 	${INSTALL_DIR}
		)
install(	DIRECTORY	${CMAKE_PROJECT_DIR}include
		DESTINATION 	${INSTALL_DIR}
		)
if (WIN32)
	install(	FILES	${CMAKE_CURRENT_BINARY_DIR}/bin/${TARGET_NAME}${CMAKE_DEBUG_POSTFIX}.pdb
			DESTINATION		$ENV{MEDITEC_LIBS}/bin
			OPTIONAL
			)
	install(	FILES	${CMAKE_CURRENT_BINARY_DIR}/bin/${TARGET_NAME}${CMAKE_DEBUG_POSTFIX}.pdb
			DESTINATION		${INSTALL_DIR}/bin
			OPTIONAL
			)
endif()


############################################
# Unit tests
############################################
if( BUILD_TEST )
	add_subdirectory( test )
endif()
############################################

############################################
# Doxygen API documentation
############################################
if( BUILD_DOC )
	find_package( Doxygen REQUIRED )
	if( DOXYGEN_FOUND )
		add_custom_target(	doc ALL
						${DOXYGEN_EXECUTABLE} 
						WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/doc
						COMMENT "Generating API documentation with Doxygen" VERBATIM
					)
		install(	DIRECTORY 	${CMAKE_CURRENT_SOURCE_DIR}/doc
				DESTINATION		${INSTALL_DIR}
			)
	endif( DOXYGEN_FOUND )
endif()
############################################
