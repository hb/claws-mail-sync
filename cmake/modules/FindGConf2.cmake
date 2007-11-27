# - Try to find gconf2 
# Find gconf2 headers, libraries and the answer to all questions.
#
#  GCONF2_FOUND               True if gconf2 got found
#  GCONF2_INCLUDE_DIR         Location of gconf2 headers 
#  GCONF2_LIBRARIES           List of libaries to use gconf2
#  GCONF2_DEFINITIONS         Definitions to compile gconf2 
#
# Copyright (c) 2007 Juha Tuomala <tuju@iki.fi>
# Copyright (c) 2007 Daniel Gollub <dgollub@suse.de>
# Copyright (c) 2007 Alban Browaeys <prahal@yahoo.com>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

IF ( NOT WIN32 )
	INCLUDE( UsePkgConfig )
	# Take care about gconf-2.0.pc settings
	PKGCONFIG( gconf-2.0 _gconf2_include_DIR _gconf2_link_DIR _gconf2_link_FLAGS _gconf2_cflags )
ENDIF ( NOT WIN32 )

# Look for gconf2 include dir and libraries, and take care about pkg-config first...
FIND_PATH( GCONF2_INCLUDE_DIR gconf/gconf.h 
		PATHS ${_gconf2_include_DIR} 
		PATH_SUFFIXES gconf/2 NO_DEFAULT_PATH )

FIND_PATH( GCONF2_INCLUDE_DIR gconf/gconf.h PATH_SUFFIXES gconf/2 
		PATHS
		/opt/local/include/
		/sw/include/
		/usr/local/include/
		/usr/include/ )

FIND_LIBRARY( GCONF2_LIBRARIES gconf-2 
		PATHS ${_gconf2_link_DIR} NO_DEFAULT_PATH )

FIND_LIBRARY( GCONF2_LIBRARIES gconf-2 
		PATHS
		/opt/local/lib
		/sw/lib
		/usr/lib
		/usr/local/lib
		/usr/lib64
		/usr/local/lib64
		/opt/lib64 )

# Report results
IF ( GCONF2_LIBRARIES AND GCONF2_INCLUDE_DIR )	
	SET( GCONF2_FOUND 1 )
	IF ( NOT GCONF2_FIND_QUIETLY )
		MESSAGE( STATUS "Found gconf2: ${GCONF2_LIBRARIES}" )
	ENDIF ( NOT GCONF2_FIND_QUIETLY )
ELSE ( GCONF2_LIBRARIES AND GCONF2_INCLUDE_DIR )	
	IF ( NOT GCONF2_FIND_QUIETLY )
		MESSAGE( STATUS "Could NOT find gconf2" )
	ENDIF ( NOT GCONF2_FIND_QUIETLY )
ENDIF ( GCONF2_LIBRARIES AND GCONF2_INCLUDE_DIR )	

# Hide advanced variables from CMake GUIs
MARK_AS_ADVANCED( GCONF2_LIBRARIES GCONF2_INCLUDE_DIR )

