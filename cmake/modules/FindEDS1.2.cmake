# - Try to find evolution-data-server components
# Find evolution-data-server 1.2 headers, libraries and the answer to all questions.
#
#  EDS1.2_FOUND			  True if all components got found
#
#  LIBEBOOK1.2_FOUND               True if libebook1.2 got found
#  LIBEBOOK1.2_INCLUDE_DIR         Location of libebook1.2 headers 
#  LIBEBOOK1.2_LIBRARIES           List of libaries to use libebook1.2
#  LIBEBOOK1.2_DEFINITIONS         Definitions to compile libebook1.2 
#
#  LIBECAL1.2_FOUND               True if libecal1.2 got found
#  LIBECAL1.2_INCLUDE_DIR         Location of libecal1.2 headers 
#  LIBECAL1.2_LIBRARIES           List of libaries to use libecal1.2
#  LIBECAL1.2_DEFINITIONS         Definitions to compile libecal1.2 
#
#  LIBEDATABOOK1.2_FOUND               True if libedata-book1.2 got found
#  LIBEDATABOOK1.2_INCLUDE_DIR         Location of libedata-book1.2 headers 
#  LIBEDATABOOK1.2_LIBRARIES           List of libaries to use libedata-book1.2
#  LIBEDATABOOK1.2_DEFINITIONS         Definitions to compile libedata-book1.2 
#
#  LIBEDATACAL1.2_FOUND               True if libedata-cal1.2 got found
#  LIBEDATACAL1.2_INCLUDE_DIR         Location of libedata-cal1.2 headers 
#  LIBEDATACAL1.2_LIBRARIES           List of libaries to use libedata-cal1.2
#  LIBEDATACAL1.2_DEFINITIONS         Definitions to compile libedata-cal1.2 
#
#  LIBEDATASERVER1.2_FOUND               True if libedataserver1.2 got found
#  LIBEDATASERVER1.2_INCLUDE_DIR         Location of libedataserver1.2 headers 
#  LIBEDATASERVER1.2_LIBRARIES           List of libaries to use libedataserver1.2
#  LIBEDATASERVER1.2_DEFINITIONS         Definitions to compile libedataserver1.2 
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
	# Take care about evolution-data-server-1.2.pc settings
	PKGCONFIG( evolution-data-server-1.2 _evolution-data-server-1.2_include_DIR _evolution-data-server-1.2_link_DIR _evolution-data-server-1.2_link_FLAGS _evolution-data-server-1.2_cflags )
	# Take care about libebook-1.2.pc settings
	PKGCONFIG( libebook-1.2 _libebook-1.2_include_DIR _libebook-1.2_link_DIR _libebook-1.2_link_FLAGS _libebook-1.2_cflags )
	# Take care about libecal-1.2.pc settings
	PKGCONFIG( libecal-1.2 _libecal-1.2_include_DIR _libecal-1.2_link_DIR _libecal-1.2_link_FLAGS _libecal-1.2_cflags )
	# Take care about libedata-book-1.2.pc settings
	PKGCONFIG( libedata-book-1.2 _libedata-book-1.2_include_DIR _libedata-book-1.2_link_DIR _libedata-book-1.2_link_FLAGS _libedata-book-1.2_cflags )
	# Take care about libedata-cal-1.2.pc settings
	PKGCONFIG( libedata-cal-1.2 _libedata-cal-1.2_include_DIR _libedata-cal-1.2_link_DIR _libedata-cal-1.2_link_FLAGS _libedata-cal-1.2_cflags )
	# Take care about libedataserver-1.2.pc settings
	PKGCONFIG( libedataserver-1.2 _libedataserver-1.2_include_DIR _libedataserver-1.2_link_DIR _libedataserver-1.2_link_FLAGS _libedataserver-1.2_cflags )
ENDIF ( NOT WIN32 )

# Look for libebook1.2 include dir and libraries, and take care about pkg-config first...
FIND_PATH( LIBEBOOK1.2_INCLUDE_DIR
		NAMES libebook/e-book.h
		PATHS ${_libebook-1.2_include_DIR}
		PATH_SUFFIXES evolution-data-server-1.12 evolution-data-server-1.10 evolution-data-server-1.8 evolution-data-server-1.6 evolution-data-server-1.4 evolution-data-server-1.2 
		NO_DEFAULT_PATH )

FIND_PATH( LIBEBOOK1.2_INCLUDE_DIR
		NAMES libebook/e-book.h
		PATH_SUFFIXES evolution-data-server-1.12 evolution-data-server-1.10 evolution-data-server-1.8 evolution-data-server-1.6 evolution-data-server-1.4 evolution-data-server-1.2 
		PATHS
		/opt/local/include/
		/sw/include/
		/usr/local/include/
		/usr/include/ )

FIND_LIBRARY( LIBEBOOK1.2_LIBRARIES 
		NAMES ebook-1.2
		PATHS ${_libebook-1.2_link_DIR}
		NO_DEFAULT_PATH )

FIND_LIBRARY( LIBEBOOK1.2_LIBRARIES
		NAMES ebook-1.2
		PATHS
		/opt/local/lib
		/sw/lib
		/usr/lib
		/usr/local/lib
		/usr/lib64
		/usr/local/lib64
		/opt/lib64 )

# Look for libecal1.2 include dir and libraries, and take care about pkg-config first...
FIND_PATH( LIBECAL1.2_INCLUDE_DIR
		NAMES libecal/e-cal.h
		PATHS ${_libecal-1.2_include_DIR}
		PATH_SUFFIXES evolution-data-server-1.12 evolution-data-server-1.10 evolution-data-server-1.8 evolution-data-server-1.6 evolution-data-server-1.4 evolution-data-server-1.2 NO_DEFAULT_PATH )

FIND_PATH( LIBECAL1.2_INCLUDE_DIR
		NAMES libecal/e-cal.h
		PATH_SUFFIXES evolution-data-server-1.12 evolution-data-server-1.10 evolution-data-server-1.8 evolution-data-server-1.6 evolution-data-server-1.4 evolution-data-server-1.2 
		PATHS
		/opt/local/include/
		/sw/include/
		/usr/local/include/
		/usr/include/ )

FIND_LIBRARY( LIBECAL1.2_LIBRARIES 
		NAMES ecal-1.2
		PATHS ${_libecal-1.2_link_DIR}
		NO_DEFAULT_PATH )

FIND_LIBRARY( LIBECAL1.2_LIBRARIES
		NAMES ecal-1.2
		PATHS
		/opt/local/lib
		/sw/lib
		/usr/lib
		/usr/local/lib
		/usr/lib64
		/usr/local/lib64
		/opt/lib64 )

# Look for libedata-book-1.2 include dir and libraries, and take care about pkg-config first...
FIND_PATH( LIBEDATABOOK1.2_INCLUDE_DIR
		NAMES libedata-book/e-data-book.h
		PATHS ${_libedata-book-1.2_include_DIR}
		PATH_SUFFIXES evolution-data-server-1.12 evolution-data-server-1.10 evolution-data-server-1.8 evolution-data-server-1.6 evolution-data-server-1.4 evolution-data-server-1.2 NO_DEFAULT_PATH )

FIND_PATH( LIBEDATABOOK1.2_INCLUDE_DIR
		NAMES libedata-book/e-data-book.h
		PATH_SUFFIXES evolution-data-server-1.12 evolution-data-server-1.10 evolution-data-server-1.8 evolution-data-server-1.6 evolution-data-server-1.4 evolution-data-server-1.2 
		PATHS
		/opt/local/include/
		/sw/include/
		/usr/local/include/
		/usr/include/ )

FIND_LIBRARY( LIBEDATABOOK1.2_LIBRARIES 
		NAMES edata-book-1.2
		PATHS ${_libedata-book-1.2_link_DIR}
		NO_DEFAULT_PATH )

FIND_LIBRARY( LIBEDATABOOK1.2_LIBRARIES
		NAMES edata-book-1.2
		PATHS
		/opt/local/lib
		/sw/lib
		/usr/lib
		/usr/local/lib
		/usr/lib64
		/usr/local/lib64
		/opt/lib64 )

# Look for libedata-cal-1.2 include dir and libraries, and take care about pkg-config first...
FIND_PATH( LIBEDATACAL1.2_INCLUDE_DIR
		NAMES libedata-cal/e-data-cal.h
		PATHS ${_libedata-cal-1.2_include_DIR}
		PATH_SUFFIXES evolution-data-server-1.12 evolution-data-server-1.10 evolution-data-server-1.8 evolution-data-server-1.6 evolution-data-server-1.4 evolution-data-server-1.2 NO_DEFAULT_PATH )

FIND_PATH( LIBEDATACAL1.2_INCLUDE_DIR
		NAMES libedata-cal/e-data-cal.h
		PATH_SUFFIXES evolution-data-server-1.12 evolution-data-server-1.10 evolution-data-server-1.8 evolution-data-server-1.6 evolution-data-server-1.4 evolution-data-server-1.2 
		PATHS
		/opt/local/include/
		/sw/include/
		/usr/local/include/
		/usr/include/ )

FIND_LIBRARY( LIBEDATACAL1.2_LIBRARIES 
		NAMES edata-cal-1.2
		PATHS ${_libedata-cal-1.2_link_DIR} 
		NO_DEFAULT_PATH )

FIND_LIBRARY( LIBEDATACAL1.2_LIBRARIES
		NAMES edata-cal-1.2
		PATHS
		/opt/local/lib
		/sw/lib
		/usr/lib
		/usr/local/lib
		/usr/lib64
		/usr/local/lib64
		/opt/lib64 )

# Look for libedataserver-1.2 include dir and libraries, and take care about pkg-config first...
FIND_PATH( LIBEDATASERVER1.2_INCLUDE_DIR
		NAMES libedataserver/e-data-server-module.h 
		PATHS ${_libedataserver-1.2_include_DIR} 
		PATH_SUFFIXES evolution-data-server-1.12 evolution-data-server-1.10 evolution-data-server-1.8 evolution-data-server-1.6 evolution-data-server-1.4 evolution-data-server-1.2 NO_DEFAULT_PATH )

FIND_PATH( LIBEDATASERVER1.2_INCLUDE_DIR
		NAMES libedataserver/e-data-server-module.h 
		PATH_SUFFIXES evolution-data-server-1.12 evolution-data-server-1.10 evolution-data-server-1.8 evolution-data-server-1.6 evolution-data-server-1.4 evolution-data-server-1.2 
		PATHS
		/opt/local/include/
		/sw/include/
		/usr/local/include/
		/usr/include/ )

FIND_LIBRARY( LIBEDATASERVER1.2_LIBRARIES 
		NAMES edataserver-1.2
		PATHS ${_libedataserver-1.2_link_DIR} 
		NO_DEFAULT_PATH )

FIND_LIBRARY( LIBEDATASERVER1.2_LIBRARIES
		NAMES edataserver-1.2
		PATHS
		/opt/local/lib
		/sw/lib
		/usr/lib
		/usr/local/lib
		/usr/lib64
		/usr/local/lib64
		/opt/lib64 )

# Report results
IF ( LIBEBOOK1.2_LIBRARIES AND LIBEBOOK1.2_INCLUDE_DIR )	
	SET( LIBEBOOK1.2_FOUND 1 )
	IF ( NOT LIBEBOOK1.2_FIND_QUIETLY )
		MESSAGE( STATUS "Found libebook-1.2: ${LIBEBOOK1.2_LIBRARIES}" )
	ENDIF ( NOT LIBEBOOK1.2_FIND_QUIETLY )
	SET( EDS1.2_FOUND 1 )
ELSE ( LIBEBOOK1.2_LIBRARIES AND LIBEBOOK1.2_INCLUDE_DIR )	
	IF ( NOT LIBEBOOK1.2_FIND_QUIETLY )
		MESSAGE( STATUS "Could NOT find libebook-1.2" )
	ENDIF ( NOT LIBEBOOK1.2_FIND_QUIETLY )
	SET( EDS1.2_FOUND 0 )
ENDIF ( LIBEBOOK1.2_LIBRARIES AND LIBEBOOK1.2_INCLUDE_DIR )	

IF ( LIBECAL1.2_LIBRARIES AND LIBECAL1.2_INCLUDE_DIR )	
	SET( LIBECAL1.2_FOUND 1 )
	IF ( NOT LIBECAL1.2_FIND_QUIETLY )
		MESSAGE( STATUS "Found libecal-1.2: ${LIBECAL1.2_LIBRARIES}" )
	ENDIF ( NOT LIBECAL1.2_FIND_QUIETLY )
ELSE ( LIBECAL1.2_LIBRARIES AND LIBECAL1.2_INCLUDE_DIR )	
	IF ( NOT LIBECAL1.2_FIND_QUIETLY )
		MESSAGE( STATUS "Could NOT find libecal-1.2" )
	ENDIF ( NOT LIBECAL1.2_FIND_QUIETLY )
	SET( EDS1.2_FOUND 0 )
ENDIF ( LIBECAL1.2_LIBRARIES AND LIBECAL1.2_INCLUDE_DIR )	

IF ( LIBEDATABOOK1.2_LIBRARIES AND LIBEDATABOOK1.2_INCLUDE_DIR )	
	SET( LIBEDATABOOK1.2_FOUND 1 )
	IF ( NOT LIBEDATABOOK1.2_FIND_QUIETLY )
		MESSAGE( STATUS "Found libedata-book-1.2: ${LIBEDATABOOK1.2_LIBRARIES}" )
	ENDIF ( NOT LIBEDATABOOK1.2_FIND_QUIETLY )
ELSE ( LIBEDATABOOK1.2_LIBRARIES AND LIBEDATABOOK1.2_INCLUDE_DIR )	
	IF ( NOT LIBEDATABOOK1.2_FIND_QUIETLY )
		MESSAGE( STATUS "Could NOT find libedata-book-1.2" )
	ENDIF ( NOT LIBEDATABOOK1.2_FIND_QUIETLY )
	SET( EDS1.2_FOUND 0 )
ENDIF ( LIBEDATABOOK1.2_LIBRARIES AND LIBEDATABOOK1.2_INCLUDE_DIR )	

IF ( LIBEDATACAL1.2_LIBRARIES AND LIBEDATACAL1.2_INCLUDE_DIR )	
	SET( LIBEDATACAL1.2_FOUND 1 )
	IF ( NOT LIBEDATACAL1.2_FIND_QUIETLY )
		MESSAGE( STATUS "Found libedata-cal-1.2: ${LIBEDATACAL1.2_LIBRARIES}" )
	ENDIF ( NOT LIBEDATACAL1.2_FIND_QUIETLY )
ELSE ( LIBEDATACAL1.2_LIBRARIES AND LIBEDATACAL1.2_INCLUDE_DIR )	
	IF ( NOT LIBEDATACAL1.2_FIND_QUIETLY )
		MESSAGE( STATUS "Could NOT find libedata-cal-1.2" )
	ENDIF ( NOT LIBEDATACAL1.2_FIND_QUIETLY )
	SET( EDS1.2_FOUND 0 )
ENDIF ( LIBEDATACAL1.2_LIBRARIES AND LIBEDATACAL1.2_INCLUDE_DIR )	

IF ( LIBEDATASERVER1.2_LIBRARIES AND LIBEDATASERVER1.2_INCLUDE_DIR )	
	SET( LIBEDATASERVER1.2_FOUND 1 )
	IF ( NOT LIBEDATASERVER1.2_FIND_QUIETLY )
		MESSAGE( STATUS "Found libedataserver-1.2: ${LIBEDATASERVER1.2_LIBRARIES}" )
	ENDIF ( NOT LIBEDATASERVER1.2_FIND_QUIETLY )
ELSE ( LIBEDATASERVER1.2_LIBRARIES AND LIBEDATASERVER1.2_INCLUDE_DIR )	
	IF ( NOT LIBEDATASERVER1.2_FIND_QUIETLY )
		MESSAGE( STATUS "Could NOT find libedataserver-1.2" )
	ENDIF ( NOT LIBEDATASERVER1.2_FIND_QUIETLY )
	SET( EDS1.2_FOUND 0 )
ENDIF ( LIBEDATASERVER1.2_LIBRARIES AND LIBEDATASERVER1.2_INCLUDE_DIR )	

# Hide advanced variables from CMake GUIs
MARK_AS_ADVANCED( LIBEBOOK1.2_LIBRARIES LIBEBOOK1.2_INCLUDE_DIR LIBECAL1.2_LIBRARIES LIBECAL1.2_INCLUDE_DIR LIBEDATABOOK1.2_LIBRARIES LIBEDATABOOK1.2_INCLUDE_DIR LIBEDATACAL1.2_LIBRARIES LIBEDATACAL1.2_INCLUDE_DIR LIBEDATASERVER1.2_LIBRARIES LIBEDATASERVER1.2_INCLUDE_DIR )

