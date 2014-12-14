cmake_minimum_required(VERSION 2.8.7)

############################################
# It is important to use the root binary directory,
# so that the proper LIB from the tested project is found
# and the EXE is placed in the same directory as the DLL.
############################################
set( CMAKE_CURRENT_BINARY_DIR ${CMAKE_BINARY_DIR} )
############################################

set( CMAKE_DEBUG_POSTFIX d )
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin )



############################################
# Project name
############################################
project( CarnaInteractiveTest )
set( TARGET_NAME	${PROJECT_NAME}-${FULL_VERSION} )
############################################



############################################
# Find dependencies and add them later to the linker (see below)
############################################

# Qt

set( QT_USE_QTMAIN		TRUE )
set( QT_USE_QTOPENGL	TRUE )
find_package( Qt4 REQUIRED )
include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )

# Boost

include( $ENV{MEDITEC_LIBS}/Boost/1.55.0/configure.cmake )

# GLEW

include( $ENV{MEDITEC_LIBS}/GLEW/1.7.0/configure.cmake )

# Carna

include_directories( ${CMAKE_SOURCE_DIR}/include )
link_directories( ${CMAKE_BINARY_DIR}/bin )
message( STATUS ${CMAKE_BINARY_DIR}/bin )

# Carna-DICOM

find_package(CarnaDICOM QUIET)

# Project

include_directories(${CMAKE_PROJECT_DIR}src)
include_directories(${CMAKE_PROJECT_DIR}../Tools)
set( HEADERS
		../Tools/HUGZSceneFactory.h
		../Tools/HUIO.h
		../Tools/MyVisualization.h
		src/MPRDemo.h
		src/MyVisualizationDemo.h
		src/SlicePlaneVisualizationDemo.h
		src/VolumeVisualizationDemo.h
		src/OffscreenDisplayDemo.h
		src/InteractiveOffscreenDisplayDemo.h
		src/FactorialMaskDemo.h
		src/BinaryMaskDemo.h
		src/HistogramExporter.h
		src/CompressedVolumeExporter.h
		src/CompressedVolumeImporter.h
	)
set( QOBJECT_HEADERS
		src/OffscreenDisplayTestingWidget.h
		src/OpeningDialog.h
	)
set( SRC
		../Tools/MyVisualization.cpp
		src/main.cpp
		src/OffscreenDisplayDemo.cpp
		src/OffscreenDisplayTestingWidget.cpp
	)
set( FORMS
		""
	)
set( RESOURCES
		""
	)
	
unset( RESOURCES_RCC )
QT4_WRAP_CPP( QOBJECT_HEADERS_MOC ${QOBJECT_HEADERS} )
QT4_WRAP_UI( FORMS_HEADERS ${FORMS} )
QT4_ADD_RESOURCES( RESOURCES_RCC ${RESOURCES} )

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

if( CARNA_DICOM_FOUND )
	add_definitions(
			-DNOMINMAX
			-DBOOST_IOSTREAMS_NO_LIB
			-D_SCL_SECURE_NO_WARNINGS
			-DEXPECTED_MAJOR_VERSION=${MAJOR_VERSION}
			-DEXPECTED_MINOR_VERSION=${MINOR_VERSION}
			-DEXPECTED_RELEASE_VERSION=${PATCH_VERSION}
		)
	remove_definitions( -DCARNA_LIB )
	
	include_directories(${CARNA_DICOM_INCLUDE_DIR})
	
	add_executable( ${TARGET_NAME} WIN32
			${SRC}
			${HEADERS}
			${QOBJECT_HEADERS}
			${QOBJECT_HEADERS_MOC}
			${FORMS_HEADERS}
			${RESOURCES_RCC}
		)

	target_link_libraries( ${TARGET_NAME}
			opengl32
			glu32
			${GLEW_LIBLIST}
			${QT_LIBRARIES}
			${TRTK_LIBLIST}
			${BOOST_LIBLIST}
			${CARNA_DICOM_LIBRARIES}
			optimized	Carna-${FULL_VERSION}
			debug		Carna-${FULL_VERSION}${CMAKE_DEBUG_POSTFIX}
		)
else( CARNA_DICOM_FOUND )
	message(WARNING "\nCarnaDICOM not found, skipping ${PROJECT_NAME}\n")
endif( CARNA_DICOM_FOUND )
