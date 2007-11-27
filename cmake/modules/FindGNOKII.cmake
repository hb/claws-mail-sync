# - Try to find GNOKII
# Find GNOKII headers, libraries and the answer to all questions.
#
#  GNOKII_FOUND               True if GNOKII got found
#  GNOKII_INCLUDE_DIR         Location of GNOKII headers 
#  GNOKII_LIBRARIES           List of libaries to use GNOKII
#  GNOKII_DEFINITIONS         Definitions to compile GNOKII 
#
# Copyright (c) 2007 Daniel Gollub <dgollub@suse.de>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

IF ( NOT WIN32 )
	INCLUDE( UsePkgConfig )
	# Take care about gnokii.pc settings
	PKGCONFIG( gnokii _gnokii_include_DIR _gnokii_link_DIR _gnokii_link_FLAGS _gnokii_cflags )
ENDIF ( NOT WIN32 )

# Look for GNOKII include dir and libraries, and take care about pkg-config first...
FIND_PATH( GNOKII_INCLUDE_DIR gnokii.h PATHS ${_gnokii_include_DIR} NO_DEFAULT_PATH )
FIND_PATH( GNOKII_INCLUDE_DIR gnokii.h )

FIND_LIBRARY( GNOKII_LIBRARIES gnokii PATHS ${_gnokii_link_DIR} NO_DEFAULT_PATH )
FIND_LIBRARY( GNOKII_LIBRARIES gnokii )

# Report results
IF ( GNOKII_LIBRARIES AND GNOKII_INCLUDE_DIR )	
	SET( GNOKII_FOUND 1 )
	IF ( NOT GNOKII_FIND_QUIETLY )
		MESSAGE( STATUS "Found GNOKII: ${GNOKII_LIBRARIES}" )
	ENDIF ( NOT GNOKII_FIND_QUIETLY )
ELSE ( GNOKII_LIBRARIES AND GNOKII_INCLUDE_DIR )	
	IF ( NOT GNOKII_FIND_QUIETLY )
		MESSAGE( STATUS "Could NOT find GNOKII" )
	ENDIF ( NOT GNOKII_FIND_QUIETLY )
ENDIF ( GNOKII_LIBRARIES AND GNOKII_INCLUDE_DIR )	

# Hide advanced variables from CMake GUIs
MARK_AS_ADVANCED( GNOKII_LIBRARIES GNOKII_INCLUDE_DIR )

