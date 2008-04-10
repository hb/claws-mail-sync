link_directories(${GLIB2_LIBRARY_DIRS} ${LIBXML2_LIBRARY_DIRS}) 
include_directories(${OPENSYNC_INCLUDE_DIR} ${GLIB2_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIRS})

# claws-mail-sync sources
set( claws_mail_sync_LIB_SRCS
	claws_mail_sync.c
	claws_mail_connect.c
	claws_mail_contact.c
	claws_mail_event.c
	)

OPENSYNC_PLUGIN_ADD(claws-mail-sync ${claws_mail_sync_LIB_SRCS})

TARGET_LINK_LIBRARIES(claws-mail-sync ${OPENSYNC_LIBRARY})

OPENSYNC_PLUGIN_CONFIG(claws-mail-sync)
OPENSYNC_PLUGIN_DESCRIPTIONS(claws-mail-descriptions.xml)
OPENSYNC_PLUGIN_CAPABILITIES(claws-mail-capabilities-default.xml)


OPENSYNC_PLUGIN_INSTALL(claws-mail-sync)
