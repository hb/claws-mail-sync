/* Claws Mail plugin for OpenSync
 * Copyright (C) 2007 Holger Berndt
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "claws_mail_connect.h"

#include <opensync/opensync.h>

#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

static char* get_socket_name(void);
static gboolean sock_send(int, char*);
static int sock_write_all(int, const char*, int);
static char* sock_get_next_line(int);

static int uxsock = -1;

/* Connect to Claws Mail unix domain socket.
 * Returns TRUE if successful, FALSE on error. */
gboolean claws_mail_connect(void)
{
	gchar *path;
	struct sockaddr_un addr;

	osync_trace(TRACE_ENTRY, "%s", __func__);

	path = get_socket_name();

	uxsock = socket(PF_UNIX, SOCK_STREAM, 0);
	if (uxsock < 0) {
		osync_trace(TRACE_INTERNAL,
				"Claws Mail connect error: Could not open socket.");
		g_free(path);
		return FALSE;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);
	g_free(path);
	if(connect(uxsock,(struct sockaddr *)&addr, sizeof(addr)) < 0) {
		close(uxsock);
		uxsock = -1;
		osync_trace(TRACE_INTERNAL,"Claws Mail connect error: Could not connect to "
		"Claws Mail. Check that Claws Mail is running and the OpenSync plugin is "
		"loaded.");
		return FALSE;
	}

	osync_trace(TRACE_EXIT, "%s", __func__);

	return TRUE;
}

void claws_mail_disconnect(void)
{
	osync_trace(TRACE_ENTRY, "%s", __func__);
	if (!sock_send(uxsock, ":finished:"))
		osync_trace(TRACE_INTERNAL, "failed to send :finished:");
	if (uxsock != -1) {
		close(uxsock);
		uxsock = -1;
	}
	osync_trace(TRACE_EXIT, "%s", __func__);
}

static char* get_socket_name(void)
{
	static char *filename = NULL;

	if (filename == NULL) {
		filename = g_strdup_printf("%s%cclaws-mail-opensync-%d", g_get_tmp_dir(),
				G_DIR_SEPARATOR, getuid());
	}

	return filename;
}

static int sock_write_all(int fd, const char *buf, int len)
{
	int n, wrlen = 0;

	while (len) {
		n = write(fd, buf, len);
		if (n <= 0) {
			g_print("Error writing on fd%d: %s\n", fd, strerror(errno));
			return -1;
		}
		len -= n;
		wrlen += n;
		buf += n;
	}
	return wrlen;
}

static gboolean sock_send(int fd, char *msg)
{
	int bytes_to_write, bytes_written;

	bytes_to_write = strlen(msg);
	bytes_written = sock_write_all(fd, msg, bytes_to_write);
	if (bytes_written != bytes_to_write)
		return FALSE;
	else
		return TRUE;
}

static char* sock_get_next_line(int fd)
{
	int n;
	static char buf[BUFFSIZE];
	char *newline, *bp, *nl;
	int len;

	len = BUFFSIZE-1;
	bp = buf;
	do {
		if ((n = recv(fd, bp, len, MSG_PEEK)) <= 0)
			return NULL;
		else {
			if ((newline = memchr(bp, '\n', n)) != NULL)
				n = newline - bp + 1;
			if ((n = read(fd, bp, n)) < 0)
				return NULL;
			bp += n;
			len -= n;
		}
	} while (!newline && len);
	nl = strchr(buf, '\n');
	if (nl)
		*(nl+1) = '\0';
	return buf;
}

gchar* claws_mail_connect_get_contacts(void)
{
	gchar *vcard;
	static gboolean is_receiving = FALSE;

	if (!is_receiving) {
		if (!sock_send(uxsock, ":request_contacts:\n"))
			return NULL;
		is_receiving = TRUE;
	}

	vcard = claws_mail_connect_get_next_contact();
	if (!vcard)
		is_receiving = FALSE;
	return vcard;
}

/* Returns the next contact in a freshly allocated string
 * (which must be freed by the caller) or NULL if no more
 * contacts exist. */
gchar* claws_mail_connect_get_next_contact(void)
{
	char *line;
	char *vcard;
	char *vcard_tmp;
	gboolean complete = FALSE;

	osync_trace(TRACE_ENTRY, "%s", __func__);

	vcard = g_strdup("");
	while (!complete && ((line = sock_get_next_line(uxsock)) != NULL)) {
		if (g_str_has_prefix(line, ":done:")) {
			g_free(vcard);
			vcard = NULL;
			break;
		}
		else if(g_str_has_prefix(line, ":failure:")) {
			g_free(vcard);
			vcard = NULL;
			break;
		}
		else if (g_str_has_prefix(line, ":start_contact:")) {
			continue;
		}
		else if (g_str_has_prefix(line, ":end_contact:")) {
			complete = TRUE;
			continue;
		}

		/* append line to vcard string */
		vcard_tmp = vcard;
		vcard = g_strconcat(vcard_tmp, line, NULL);
		g_free(vcard_tmp);
	}

	osync_trace(TRACE_EXIT, "%s", __func__);

	return vcard;
}

gboolean claws_mail_connect_modify_contact(gchar *uid,
																					 gchar *vcard)
{
	gchar *msg;
	gboolean retVal;
	char *line;

	if (!sock_send(uxsock, ":modify_contact:\n"))
		return FALSE;

	msg = g_strdup_printf("%s\n", uid);
	retVal = sock_send(uxsock, msg);
	g_free(msg);
	if (!retVal)
		return FALSE;

	retVal = sock_send(uxsock, vcard);
	if(!retVal)
		return FALSE;

	if (!sock_send(uxsock, ":done:\n"))
		return FALSE;

	line = sock_get_next_line(uxsock);
	retVal = FALSE;
	if (line && g_str_has_prefix(line, ":ok:"))
		retVal = TRUE;

	return retVal;
}

gboolean claws_mail_connect_delete_contact(gchar *uid)
{
	gchar *msg;
	gboolean retVal;
	char *line;

	if (!sock_send(uxsock, ":delete_contact:\n"))
		return FALSE;

	msg = g_strdup_printf("%s\n", uid);
	retVal = sock_send(uxsock, msg);
	g_free(msg);
	if (!retVal)
		return FALSE;

	line = sock_get_next_line(uxsock);
	retVal = FALSE;
	if (line && g_str_has_prefix(line, ":ok:"))
		retVal = TRUE;

	return retVal;
}

gchar* claws_mail_connect_add_contact(gchar *vcard)
{
	gchar *msg;
	gboolean retVal;
	char *line;

	if (!sock_send(uxsock, ":add_contact:\n"))
		return NULL;

	if (!sock_send(uxsock, ":start_contact:\n"))
		return NULL;

	msg = g_strdup_printf("%s\n", vcard);
	retVal = sock_send(uxsock, msg);
	g_free(msg);
	if (!retVal)
		return NULL;

	if (!sock_send(uxsock, ":end_contact:\n"))
		return NULL;

	return claws_mail_connect_get_next_contact();
}

gchar* claws_mail_connect_get_events(void)
{
	gchar *vevent;
	static gboolean is_receiving = FALSE;

	if (!is_receiving) {
		if (!sock_send(uxsock, ":request_events:\n"))
			return NULL;
		is_receiving = TRUE;
	}

	vevent = claws_mail_connect_get_next_event();
	if(!vevent)
		is_receiving = FALSE;
	return vevent;
}

gchar* claws_mail_connect_get_next_event(void)
{
	char *line;
	char *vevent;
	char *vevent_tmp;
	gboolean complete = FALSE;

	osync_trace(TRACE_ENTRY, "%s", __func__);

	vevent = g_strdup("");
	while(!complete && ((line = sock_get_next_line(uxsock)) != NULL)) {
		if (g_str_has_prefix(line, ":done:")) {
			g_free(vevent);
			vevent = NULL;
			break;
		}
		else if(g_str_has_prefix(line, ":failure:")) {
			g_free(vevent);
			vevent = NULL;
			break;
		}
		else if (g_str_has_prefix(line, ":start_event:")) {
			continue;
		}
		else if (g_str_has_prefix(line, ":end_event:")) {
			complete = TRUE;
			continue;
		}

		/* append line to vcard string */
		vevent_tmp = vevent;
		vevent = g_strconcat(vevent_tmp, line, NULL);
		g_free(vevent_tmp);
	}

	osync_trace(TRACE_EXIT, "%s", __func__);

	return vevent;
}

gboolean claws_mail_connect_modify_event(gchar *uid,
																				 gchar *vevent)
{
	gchar *msg;
	gboolean retVal;
	char *line;
	
	if (!sock_send(uxsock, ":modify_event:\n"))
		return FALSE;

	msg = g_strdup_printf("%s\n", uid);
	retVal = sock_send(uxsock, msg);
	g_free(msg);
	if (!retVal)
		return FALSE;

	retVal = sock_send(uxsock, vevent);
	if(!retVal)
		return FALSE;

	if (!sock_send(uxsock, ":done:\n"))
		return FALSE;

	line = sock_get_next_line(uxsock);
	retVal = FALSE;
	if (line && g_str_has_prefix(line, ":ok:"))
		retVal = TRUE;

	return retVal;
}

gboolean claws_mail_connect_delete_event(gchar *uid)
{
	gchar *msg;
	gboolean retVal;
	char *line;

	if (!sock_send(uxsock, ":delete_event:\n"))
		return FALSE;

	msg = g_strdup_printf("%s\n", uid);
	retVal = sock_send(uxsock, msg);
	g_free(msg);
	if (!retVal)
		return FALSE;

	line = sock_get_next_line(uxsock);
	retVal = FALSE;
	if (line && g_str_has_prefix(line, ":ok:"))
		retVal = TRUE;

	return retVal;
}

gchar* claws_mail_connect_add_event(gchar *vevent)
{
	gchar *msg;
	gboolean retVal;
	char *line;
	
	if(!sock_send(uxsock, ":add_event:\n"))
		return NULL;
	
	if (!sock_send(uxsock, ":start_event:\n"))
		return NULL;

	msg = g_strdup_printf("%s\n", vevent);
	retVal = sock_send(uxsock, msg);
	g_free(msg);
	if (!retVal)
		return NULL;

	if (!sock_send(uxsock, ":end_event:\n"))
		return NULL;
	
	return claws_mail_connect_get_next_event();
}
