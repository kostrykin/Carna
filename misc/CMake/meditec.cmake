############################################
# MediTEC Compatibility Script
# ------------------------------------------
# The script does nothing unless the
# environmental variable MEDITEC_LIBS is set
#-------------------------------------------
# Leonid Kostrykin, 12.12.2014
############################################

if( DEFINED ENV{MEDITEC_LIBS} )
	
	# make sure GLEW headers will be found
	list(APPEND CMAKE_INCLUDE_PATH "$ENV{MEDITEC_LIBS}/GLEW/1.7.0/include")
	
	# find GLEW libraries
	list(APPEND CMAKE_LIBRARY_PATH "$ENV{MEDITEC_LIBS}/GLEW/1.7.0/bin")
	find_library(GLEW_LIBRARY NAMES glew32-1.7.0 glew32s-1.7.0 PATH_SUFFIXES lib64)
	
	# find Eigen headers
	find_path(	EIGEN3_INCLUDE_DIR
				Eigen/Core
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "Eigen/3.0.5/include"
				DOC "Eigen3 headers")

endif( DEFINED ENV{MEDITEC_LIBS} )