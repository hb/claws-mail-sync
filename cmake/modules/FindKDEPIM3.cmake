# - Try to find KDEPIM3
# Find KDEPIM3 headers, libraries and the answer to all questions.
#
#  KDEPIM3_FOUND               True if KDEPIM3 got found
#  KDEPIM3_INCLUDE_DIR         Location of KDEPIM3 headers 
#  KDEPIM3_KCAL_LIBRARIES      List of libaries to use KABC of KDEPIM3 
#  KDEPIM3_KABC_LIBRARIES      List of libaries to use KCAL of KDEPIM3 
#  KDEPIM3_DEFINITIONS         Definitions to compile KDEPIM3 
#
# Copyright (c) 2007 Daniel Gollub <dgollub@suse.de>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

# Look for KDEPIM3 include dir and libraries
FIND_PATH( KDEPIM3_INCLUDE_DIR kdepimmacros.h
		PATHS
		$ENV{KDEDIR}/include/
		/opt/kde3/include/
		/usr/include/kde/
		/opt/local/include/
		/sw/include/
		/usr/local/include/
		/usr/include/ )

FIND_LIBRARY( KDEPIM3_KCAL_LIBRARIES kcal 
		PATHS
		$ENV{KDEDIR}/lib64
		$ENV{KDEDIR}/lib
		/opt/kde3/lib64
		/opt/kde3/lib
		/opt/local/lib
		/sw/lib
		/usr/lib
		/usr/local/lib
		/usr/lib64
		/usr/local/lib64
		/opt/lib64 )

FIND_LIBRARY( KDEPIM3_KABC_LIBRARIES kabc
		PATHS
		$ENV{KDEDIR}/lib64
		$ENV{KDEDIR}/lib
		/opt/kde3/lib64
		/opt/kde3/lib
		/opt/local/lib
		/sw/lib
		/usr/lib
		/usr/local/lib
		/usr/lib64
		/usr/local/lib64
		/opt/lib64 )

# Report results
IF ( KDEPIM3_KCAL_LIBRARIES AND KDEPIM3_KABC_LIBRARIES AND KDEPIM3_INCLUDE_DIR )	
	SET( KDEPIM3_FOUND 1 )
	IF ( NOT KDEPIM3_FIND_QUIETLY )
		MESSAGE( STATUS "Found KDE PIM 3: ${KDEPIM3_KABC_LIBRARIES} ${KDEPIM3_KCAL_LIBRARIES}" )
	ENDIF ( NOT KDEPIM3_FIND_QUIETLY )
ELSE ( KDEPIM3_KCAL_LIBRARIES AND KDEPIM3_KABC_LIBRARIES AND KDEPIM3_INCLUDE_DIR )	
	IF ( NOT KDEPIM3_FIND_QUIETLY )
		MESSAGE( STATUS "Could NOT find KDE PIM 3" )
	ENDIF ( NOT KDEPIM3_FIND_QUIETLY )
ENDIF ( KDEPIM3_KCAL_LIBRARIES AND KDEPIM3_KABC_LIBRARIES AND KDEPIM3_INCLUDE_DIR )	

# Hide advanced variables from CMake GUIs
MARK_AS_ADVANCED( KDEPIM3_KCAL_LIBRARIES KDEPIM3_KABC_LIBRARIES KDEPIM3_INCLUDE_DIR )

