############################################
# Locates GDCM library.
# ------------------------------------------
# If GDCM headers and library files are not
# found in the standard CMake locations,
# then %MEDITEC_LIBS%/gdcm/<version> is
# checked. In this, letter case it is
# necessary that the version is matched
# exactly. Consider using symbolic links
# when you have a compatible version.
#-------------------------------------------
# The following variables are set:
#	GDCM_INCLUDE_DIR	points to headers
#	GDCM_LIBRARY		points to lib file
#	GDCM_FOUND			indicates success
#-------------------------------------------
# Leonid Kostrykin, 12.12.2014
############################################

find_path(	GDCM_INCLUDE_DIR
			gdcm/gdcmReader.h gdcm/gdcmImage.h
			PATHS ENV MEDITEC_LIBS
			PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/include"
			DOC "GDCM headers")

find_library(	GDCM_LIBRARY
				NAMES "gdcmCommon-${GDCM_FIND_VERSION}" gdcmCommon
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "gdcm/${GDCM_FIND_VERSION}/bin"
				DOC "GDCM library")

# handles REQUIRED, QUIET and version-related
# arguments and also sets the _FOUND variable
find_package_handle_standard_args(GDCM
	REQUIRED_VARS GDCM_INCLUDE_DIR GDCM_LIBRARY)