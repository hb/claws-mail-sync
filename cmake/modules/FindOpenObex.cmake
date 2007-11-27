# - Try to find OpenObex
# Find OpenObex headers, libraries and the answer to all questions.
#
#  OPENOBEX_FOUND               True if OpenObex got found
#  OPENOBEX_INCLUDE_DIR         Location of OpenObex headers 
#  OPENOBEX_LIBRARIES           List of libaries to use OpenObex
#  OPENOBEX_DEFINITIONS         Definitions to compile OpenObex 
#
# Copyright (c) 2007 Daniel Gollub <dgollub@suse.de>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

IF ( NOT WIN32 )
	INCLUDE( UsePkgConfig )
	# Take care about openobex.pc settings
	PKGCONFIG( openobex _openobex_include_DIR _openobex_link_DIR _openobex_link_FLAGS _openobex_cflags )
ENDIF ( NOT WIN32 )

# Look for OpenObex include dir and libraries, and take care about pkg-config first...
FIND_PATH( OPENOBEX_INCLUDE_DIR openobex/obex.h PATHS ${_openobex_include_DIR} NO_DEFAULT_PATH )
FIND_PATH( OPENOBEX_INCLUDE_DIR openobex/obex.h
		PATHS
		/opt/local/include/
		/sw/include/
		/usr/local/include/
		/usr/include/ )

FIND_LIBRARY( OPENOBEX_LIBRARIES openobex PATHS ${_openobex_link_DIR} NO_DEFAULT_PATH )
FIND_LIBRARY( OPENOBEX_LIBRARIES openobex
		PATHS
		/opt/local/lib
		/sw/lib
		/usr/lib
		/usr/local/lib
		/usr/lib64
		/usr/local/lib64
		/opt/lib64 )

# Report results
IF ( OPENOBEX_LIBRARIES AND OPENOBEX_INCLUDE_DIR )	
	SET( OPENOBEX_FOUND 1 )
	IF ( NOT OPENOBEX_FIND_QUIETLY )
		MESSAGE( STATUS "Found OpenObex: ${OPENOBEX_LIBRARIES}" )
	ENDIF ( NOT OPENOBEX_FIND_QUIETLY )
ELSE ( OPENOBEX_LIBRARIES AND OPENOBEX_INCLUDE_DIR )	
	IF ( NOT OPENOBEX_FIND_QUIETLY )
		MESSAGE( STATUS "Could NOT find OpenObex" )
	ENDIF ( NOT OPENOBEX_FIND_QUIETLY )
ENDIF ( OPENOBEX_LIBRARIES AND OPENOBEX_INCLUDE_DIR )	

# Hide advanced variables from CMake GUIs
MARK_AS_ADVANCED( OPENOBEX_LIBRARIES OPENOBEX_INCLUDE_DIR )

