############################################
# MediTEC Compatibility Script
# ------------------------------------------
# The script does nothing unless the
# environmental variable MEDITEC_LIBS is set
############################################

if( DEFINED ENV{MEDITEC_LIBS} )

	# set where Find<ModuleName>.cmake scripts are located
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/misc/MediTEC)

	
	# make sure GLEW includes will be found
	list(APPEND CMAKE_INCLUDE_PATH "$ENV{MEDITEC_LIBS}/GLEW/1.7.0/include")
	
	# find GLEW libraries
	list(APPEND CMAKE_LIBRARY_PATH "$ENV{MEDITEC_LIBS}/GLEW/1.7.0/bin")
	find_library(GLEW_LIBRARY NAMES glew32-1.7.0 glew32s-1.7.0 PATH_SUFFIXES lib64)
	
	
	# make sure TRTK includes will be found
	list(APPEND CMAKE_INCLUDE_PATH "$ENV{MEDITEC_LIBS}/TRTK/0.13.1/include")
	
	# find TRTK libraries
	list(APPEND CMAKE_LIBRARY_PATH "$ENV{MEDITEC_LIBS}/TRTK/0.13.1/bin")
	find_library(TRTK_LIBRARY NAMES TRTK-0.13.1)

endif( DEFINED ENV{MEDITEC_LIBS} )