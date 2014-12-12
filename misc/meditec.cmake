############################################
# MediTEC Compatibility Script
# ------------------------------------------
# The script does nothing unless the
# environmental variable MEDITEC_LIBS is set
############################################

if( DEFINED ENV{MEDITEC_LIBS} )

	# make sure GLEW includes will be found
	list(APPEND CMAKE_INCLUDE_PATH "$ENV{MEDITEC_LIBS}/GLEW/1.7.0/include")
	
	# find GLEW libraries
	list(APPEND CMAKE_LIBRARY_PATH "$ENV{MEDITEC_LIBS}/GLEW/1.7.0/bin")
	find_library(GLEW_LIBRARY NAMES glew32-1.7.0 glew32s-1.7.0 PATH_SUFFIXES lib64)

endif( DEFINED ENV{MEDITEC_LIBS} )