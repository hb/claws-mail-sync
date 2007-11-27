# - Try to find libsyncml
# Find libsyncml headers, libraries and the answer to all questions.
#
#  LIBSYNCML_FOUND               True if libsyncml got found
#  LIBSYNCML_INCLUDE_DIR         Location of libsyncml headers 
#  LIBSYNCML_LIBRARIES           List of libaries to use libsyncml
#  LIBSYNCML_DEFINITIONS         Definitions to compile libsyncml 
#
# Copyright (c) 2007 Juha Tuomala <tuju@iki.fi>
# Copyright (c) 2007 Daniel Gollub <dgollub@suse.de>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

IF ( NOT WIN32 )
	INCLUDE( UsePkgConfig )
	# Take care about libsyncml-1.0.pc settings
	PKGCONFIG( libsyncml-1.0 _libsyncml_include_DIR _libsyncml_link_DIR _libsyncml_link_FLAGS _libsyncml_cflags )
ENDIF ( NOT WIN32 )

# Look for libsyncml include dir and libraries, and take care about pkg-config first...
FIND_PATH( LIBSYNCML_INCLUDE_DIR libsyncml/syncml.h 
		PATHS ${_libsyncml_include_DIR} 
		PATH_SUFFIXES libsyncml-1.0 NO_DEFAULT_PATH )

FIND_PATH( LIBSYNCML_INCLUDE_DIR libsyncml/syncml.h PATH_SUFFIXES libsyncml-1.0 
		PATHS
		/opt/local/include/
		/sw/include/
		/usr/local/include/
		/usr/include/ )

FIND_LIBRARY( LIBSYNCML_LIBRARIES syncml 
		PATHS ${_libsyncml_link_DIR} NO_DEFAULT_PATH )

FIND_LIBRARY( LIBSYNCML_LIBRARIES syncml 
		PATHS
		/opt/local/lib
		/sw/lib
		/usr/lib
		/usr/local/lib
		/usr/lib64
		/usr/local/lib64
		/opt/lib64 )

# Report results
IF ( LIBSYNCML_LIBRARIES AND LIBSYNCML_INCLUDE_DIR )	
	SET( LIBSYNCML_FOUND 1 )
	IF ( NOT LIBSYNCML_FIND_QUIETLY )
		MESSAGE( STATUS "Found libsyncml: ${LIBSYNCML_LIBRARIES}" )
	ENDIF ( NOT LIBSYNCML_FIND_QUIETLY )
ELSE ( LIBSYNCML_LIBRARIES AND LIBSYNCML_INCLUDE_DIR )	
	IF ( NOT LIBSYNCML_FIND_QUIETLY )
		MESSAGE( STATUS "Could NOT find libsyncml" )
	ENDIF ( NOT LIBSYNCML_FIND_QUIETLY )
ENDIF ( LIBSYNCML_LIBRARIES AND LIBSYNCML_INCLUDE_DIR )	

# Hide advanced variables from CMake GUIs
MARK_AS_ADVANCED( LIBSYNCML_LIBRARIES LIBSYNCML_INCLUDE_DIR )

