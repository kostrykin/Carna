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
option_default_to(INSTALL_LIBRARY_DIR "lib"                       String "Installation directory for libraries")
option_default_to(INSTALL_HEADERS_DIR "include"                   String "Installation directory for headers")
option_default_to(INSTALL_DOC_DIR     "share/doc/${PROJECT_NAME}" String "Installation directory for API documentation")

# set default destination for CMake modules
option_default_to(INSTALL_CMAKE_DIR "${CMAKE_ROOT}/Modules" String "Installation directory for CMake files")

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
set( PUBLIC_QOBJECT_HEADERS
		""
)
set( PUBLIC_HEADERS
		${PUBLIC_QOBJECT_HEADERS}
		include/${PROJECT_NAME}/base/BlendFunction.h
		include/${PROJECT_NAME}/base/Camera.h
		include/${PROJECT_NAME}/base/Color.h
		include/${PROJECT_NAME}/base/RenderQueue.h
		include/${PROJECT_NAME}/base/Spatial.h
		include/${PROJECT_NAME}/base/SpatialMovement.h
		include/${PROJECT_NAME}/base/Node.h
		include/${PROJECT_NAME}/base/Geometry.h
		include/${PROJECT_NAME}/base/Renderable.h
		include/${PROJECT_NAME}/base/FrameRenderer.h
		include/${PROJECT_NAME}/base/RenderTask.h
		include/${PROJECT_NAME}/base/RenderStage.h
		include/${PROJECT_NAME}/base/RenderState.h
		include/${PROJECT_NAME}/base/GeometryStage.h
		include/${PROJECT_NAME}/base/GLContext.h
		include/${PROJECT_NAME}/base/MeshRenderingStage.h
		include/${PROJECT_NAME}/base/Framebuffer.h
		include/${PROJECT_NAME}/base/RenderTexture.h
		include/${PROJECT_NAME}/base/Sampler.h
		include/${PROJECT_NAME}/base/Shader.h
		include/${PROJECT_NAME}/base/ShaderCompilationError.h
		include/${PROJECT_NAME}/base/ShaderProgram.h
		include/${PROJECT_NAME}/base/Material.h
		include/${PROJECT_NAME}/base/Viewport.h
		include/${PROJECT_NAME}/base/glError.h
		include/${PROJECT_NAME}/base/Texture3D.h
		include/${PROJECT_NAME}/base/BufferedHUVolumeFormat.h
		include/${PROJECT_NAME}/base/BufferedHUVolumeTexture.h
		include/${PROJECT_NAME}/base/HUVolumeTexture.h
		include/${PROJECT_NAME}/base/GeometryFeature.h
		include/${PROJECT_NAME}/base/Vertex.h
		include/${PROJECT_NAME}/base/VertexBuffer.h
		include/${PROJECT_NAME}/base/VertexAttributes.h
		include/${PROJECT_NAME}/base/IndexBuffer.h
		include/${PROJECT_NAME}/base/Mesh.h
		include/${PROJECT_NAME}/base/MeshFactory.h
		include/${PROJECT_NAME}/base/BaseBuffer.h
		include/${PROJECT_NAME}/base/ShaderManager.h
		include/${PROJECT_NAME}/base/ShaderUniform.h
		include/${PROJECT_NAME}/base/ScalarField.h
		include/${PROJECT_NAME}/base/HUVolumeGrid.h
		include/${PROJECT_NAME}/base/HUVolumeSegment.h
		include/${PROJECT_NAME}/base/HUVolume.h
		include/${PROJECT_NAME}/base/BufferedHUVolume.h
		include/${PROJECT_NAME}/base/UInt16HUVolume.h
		include/${PROJECT_NAME}/base/Aggregation.h
		include/${PROJECT_NAME}/base/Association.h
		include/${PROJECT_NAME}/base/CarnaException.h
		include/${PROJECT_NAME}/base/Composition.h
		include/${PROJECT_NAME}/base/noncopyable.h
		include/${PROJECT_NAME}/base/math.h
		include/${PROJECT_NAME}/base/RayPlaneHitTest.h
		include/${PROJECT_NAME}/base/Span.h
		include/${PROJECT_NAME}/base/Singleton.h
		include/${PROJECT_NAME}/base/text.h
		include/${PROJECT_NAME}/base/Log.h
		include/${PROJECT_NAME}/base/Ray.h
		include/${PROJECT_NAME}/base/Stopwatch.h
		include/${PROJECT_NAME}/base/CameraControl.h
		include/${PROJECT_NAME}/base/ProjectionControl.h
		include/${PROJECT_NAME}/base/glew.h
		include/${PROJECT_NAME}/presets/MeshColorCodingStage.h
		include/${PROJECT_NAME}/presets/RayMarchingStage.h
		include/${PROJECT_NAME}/presets/DRRStage.h
		include/${PROJECT_NAME}/presets/MIPStage.h
		include/${PROJECT_NAME}/presets/MIPChannel.h
		include/${PROJECT_NAME}/presets/CuttingPlanesStage.h
		include/${PROJECT_NAME}/presets/OccludedRenderingStage.h
		include/${PROJECT_NAME}/presets/OpaqueRenderingStage.h
		include/${PROJECT_NAME}/presets/TransparentRenderingStage.h
		include/${PROJECT_NAME}/presets/CameraNavigationControl.h
		include/${PROJECT_NAME}/presets/CameraShowcaseControl.h
		include/${PROJECT_NAME}/presets/PerspectiveControl.h
		include/${PROJECT_NAME}/presets/CompositionStage.h
		include/${PROJECT_NAME}/presets/ParallaxStage.h
		include/${PROJECT_NAME}/helpers/HUVolumeGridHelper.h
		include/${PROJECT_NAME}/helpers/FrameRendererHelper.h
		include/${PROJECT_NAME}/Carna.h
		include/${PROJECT_NAME}/Version.h
	)
include_directories(${CMAKE_PROJECT_DIR}src/include)
set( PRIVATE_QOBJECT_HEADERS
		""
)
set( PRIVATE_HEADERS
		${PRIVATE_QOBJECT_HEADERS}
		src/include/Carna/base/gl.h
		src/include/Carna/base/glu.h
	)
set( SRC
		src/base/BlendFunction.cpp
		src/base/Camera.cpp
		src/base/Color.cpp
		src/base/Geometry.cpp
		src/base/GLContext.cpp
		src/base/Renderable.cpp
		src/base/FrameRenderer.cpp
		src/base/RenderTask.cpp
		src/base/RenderStage.cpp
		src/base/RenderState.cpp
		src/base/Spatial.cpp
		src/base/SpatialMovement.cpp
		src/base/MeshRenderingStage.cpp
		src/base/Node.cpp
		src/base/Framebuffer.cpp
		src/base/RenderTexture.cpp
		src/base/Sampler.cpp
		src/base/Shader.cpp
		src/base/Material.cpp
		src/base/ShaderCompilationError.cpp
		src/base/ShaderProgram.cpp
		src/base/Texture3D.cpp
		src/base/Viewport.cpp
		src/base/BufferedHUVolumeFormat.cpp
		src/base/HUVolumeTexture.cpp
		src/base/GeometryFeature.cpp
		src/base/Stopwatch.cpp
		src/base/Vertex.cpp
		src/base/VertexAttributes.cpp
		src/base/VertexBuffer.cpp
		src/base/IndexBuffer.cpp
		src/base/Mesh.cpp
		src/base/BaseBuffer.cpp
		src/base/ShaderManager.cpp
		src/base/ShaderUniform.cpp
		src/base/HUVolume.cpp
		src/base/CarnaException.cpp
		src/base/text.cpp
		src/base/Log.cpp
		src/base/Ray.cpp
		src/base/CameraControl.cpp
		src/base/ProjectionControl.cpp
		src/presets/MeshColorCodingStage.cpp
		src/presets/RayMarchingStage.cpp
		src/presets/DRRStage.cpp
		src/presets/MIPStage.cpp
		src/presets/MIPChannel.cpp
		src/presets/CuttingPlanesStage.cpp
		src/presets/OccludedRenderingStage.cpp
		src/presets/TransparentRenderingStage.cpp
		src/presets/CameraNavigationControl.cpp
		src/presets/CameraShowcaseControl.cpp
		src/presets/PerspectiveControl.cpp
		src/presets/CompositionStage.cpp
		src/presets/ParallaxStage.cpp
		src/helpers/FrameRendererHelper.cpp
	)
set( FORMS
		""
	)
set( DOC_SRC
		src/doc/Doxyfile.in
		src/doc/doc_extra.css
		src/doc/doc_main.dox
		src/doc/doc_CoordinateSystems.dox
		src/doc/doc_VersionLog.dox
	)
set( SHADERS_SRC
		src/res/cutting-plane.vert
		src/res/cutting-plane.frag
		src/res/drr-accumulation.frag
		src/res/drr-accumulation.vert
		src/res/drr-exponential.frag
		src/res/drr-exponential.vert
		src/res/full_frame_quad.vert
		src/res/full_frame_quad.frag
		src/res/mip.frag
		src/res/mip.vert
		src/res/unshaded.vert
		src/res/unshaded.frag
	)
set( RESOURCES
		${SHADERS_SRC}
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
	REGULAR_EXPRESSION .*include/Carna/[^/]*\\.h )

SOURCE_GROUP( "Headers\\base"
	REGULAR_EXPRESSION .*include/Carna/base/[^/]*\\.h )

SOURCE_GROUP( "Headers\\presets"
	REGULAR_EXPRESSION .*include/Carna/presets/[^/]*\\.h )

SOURCE_GROUP( "Headers\\helpers"
	REGULAR_EXPRESSION .*include/Carna/helpers/[^/]*\\.h )

SOURCE_GROUP( "Sources"
	REGULAR_EXPRESSION src/[^/]*\\.cpp )

SOURCE_GROUP( "Sources\\base"
	REGULAR_EXPRESSION src/base/[^/]*\\.cpp )

SOURCE_GROUP( "Sources\\presets"
	REGULAR_EXPRESSION src/presets/[^/]*\\.cpp )

SOURCE_GROUP( "Sources\\helpers"
	REGULAR_EXPRESSION src/helpers/[^/]*\\.cpp )

SOURCE_GROUP( "Miscellaneous"
	REGULAR_EXPRESSION .*\\.\(\(in\)|\(txt\)|\(qrc\)\) )

SOURCE_GROUP( "Miscellaneous\\Documentation"
	REGULAR_EXPRESSION .*\\.\(\(css\)|\(dox\)\) )

SOURCE_GROUP( "Miscellaneous\\Shaders"
	REGULAR_EXPRESSION .*\\.\(\(frag\)|\(vert\)\) )

SOURCE_GROUP( "Miscellaneous\\Generated Files"
	REGULAR_EXPRESSION build/\(\(.*\\.\(\(cxx\)|\(cpp\)\)\)|\(.*/res/.*\)\) )
	
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
								${wrapped_cpp}
	PROPERTIES
	HEADER_FILE_ONLY TRUE )

############################################
# Write unity build file
############################################

set( UNITY_BUILD_FILE	${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}-unitybuild.cpp )

file( REMOVE	${UNITY_BUILD_FILE} )
file( WRITE		${UNITY_BUILD_FILE} "// This file is automatically generated by CMake.\n\n" )
file( APPEND	${UNITY_BUILD_FILE}	"#include <Carna/base/glew.h>\n" )
file( APPEND	${UNITY_BUILD_FILE}	"#include <Carna/base/CarnaException.h>\n" )
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
file( APPEND	${UNITY_BUILD_FILE} "        CARNA_ASSERT( strItr != strings.end() );\n" )
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
			${FORMS_HEADERS}
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
			${GLEW_LIBRARIES}
			${GOMP_LIBRARIES}
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
