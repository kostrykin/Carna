cmake_minimum_required(VERSION 3.0.2)
project(Carna)

set(MAJOR_VERSION	3)
set(MINOR_VERSION	0)
set(PATCH_VERSION	0)

############################################

set(CMAKE_DEBUG_POSTFIX d)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(FULL_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
set(TARGET_NAME ${PROJECT_NAME}-${FULL_VERSION})
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_CAPS)

############################################
# Set default options for this build
############################################

option(BUILD_DOC	"Build and install the API documentation" OFF)
option(BUILD_TEST	"Build the unit tests" OFF)
option(BUILD_DEMO	"Build interactive demo" OFF)

############################################
# Macro that sets variable to default value
# only when the variable isn't defined yet
############################################

macro( option_default_to var_name default_val var_type doc_string )
	if( NOT DEFINED ${var_name} )
		set(${var_name} ${default_val})
	endif()
	set(${var_name} ${${var_name}} CACHE ${var_type} ${doc_string} FORCE)
endmacro()

############################################
# Locate Find<ModuleName>.cmake scripts
############################################

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/misc/CMake-Modules)

############################################
# Run MediTEC compatiblity script
############################################

add_subdirectory(misc/MediTEC)

############################################
# Define default paths for the installation
############################################

# set default library and header destinations (relative to CMAKE_INSTALL_PREFIX)
option_default_to(INSTALL_LIBRARY_DIR "lib" String "Installation directory for libraries")
option_default_to(INSTALL_HEADERS_DIR "include" String "Installation directory for headers")

# set default destination for CMake modules
if( WIN32 )
	option_default_to(INSTALL_CMAKE_DIR "share/cmake-3.0/Modules" String "Installation directory for CMake files")
else( WIN32 )
	option_default_to(INSTALL_CMAKE_DIR "share/cmake/Modules" String "Installation directory for CMake files")
endif( WIN32 )

############################################
# Normalize installation paths
# (get rid of Windows-style delimiters)
############################################

file(	TO_CMAKE_PATH	${INSTALL_LIBRARY_DIR}	INSTALL_LIBRARY_DIR )
file(	TO_CMAKE_PATH	${INSTALL_HEADERS_DIR}	INSTALL_HEADERS_DIR )
file(	TO_CMAKE_PATH	${INSTALL_CMAKE_DIR}	INSTALL_CMAKE_DIR )

############################################

configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/misc/Version.h.in
				${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/Version.h @ONLY)

configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/src/doc/Doxyfile.in
				${CMAKE_CURRENT_SOURCE_DIR}/src/doc/Doxyfile @ONLY)

configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/misc/Find${PROJECT_NAME}.cmake.in
				${CMAKE_CURRENT_BINARY_DIR}/misc/Find${PROJECT_NAME}.cmake @ONLY)

############################################
# Find required dependencies
############################################

# Qt
find_package( Qt4 4.8.0 COMPONENTS QtCore QtGui QtOpenGL QtXml REQUIRED )
include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )

# GLEW
find_package( GLEW 1.7.0 REQUIRED )
include_directories( ${GLEW_INCLUDE_DIRS} )

# Eigen
find_package( Eigen3 REQUIRED )
include_directories( ${EIGEN3_INCLUDE_DIR} )

############################################
# Project
############################################

include_directories(${CMAKE_PROJECT_DIR}include)
include_directories(${CMAKE_PROJECT_DIR}include/Carna/base/qt)	# required by FlowLayout.h
set( PUBLIC_QOBJECT_HEADERS
		""
)
set( PUBLIC_HEADERS
		${PUBLIC_QOBJECT_HEADERS}
		include/${PROJECT_NAME}/base/view/Camera.h
		include/${PROJECT_NAME}/base/view/Color.h
		include/${PROJECT_NAME}/base/view/RenderQueue.h
		include/${PROJECT_NAME}/base/view/Spatial.h
		include/${PROJECT_NAME}/base/view/Node.h
		include/${PROJECT_NAME}/base/view/Geometry.h
		include/${PROJECT_NAME}/base/view/Renderable.h
		include/${PROJECT_NAME}/base/view/FrameRenderer.h
		include/${PROJECT_NAME}/base/view/RenderTask.h
		include/${PROJECT_NAME}/base/view/RenderStage.h
		include/${PROJECT_NAME}/base/view/GeometryStage.h
		include/${PROJECT_NAME}/base/view/GLContext.h
		include/${PROJECT_NAME}/base/view/OpaqueStage.h
		include/${PROJECT_NAME}/base/view/FragmentShader.h
		include/${PROJECT_NAME}/base/view/Framebuffer.h
		include/${PROJECT_NAME}/base/view/RenderTexture.h
		include/${PROJECT_NAME}/base/view/Shader.h
		include/${PROJECT_NAME}/base/view/ShaderBundle.h
		include/${PROJECT_NAME}/base/view/ShaderCompilationError.h
		include/${PROJECT_NAME}/base/view/ShaderProgram.h
		include/${PROJECT_NAME}/base/view/VertexShader.h
		include/${PROJECT_NAME}/base/view/Viewport.h
		include/${PROJECT_NAME}/base/view/glError.h
		include/${PROJECT_NAME}/base/view/Texture3D.h
		include/${PROJECT_NAME}/base/view/BufferedHUVolumeUploader.h
		include/${PROJECT_NAME}/base/view/BufferedHUVolumeManager.h
		include/${PROJECT_NAME}/base/view/GeometryDefinition.h
		include/${PROJECT_NAME}/base/view/VideoResourcesManager.h
		include/${PROJECT_NAME}/base/view/RayMarchingStage.h
		include/${PROJECT_NAME}/base/model/ScalarField.h
		include/${PROJECT_NAME}/base/model/HUCubeField.h
		include/${PROJECT_NAME}/base/model/HUCubeSegment.h
		include/${PROJECT_NAME}/base/model/HUVolume.h
		include/${PROJECT_NAME}/base/model/BufferedHUVolume.h
		include/${PROJECT_NAME}/base/model/UInt16HUVolume.h
		include/${PROJECT_NAME}/base/Aggregation.h
		include/${PROJECT_NAME}/base/Application.h
		include/${PROJECT_NAME}/base/Association.h
		include/${PROJECT_NAME}/base/CarnaException.h
		include/${PROJECT_NAME}/base/Composition.h
		include/${PROJECT_NAME}/base/noncopyable.h
		include/${PROJECT_NAME}/base/Matrix4f.h
		include/${PROJECT_NAME}/base/Vector3ui.h
		include/${PROJECT_NAME}/base/Singleton.h
		include/${PROJECT_NAME}/base/Text.h
		include/${PROJECT_NAME}/base/Log.h
		include/${PROJECT_NAME}/Carna.h
		include/${PROJECT_NAME}/Version.h
	)
include_directories(${CMAKE_PROJECT_DIR}src/include)
set( PRIVATE_QOBJECT_HEADERS
		""
)
set( PRIVATE_HEADERS
		${PRIVATE_QOBJECT_HEADERS}
		src/include/Carna/base/view/gl.h
		src/include/Carna/base/view/glew.h
		src/include/Carna/base/view/glu.h
	)
set( SRC
		src/base/view/Camera.cpp
		src/base/view/Color.cpp
		src/base/view/Geometry.cpp
		src/base/view/GLContext.cpp
		src/base/view/Renderable.cpp
		src/base/view/FrameRenderer.cpp
		src/base/view/RenderTask.cpp
		src/base/view/RenderStage.cpp
		src/base/view/OpaqueStage.cpp
		src/base/view/Spatial.cpp
		src/base/view/Node.cpp
		src/base/view/FragmentShader.cpp
		src/base/view/Framebuffer.cpp
		src/base/view/RenderTexture.cpp
		src/base/view/Shader.cpp
		src/base/view/ShaderBundle.cpp
		src/base/view/ShaderCompilationError.cpp
		src/base/view/ShaderProgram.cpp
		src/base/view/Texture3D.cpp
		src/base/view/VertexShader.cpp
		src/base/view/Viewport.cpp
		src/base/view/BufferedHUVolumeUploader.cpp
		src/base/view/GeometryDefinition.cpp
		src/base/view/VideoResourcesManager.cpp
		src/base/view/RayMarchingStage.cpp
		src/base/model/HUVolume.cpp
		src/base/Application.cpp
		src/base/CarnaException.cpp
		src/base/Text.cpp
		src/base/Log.cpp
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
	
############################################

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
############################################

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

############################################

add_definitions( -D${PROJECT_NAME_CAPS}_EXPORT -DNOMINMAX -D_NO_FRAMEBUFFER_DEBUG )

############################################
# Add dependencies to the linker
############################################

target_link_libraries( ${TARGET_NAME}
			opengl32
			glu32
			${GLEW_LIBRARIES}
			${QT_LIBRARIES}
			${TRTK_LIBRARIES}
		)

############################################
# Define installation routines
############################################

install(TARGETS	${TARGET_NAME}
		RUNTIME	DESTINATION	${INSTALL_LIBRARY_DIR}
		ARCHIVE	DESTINATION	${INSTALL_LIBRARY_DIR}
		LIBRARY	DESTINATION	${INSTALL_LIBRARY_DIR})

# install headers
install(DIRECTORY ${CMAKE_PROJECT_DIR}include/${PROJECT_NAME}
		DESTINATION ${INSTALL_HEADERS_DIR})

# also install '.PDB' files on Windows
if( WIN32 )
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/bin/${TARGET_NAME}${CMAKE_DEBUG_POSTFIX}.pdb
			DESTINATION ${INSTALL_LIBRARY_DIR}
			OPTIONAL)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/bin/${TARGET_NAME}${CMAKE_DEBUG_POSTFIX}.pdb
			DESTINATION ${INSTALL_LIBRARY_DIR}
			OPTIONAL)
endif( WIN32 )

# install CMake Find<Module> file
install(	FILES ${CMAKE_CURRENT_BINARY_DIR}/misc/Find${PROJECT_NAME}.cmake
			DESTINATION ${INSTALL_CMAKE_DIR})

# install MediTEC-specific files			
add_meditec_install_routines()

############################################
# Build interactive demo
############################################

if( BUILD_DEMO )
	add_subdirectory( test/CarnaInteractiveTest )
endif()

############################################
# Process unit tests
############################################

if( BUILD_TEST )
	add_subdirectory(test)
endif()

############################################
# Doxygen API documentation
############################################

if( BUILD_DOC )
	add_subdirectory(src/doc)
endif()

############################################
