cmake_minimum_required(VERSION 3.5)
project(LibCarna)

set(MAJOR_VERSION	3)
set(MINOR_VERSION	4)
set(PATCH_VERSION	0)

############################################

set(CMAKE_DEBUG_POSTFIX d)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)

string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWS)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_CAPS)
string(REGEX REPLACE "^lib" "" TARGET_NAME_BASE ${PROJECT_NAME_LOWS})

set(FULL_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
set(TARGET_NAME ${TARGET_NAME_BASE}-${FULL_VERSION}${TARGET_NAME_SUFFIX})

############################################
# Set default options for this build
############################################

option(BUILD_DOC	"Build and install the API documentation" OFF)
option(BUILD_TEST	"Build the unit tests" OFF)
option(BUILD_DEMO	"Build interactive demo" OFF)
option(BUILD_EGL	"Build GLEW with EGL support" OFF)

############################################
# Switch EGL support for GLEW on/off
############################################

if( BUILD_EGL )
    add_definitions( -DGLEW_EGL )
    set( EGL_LIBRARIES "EGL" )
else()
    set( EGL_LIBRARIES "" )
endif()

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
# Define default paths for the installation
############################################

# set default library and header destinations (relative to CMAKE_INSTALL_PREFIX)
option_default_to(INSTALL_LIBRARY_DIR "lib"                       STRING "Installation directory for libraries")
option_default_to(INSTALL_HEADERS_DIR "include"                   STRING "Installation directory for headers")
option_default_to(INSTALL_DOC_DIR     "share/doc/${PROJECT_NAME}" STRING "Installation directory for API documentation")
option_default_to(INSTALL_LICENSE_DIR "share/doc/${PROJECT_NAME}" STRING "Installation directory for license files")

# set default destination for CMake modules
option_default_to(INSTALL_CMAKE_DIR "${CMAKE_ROOT}/Modules" STRING "Installation directory for CMake files")

############################################
# Normalize installation paths
# (get rid of Windows-style delimiters)
############################################

file(	TO_CMAKE_PATH	${INSTALL_LIBRARY_DIR}	INSTALL_LIBRARY_DIR )
file(	TO_CMAKE_PATH	${INSTALL_HEADERS_DIR}	INSTALL_HEADERS_DIR )
file(	TO_CMAKE_PATH	${INSTALL_LICENSE_DIR}	INSTALL_LICENSE_DIR )
file(	TO_CMAKE_PATH	${INSTALL_CMAKE_DIR}	INSTALL_CMAKE_DIR )

############################################

configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/misc/Version.hpp.in
				${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/Version.hpp @ONLY)

configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/src/doc/Doxyfile.in
				${CMAKE_CURRENT_SOURCE_DIR}/src/doc/Doxyfile @ONLY)

configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/misc/Find${PROJECT_NAME}.cmake.in
				${CMAKE_CURRENT_BINARY_DIR}/misc/Find${PROJECT_NAME}.cmake @ONLY)

############################################
# Find required dependencies
############################################

# Eigen
find_package( Eigen3 REQUIRED )
include_directories( ${EIGEN3_INCLUDE_DIR} )

############################################
# Project
############################################

include_directories(${CMAKE_PROJECT_DIR}include)
set( PUBLIC_HEADERS
		include/${PROJECT_NAME}/LibCarna.hpp
		include/${PROJECT_NAME}/Version.hpp

		include/${PROJECT_NAME}/base/Aggregation.hpp
		include/${PROJECT_NAME}/base/Association.hpp
		include/${PROJECT_NAME}/base/BaseBuffer.hpp
		include/${PROJECT_NAME}/base/BlendFunction.hpp
		include/${PROJECT_NAME}/base/BoundingBox.hpp
		include/${PROJECT_NAME}/base/BoundingSphere.hpp
		include/${PROJECT_NAME}/base/BoundingVolume.hpp
		include/${PROJECT_NAME}/base/BufferedIntensityVolume.hpp
		include/${PROJECT_NAME}/base/BufferedNormalMap3D.hpp
		include/${PROJECT_NAME}/base/BufferedVectorFieldFormat.hpp
		include/${PROJECT_NAME}/base/BufferedVectorFieldTexture.hpp
		include/${PROJECT_NAME}/base/Camera.hpp
		include/${PROJECT_NAME}/base/CameraControl.hpp
		include/${PROJECT_NAME}/base/Color.hpp
		include/${PROJECT_NAME}/base/ColorMap.hpp
		include/${PROJECT_NAME}/base/Composition.hpp
		include/${PROJECT_NAME}/base/Framebuffer.hpp
		include/${PROJECT_NAME}/base/FrameRenderer.hpp
		include/${PROJECT_NAME}/base/Geometry.hpp
		include/${PROJECT_NAME}/base/GeometryFeature.hpp
		include/${PROJECT_NAME}/base/GeometryStage.hpp
		include/${PROJECT_NAME}/base/GL/eglew.h
		include/${PROJECT_NAME}/base/GL/glew.h
		include/${PROJECT_NAME}/base/GL/glxew.h
		include/${PROJECT_NAME}/base/GL/wglew.h
		include/${PROJECT_NAME}/base/GLContext.hpp
		include/${PROJECT_NAME}/base/glError.hpp
		include/${PROJECT_NAME}/base/glew.hpp
		include/${PROJECT_NAME}/base/HUV.hpp
		include/${PROJECT_NAME}/base/IndexBuffer.hpp
		include/${PROJECT_NAME}/base/IntensityVolume.hpp
		include/${PROJECT_NAME}/base/LibCarnaException.hpp
		include/${PROJECT_NAME}/base/Log.hpp
		include/${PROJECT_NAME}/base/ManagedMesh.hpp
		include/${PROJECT_NAME}/base/ManagedMeshInterface.hpp
		include/${PROJECT_NAME}/base/ManagedTexture3D.hpp
		include/${PROJECT_NAME}/base/ManagedTexture3DInterface.hpp
		include/${PROJECT_NAME}/base/Material.hpp
		include/${PROJECT_NAME}/base/math.hpp
		include/${PROJECT_NAME}/base/math/Ray.hpp
		include/${PROJECT_NAME}/base/math/RayPlaneHitTest.hpp
		include/${PROJECT_NAME}/base/math/Span.hpp
		include/${PROJECT_NAME}/base/math/VectorField.hpp
		include/${PROJECT_NAME}/base/Mesh.hpp
		include/${PROJECT_NAME}/base/MeshFactory.hpp
		include/${PROJECT_NAME}/base/MeshRenderingStage.hpp
		include/${PROJECT_NAME}/base/Node.hpp
		include/${PROJECT_NAME}/base/NodeListener.hpp
		include/${PROJECT_NAME}/base/noncopyable.hpp
		include/${PROJECT_NAME}/base/NormalMap3D.hpp
		include/${PROJECT_NAME}/base/ProjectionControl.hpp
		include/${PROJECT_NAME}/base/Renderable.hpp
		include/${PROJECT_NAME}/base/RenderQueue.hpp
		include/${PROJECT_NAME}/base/RenderStage.hpp
		include/${PROJECT_NAME}/base/RenderStageListener.hpp
		include/${PROJECT_NAME}/base/RenderStageSequence.hpp
		include/${PROJECT_NAME}/base/RenderState.hpp
		include/${PROJECT_NAME}/base/RenderTask.hpp
		include/${PROJECT_NAME}/base/RotatingColor.hpp
		include/${PROJECT_NAME}/base/Sampler.hpp
		include/${PROJECT_NAME}/base/Shader.hpp
		include/${PROJECT_NAME}/base/ShaderCompilationError.hpp
		include/${PROJECT_NAME}/base/ShaderManager.hpp
		include/${PROJECT_NAME}/base/ShaderProgram.hpp
		include/${PROJECT_NAME}/base/ShaderUniform.hpp
		include/${PROJECT_NAME}/base/Singleton.hpp
		include/${PROJECT_NAME}/base/Spatial.hpp
		include/${PROJECT_NAME}/base/SpatialMovement.hpp
		include/${PROJECT_NAME}/base/Stopwatch.hpp
		include/${PROJECT_NAME}/base/text.hpp
		include/${PROJECT_NAME}/base/Texture.hpp
		include/${PROJECT_NAME}/base/Vertex.hpp
		include/${PROJECT_NAME}/base/VertexAttributes.hpp
		include/${PROJECT_NAME}/base/VertexBuffer.hpp
		include/${PROJECT_NAME}/base/Viewport.hpp
		include/${PROJECT_NAME}/base/VolumeGrid.hpp
		include/${PROJECT_NAME}/base/VolumeSegment.hpp

		include/${PROJECT_NAME}/presets/CameraNavigationControl.hpp
		include/${PROJECT_NAME}/presets/CameraShowcaseControl.hpp
		include/${PROJECT_NAME}/presets/CompositionStage.hpp
		include/${PROJECT_NAME}/presets/CuttingPlanesStage.hpp
		include/${PROJECT_NAME}/presets/DRRStage.hpp
		include/${PROJECT_NAME}/presets/DVRStage.hpp
		include/${PROJECT_NAME}/presets/MaskRenderingStage.hpp
		include/${PROJECT_NAME}/presets/MeshColorCodingStage.hpp
		include/${PROJECT_NAME}/presets/MIPStage.hpp
		include/${PROJECT_NAME}/presets/OccludedRenderingStage.hpp
		include/${PROJECT_NAME}/presets/OpaqueRenderingStage.hpp
		include/${PROJECT_NAME}/presets/OrthogonalControl.hpp
		include/${PROJECT_NAME}/presets/ParallaxStage.hpp
		include/${PROJECT_NAME}/presets/PerspectiveControl.hpp
		include/${PROJECT_NAME}/presets/TransparentRenderingStage.hpp
		include/${PROJECT_NAME}/presets/VolumeRenderingStage.hpp

		include/${PROJECT_NAME}/helpers/FrameRendererHelper.hpp
		include/${PROJECT_NAME}/helpers/PointMarkerHelper.hpp
		include/${PROJECT_NAME}/helpers/VolumeGridHelper.hpp
		include/${PROJECT_NAME}/helpers/VolumeGridHelperDetails.hpp
	)
include_directories(${CMAKE_PROJECT_DIR}src/include)
set( PRIVATE_QOBJECT_HEADERS
		""
)
set( PRIVATE_HEADERS
		${PRIVATE_QOBJECT_HEADERS}
	)
set( SRC
		src/base/BaseBuffer.cpp
		src/base/BlendFunction.cpp
		src/base/BoundingBox.cpp
		src/base/BoundingSphere.cpp
		src/base/BoundingVolume.cpp
		src/base/BufferedVectorFieldFormat.cpp
		src/base/Camera.cpp
		src/base/CameraControl.cpp
		src/base/Color.cpp
		src/base/ColorMap.cpp
		src/base/Framebuffer.cpp
		src/base/FrameRenderer.cpp
		src/base/Geometry.cpp
		src/base/GeometryFeature.cpp
		src/base/GL/glew.c
		src/base/GLContext.cpp
		src/base/IndexBuffer.cpp
		src/base/IntensityVolume.cpp
		src/base/LibCarnaException.cpp
		src/base/Log.cpp
		src/base/ManagedMesh.cpp
		src/base/ManagedMeshInterface.cpp
		src/base/ManagedTexture3D.cpp
		src/base/ManagedTexture3DInterface.cpp
		src/base/Material.cpp
		src/base/math/Ray.cpp
		src/base/Mesh.cpp
		src/base/MeshRenderingStage.cpp
		src/base/Node.cpp
		src/base/NodeListener.cpp
		src/base/NormalMap3D.cpp
		src/base/ProjectionControl.cpp
		src/base/Renderable.cpp
		src/base/RenderStage.cpp
		src/base/RenderStageListener.cpp
		src/base/RenderStageSequence.cpp
		src/base/RenderState.cpp
		src/base/RenderTask.cpp
		src/base/RotatingColor.cpp
		src/base/Sampler.cpp
		src/base/Shader.cpp
		src/base/ShaderCompilationError.cpp
		src/base/ShaderManager.cpp
		src/base/ShaderProgram.cpp
		src/base/ShaderUniform.cpp
		src/base/Spatial.cpp
		src/base/SpatialMovement.cpp
		src/base/Stopwatch.cpp
		src/base/text.cpp
		src/base/Texture.cpp
		src/base/Vertex.cpp
		src/base/VertexAttributes.cpp
		src/base/VertexBuffer.cpp
		src/base/Viewport.cpp

		src/presets/CameraNavigationControl.cpp
		src/presets/CameraShowcaseControl.cpp
		src/presets/CompositionStage.cpp
		src/presets/CuttingPlanesStage.cpp
		src/presets/DRRStage.cpp
		src/presets/DVRStage.cpp
		src/presets/MaskRenderingStage.cpp
		src/presets/MeshColorCodingStage.cpp
		src/presets/MIPStage.cpp
		src/presets/OccludedRenderingStage.cpp
		src/presets/OrthogonalControl.cpp
		src/presets/ParallaxStage.cpp
		src/presets/PerspectiveControl.cpp
		src/presets/TransparentRenderingStage.cpp
		src/presets/VolumeRenderingStage.cpp

		src/helpers/FrameRendererHelper.cpp
		src/helpers/PointMarkerHelper.cpp
		src/helpers/VolumeGridHelper.cpp
		src/helpers/VolumeGridHelperDetails.cpp
	)
set( DOC_SRC
		src/doc/Doxyfile.in
		src/doc/doc_extra.css
		src/doc/doc_main.dox
		src/doc/doc_CoordinateSystems.dox
		src/doc/doc_versions.dox
	)
set( SHADERS_SRC
		src/res/cutting-plane.frag
		src/res/cutting-plane.vert
		src/res/drr-accumulation.frag
		src/res/drr-accumulation.vert
		src/res/drr-exponential.frag
		src/res/drr-exponential.vert
		src/res/dvr.frag
		src/res/dvr.vert
		src/res/full_frame_quad.frag
		src/res/full_frame_quad.vert
		src/res/interleave.frag
		src/res/interleave.vert
		src/res/mip_colorization.frag
		src/res/mip_colorization.vert
		src/res/mip.frag
		src/res/mip.vert
		src/res/mr-edgedetect.frag
		src/res/mr-edgedetect.vert
		src/res/mr.frag
		src/res/mr.vert
		src/res/pointmarker.frag
		src/res/pointmarker.geom
		src/res/pointmarker.vert
		src/res/solid.frag
		src/res/solid.vert
		src/res/unshaded.frag
		src/res/unshaded.vert
	)
set( RESOURCES
		${SHADERS_SRC}
	)
set( LICENSES
        LICENSE-LibCarna
        LICENSE-Carna
        LICENSE-Eigen
        LICENSE-GLEW
    )
	
############################################

set( wrapped_cpp "" )
set( wrapped_vars "" )
macro( wrap_cpp file_path )
	file( READ ${file_path} file_content )
	string( REGEX REPLACE "\"" "\\\\\"" file_content "${file_content}" )
	string( REGEX REPLACE ";" "\\\\;" file_content "${file_content}" )
	string( REGEX REPLACE "\n" ";" file_content "${file_content}" )
	get_filename_component( file_basename ${file_path} NAME_WE )
	get_filename_component( file_ext ${file_path} EXT )
	set( wrapped_cpp_path ${CMAKE_CURRENT_BINARY_DIR}/res/${file_basename}${file_ext}.h )
	set( wrapped_var ${file_basename}${file_ext} )
	string( REGEX REPLACE "[^a-zA-Z0-9]" "_" wrapped_var "${wrapped_var}" )
	list( APPEND wrapped_cpp "${wrapped_cpp_path}" )
	list( APPEND wrapped_vars "${wrapped_var}" )
	
	file( REMOVE		${wrapped_cpp_path} )
	file( WRITE			${wrapped_cpp_path} "// This file is automatically generated by CMake.\n\n" )
	file( APPEND		${wrapped_cpp_path} "namespace res\n" )
	file( APPEND		${wrapped_cpp_path} "{\n" )
	file( APPEND		${wrapped_cpp_path}	"    const std::string ${wrapped_var} = \"\"\n" )

	foreach( file_line ${file_content} )
		file( APPEND	${wrapped_cpp_path}	"        \"${file_line}\\n\"\n" )
	endforeach( file_line )
	
	file( APPEND		${wrapped_cpp_path} "        \"\";\n" )
	file( APPEND		${wrapped_cpp_path} "}  // namespace res\n" )
endmacro()

foreach( res_file ${RESOURCES} )
	wrap_cpp( ${res_file} )
endforeach( res_file )

############################################
# Setup Visual Studio filters
############################################

SOURCE_GROUP( "Headers"
	REGULAR_EXPRESSION .*include/Carna/[^/]*\\.hpp )

SOURCE_GROUP( "Headers\\base"
	REGULAR_EXPRESSION .*include/Carna/base/[^/]*\\.hpp )

SOURCE_GROUP( "Headers\\base\\math"
	REGULAR_EXPRESSION .*include/Carna/base/math.*\\.hpp )

SOURCE_GROUP( "Headers\\presets"
	REGULAR_EXPRESSION .*include/Carna/presets/[^/]*\\.hpp )

SOURCE_GROUP( "Headers\\helpers"
	REGULAR_EXPRESSION .*include/Carna/helpers/[^/]*\\.hpp )

SOURCE_GROUP( "Sources"
	REGULAR_EXPRESSION src/[^/]*\\.cpp )

SOURCE_GROUP( "Sources\\base"
	REGULAR_EXPRESSION src/base/[^/]*\\.cpp )

SOURCE_GROUP( "Sources\\base\\math"
	REGULAR_EXPRESSION src/base/math.*\\.cpp )

SOURCE_GROUP( "Sources\\presets"
	REGULAR_EXPRESSION src/presets/[^/]*\\.cpp )

SOURCE_GROUP( "Sources\\helpers"
	REGULAR_EXPRESSION src/helpers/[^/]*\\.cpp )

SOURCE_GROUP( "Miscellaneous"
	REGULAR_EXPRESSION .*\\.\(\(in\)|\(txt\)|\(qrc\)\) )

SOURCE_GROUP( "Miscellaneous\\Documentation"
	REGULAR_EXPRESSION .*\\.\(\(css\)|\(dox\)\) )

SOURCE_GROUP( "Miscellaneous\\Shaders"
	REGULAR_EXPRESSION .*\\.\(\(frag\)|\(geom\)|\(vert\)\) )

SOURCE_GROUP( "Miscellaneous\\Generated Files"
	REGULAR_EXPRESSION build/\(\(.*\\.\(\(cxx\)|\(cpp\)\)\)|\(.*/res/.*\)\) )
	
############################################
# Exclude all source files from build,
# since we are using unity build.
############################################

set_source_files_properties(	${SRC}
								${PUBLIC_HEADERS_MOC}
								${PRIVATE_HEADERS_MOC}
								${RESOURCES_RCC}
								${DOC_SRC}
								${SHADERS_SRC}
								${wrapped_cpp}
	PROPERTIES
	HEADER_FILE_ONLY TRUE )

############################################
# Write unity build file
############################################

set( UNITY_BUILD_FILE	${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}-unitybuild.cpp )

file( REMOVE	${UNITY_BUILD_FILE} )
file( WRITE		${UNITY_BUILD_FILE} "// This file is automatically generated by CMake.\n\n" )
file( APPEND	${UNITY_BUILD_FILE}	"#include <LibCarna/base/glew.hpp>\n" )
file( APPEND	${UNITY_BUILD_FILE}	"#include <LibCarna/base/LibCarnaException.hpp>\n" )
file( APPEND	${UNITY_BUILD_FILE}	"#include <string>\n" )
file( APPEND	${UNITY_BUILD_FILE}	"#include <map>\n" )

foreach( WRAPPED_FILE ${wrapped_cpp} )
	file( APPEND	${UNITY_BUILD_FILE}	"#include \"${WRAPPED_FILE}\"\n" )
endforeach( WRAPPED_FILE )

file( APPEND	${UNITY_BUILD_FILE} "namespace res\n{\n" )
file( APPEND	${UNITY_BUILD_FILE} "    std::map< std::string, const std::string* > genList()\n" )
file( APPEND	${UNITY_BUILD_FILE} "    {\n" )
file( APPEND	${UNITY_BUILD_FILE} "        std::map< std::string, const std::string* > vars;\n" )

foreach( WRAPPED_VAR ${wrapped_vars} )
	file( APPEND	${UNITY_BUILD_FILE}	"        vars[ \"${WRAPPED_VAR}\" ] = &${WRAPPED_VAR};\n" )
endforeach( WRAPPED_VAR )

file( APPEND	${UNITY_BUILD_FILE} "        return vars;\n" )
file( APPEND	${UNITY_BUILD_FILE} "    }\n" )
file( APPEND	${UNITY_BUILD_FILE} "    const std::map< std::string, const std::string* > strings = genList();\n" )
file( APPEND	${UNITY_BUILD_FILE} "    const std::string& string( const std::string& key )\n" )
file( APPEND	${UNITY_BUILD_FILE} "    {\n" )
file( APPEND	${UNITY_BUILD_FILE} "        const auto strItr = strings.find( key );\n" )
file( APPEND	${UNITY_BUILD_FILE} "        LIBCARNA_ASSERT( strItr != strings.end() );\n" )
file( APPEND	${UNITY_BUILD_FILE} "        return *strItr->second;\n" )
file( APPEND	${UNITY_BUILD_FILE} "    }\n" )
file( APPEND	${UNITY_BUILD_FILE} "}\n" )

foreach( SOURCE_FILE ${SRC} )
	file( APPEND	${UNITY_BUILD_FILE}	"#include \"${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}\"\n" )
endforeach( SOURCE_FILE )

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
			${RESOURCES_RCC}
			${DOC_SRC}
			${SHADERS_SRC}
			${wrapped_cpp}
		)

add_definitions( -D${PROJECT_NAME_CAPS}_EXPORT -DNOMINMAX -D_NO_FRAMEBUFFER_DEBUG )

############################################
# Add dependencies to the linker
############################################

include( "misc/compiler_specific.cmake" )

target_link_libraries( ${TARGET_NAME}
			${OPENGL_LIBRARIES}
            ${EGL_LIBRARIES}
			${GOMP_LIBRARIES}
		)

############################################
# Define installation routines
############################################

install(
    TARGETS	${TARGET_NAME}
	RUNTIME	DESTINATION	${INSTALL_LIBRARY_DIR}
	ARCHIVE	DESTINATION	${INSTALL_LIBRARY_DIR}
	LIBRARY	DESTINATION	${INSTALL_LIBRARY_DIR}
)

# install headers
install(
    DIRECTORY ${CMAKE_PROJECT_DIR}include/${PROJECT_NAME}
	DESTINATION ${INSTALL_HEADERS_DIR}
)

# also install '.PDB' files on Windows
if( WIN32 )
	install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/bin/${TARGET_NAME}${CMAKE_DEBUG_POSTFIX}.pdb
		DESTINATION ${INSTALL_LIBRARY_DIR}
		OPTIONAL
    )
	install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/bin/${TARGET_NAME}${CMAKE_DEBUG_POSTFIX}.pdb
		DESTINATION ${INSTALL_LIBRARY_DIR}
		OPTIONAL
    )
endif( WIN32 )

# install CMake Find<Module> file
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/misc/Find${PROJECT_NAME}.cmake
	DESTINATION ${INSTALL_CMAKE_DIR}
)

# install license files
install(
    FILES ${LICENSES}
	DESTINATION ${INSTALL_LICENSE_DIR}
)

############################################
# Build interactive demo
############################################

if( BUILD_DEMO )
	add_subdirectory( test/Demo )
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
