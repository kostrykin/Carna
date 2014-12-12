############################################
# MediTEC Compatibility Script
# ------------------------------------------
# The script does nothing unless the
# environmental variable MEDITEC_LIBS is set
#-------------------------------------------
# Leonid Kostrykin, 12.12.2014
############################################

if( DEFINED ENV{MEDITEC_LIBS} )

	# normalize paths so we can compare them
	get_filename_component(MEDITEC_LIBS $ENV{MEDITEC_LIBS} ABSOLUTE)

	# print warning if CMAKE_INSTALL_PREFIX isn't set to %MEDITEC_LIBS%
	if( NOT CMAKE_INSTALL_PREFIX STREQUAL ${MEDITEC_LIBS} )
		message(WARNING "\n**WARNING** CMAKE_INSTALL_PREFIX is not set to %MEDITEC_LIBS%\n"
		                  "    CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}\n"
		                  "    %MEDITEC_LIBS%: ${MEDITEC_LIBS}\n")
	else()
		# set default installation paths (relative to CMAKE_INSTALL_PREFIX)
		# now so they won't be set to different default values later
		option_default_to(INSTALL_LIBRARY_DIR "${PROJECT_NAME}/${FULL_VERSION}/bin")
		option_default_to(INSTALL_HEADERS_DIR "${PROJECT_NAME}/${FULL_VERSION}/include")
		option_default_to(INSTALL_CMAKE_DIR "CMake/Modules")
	endif()
	
	# find GLEW headers
	list(APPEND CMAKE_INCLUDE_PATH "$ENV{MEDITEC_LIBS}/GLEW/1.7.0/include")
	find_path(GLEW_INCLUDE_DIR GL/glew.h)
	
	# find GLEW libraries
	list(APPEND CMAKE_LIBRARY_PATH "$ENV{MEDITEC_LIBS}/GLEW/1.7.0/bin")
	find_library(GLEW_LIBRARY NAMES glew32-1.7.0 glew32s-1.7.0 PATH_SUFFIXES lib64)
	
	# find Eigen headers
	find_path(	EIGEN3_INCLUDE_DIR
				Eigen/Core
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "Eigen/3.0.5/include"
				DOC "Eigen3 headers")
	
	# install MediTEC-specific files
	if( CMAKE_INSTALL_PREFIX STREQUAL ${MEDITEC_LIBS} )
		configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/configure.cmake.in
						${CMAKE_CURRENT_BINARY_DIR}/configure.cmake @ONLY )
		
		configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/install.bat.in
						${CMAKE_CURRENT_BINARY_DIR}/install.bat @ONLY )
		
		install(FILES
					${CMAKE_CURRENT_BINARY_DIR}/configure.cmake
					${CMAKE_CURRENT_BINARY_DIR}/install.bat
				DESTINATION "${PROJECT_NAME}/${FULL_VERSION}")
	endif()

endif( DEFINED ENV{MEDITEC_LIBS} )