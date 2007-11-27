# - Try to find PilotLink
# Find PilotLink headers, libraries and the answer to all questions.
#
#  PILOTLINK_FOUND               True if PilotLink got found
#  PILOTLINK_INCLUDE_DIR         Location of PilotLink headers 
#  PILOTLINK_LIBRARIES           List of libaries to use PilotLink
#  PILOTLINK_DEFINITIONS         Definitions to compile PilotLink 
#
# Copyright (c) 2007 Daniel Gollub <dgollub@suse.de>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

IF ( NOT WIN32 )
	INCLUDE( UsePkgConfig )
	# Take care about pilot-link.pc settings
	PKGCONFIG( pilot-link _pilotlink_include_DIR _pilotlink_link_DIR _pilotlink_link_FLAGS _pilotlink_cflags )
ENDIF ( NOT WIN32 )

# Look for PilotLink include dir and libraries, and take care about pkg-config first...
FIND_PATH( PILOTLINK_INCLUDE_DIR pi-version.h PATHS ${_pilotlink_include_DIR} NO_DEFAULT_PATH )
FIND_PATH( PILOTLINK_INCLUDE_DIR pi-version.h 
		PATHS
		/opt/local/include/
		/sw/include/
		/usr/local/include/
		/usr/include/ )

FIND_LIBRARY( PILOTLINK_LIBRARIES pisock PATHS ${_pilotlink_link_DIR} NO_DEFAULT_PATH )

FIND_LIBRARY( PILOTLINK_LIBRARIES pisock 
		PATHS
		/opt/local/lib
		/sw/lib
		/usr/lib
		/usr/local/lib
		/usr/lib64
		/usr/local/lib64
		/opt/lib64 )

# Report results
IF ( PILOTLINK_LIBRARIES AND PILOTLINK_INCLUDE_DIR )	
	SET( PILOTLINK_FOUND 1 )
	IF ( NOT PILOTLINK_FIND_QUIETLY )
		MESSAGE( STATUS "Found PilotLink: ${PILOTLINK_LIBRARIES}" )
	ENDIF ( NOT PILOTLINK_FIND_QUIETLY )
ELSE ( PILOTLINK_LIBRARIES AND PILOTLINK_INCLUDE_DIR )	
	IF ( NOT PILOTLINK_FIND_QUIETLY )
		MESSAGE( STATUS "Could NOT find PilotLink" )
	ENDIF ( NOT PILOTLINK_FIND_QUIETLY )
ENDIF ( PILOTLINK_LIBRARIES AND PILOTLINK_INCLUDE_DIR )	

# Hide advanced variables from CMake GUIs
MARK_AS_ADVANCED( PILOTLINK_LIBRARIES PILOTLINK_INCLUDE_DIR )

