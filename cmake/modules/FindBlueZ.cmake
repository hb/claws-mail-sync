# - Try to find BlueZ
# Find BlueZ headers, libraries and the answer to all questions.
#
#  BLUEZ_FOUND               True if BlueZ libraries got found
#  BLUEZ_INCLUDE_DIR         Location of BlueZ headers 
#  BLUEZ_LIBRARIES           List of libaries to use BlueZ
#  BLUEZ_DEFINITIONS         Definitions to compile BlueZ 
#
# Copyright (c) 2007 Daniel Gollub <dgollub@suse.de>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

INCLUDE( UsePkgConfig )
# Take care about bluez.pc settings
PKGCONFIG( bluez _bluez_include_DIR _bluez_link_DIR _bluez_link_FLAGS _bluez_cflags )

# Look for BlueZ include dir and libraries, and take care about pkg-config first...
FIND_PATH( BLUEZ_INCLUDE_DIR bluetooth/bluetooth.h PATHS ${_bluez_include_DIR} NO_DEFAULT_PATH )
FIND_PATH( BLUEZ_INCLUDE_DIR bluetooth/bluetooth.h )

FIND_LIBRARY( BLUEZ_LIBRARIES bluetooth PATHS ${_bluez_link_DIR} NO_DEFAULT_PATH )
FIND_LIBRARY( BLUEZ_LIBRARIES bluetooth )

# Report results
IF ( BLUEZ_LIBRARIES AND BLUEZ_INCLUDE_DIR )	
	SET( BLUEZ_FOUND 1 )
	IF ( NOT BLUEZ_FIND_QUIETLY )
		MESSAGE( STATUS "Found BlueZ: ${BLUEZ_LIBRARIES}" )
	ENDIF ( NOT BLUEZ_FIND_QUIETLY )
ELSE ( BLUEZ_LIBRARIES AND BLUEZ_INCLUDE_DIR )	
	IF ( NOT BLUEZ_FIND_QUIETLY )
		MESSAGE( STATUS "Could NOT find BlueZ" )
	ENDIF ( NOT BLUEZ_FIND_QUIETLY )
ENDIF ( BLUEZ_LIBRARIES AND BLUEZ_INCLUDE_DIR )	

# Hide advanced variables from CMake GUIs
MARK_AS_ADVANCED( BLUEZ_LIBRARIES BLUEZ_INCLUDE_DIR )

