INCLUDE_DIRECTORIES( ${OPENSYNC_INCLUDE_DIR} ${GLIB2_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIR} )

# claws-mail-sync source	
SET( claws_mail_sync_LIB_SRCS
claws_mail_sync.c
claws_mail_connect.c
claws_mail_contact.c
)

OPENSYNC_PLUGIN_ADD( claws-mail-sync ${claws_mail_sync_LIB_SRCS} )

TARGET_LINK_LIBRARIES( claws-mail-sync ${OPENSYNC_LIBRARY} )

###### INSTALL ################### 
OPENSYNC_PLUGIN_INSTALL( claws-mail-sync ) 

OPENSYNC_PLUGIN_CONFIG( claws-mail-sync )
