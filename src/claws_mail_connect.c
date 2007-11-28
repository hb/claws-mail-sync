/* Claws-Mail plugin for OpenSync
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

static char*    get_socket_name(void);
static gboolean sock_send(int, char*);
static int      sock_write_all(int, const char*, int);
static char*    sock_get_next_line(int);


static int uxsock = -1;

/* Connect to Claws-Mail unix domain socket.
 * Returns TRUE if successful, FALSE on error. */
gboolean claws_mail_connect(void)
{
	gchar *path;
	struct sockaddr_un addr;

	osync_trace(TRACE_ENTRY, "%s", __func__);

	path = get_socket_name();

	uxsock = socket(PF_UNIX, SOCK_STREAM, 0);
	if(uxsock < 0) {
		osync_trace(TRACE_INTERNAL,"Claws-Mail connect error: Could not open socket.");
		return FALSE;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

	if(connect(uxsock,(struct sockaddr *)&addr, sizeof(addr)) < 0) {
		close(uxsock);
		uxsock = -1;
		osync_trace(TRACE_INTERNAL,"Claws-Mail connect error: Could not connect to "
		            "Claws-Mail. Check that Claws-Mail is running and the OpenSync plugin is "
		            "loaded.");
		return FALSE;
	}

	osync_trace(TRACE_EXIT, "%s", __func__);

	return TRUE;
}

void claws_mail_disconnect(void)
{
	osync_trace(TRACE_ENTRY, "%s", __func__);
	if(!sock_send(uxsock, ":done:"))
		osync_trace(TRACE_INTERNAL,"failed to send :done:");
	if(uxsock != -1) {
		close(uxsock);
		uxsock = -1;
	}
	osync_trace(TRACE_EXIT, "%s", __func__);
}

static char* get_socket_name(void)
{
	static char *filename = NULL;

	if(filename == NULL) {
		filename = g_strdup_printf("%s%cclaws-mail-opensync-%d",
		                           g_get_tmp_dir(), G_DIR_SEPARATOR,
#if HAVE_GETUID
		                           getuid()
#else
		                           0
#endif
		                          );
	}

	return filename;
}

static int sock_write_all(int fd, const char *buf, int len)
{
	int n, wrlen = 0;

	while(len) {
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
	if(bytes_written != bytes_to_write)
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
		if((n = recv(fd, bp, len, MSG_PEEK)) <= 0)
			return NULL;
		else {
			if((newline = memchr(bp, '\n', n)) != NULL)
				n = newline - bp + 1;
			if((n = read(fd, bp, n)) < 0)
				return NULL;
			bp += n;
			len -= n;
		}
	} while(!newline && len);
	nl = strchr(buf,'\n');
	if(nl)
		*(nl+1) = '\0';
	return buf;
}

/* Returns the next contact in a freshly allocated string
 * (which must be freed by the caller) or NULL if no more
 * contacts exist. */
gchar* claws_mail_connect_get_next_contact(void)
{
	static gboolean is_receiving = FALSE;
	char *line;
	char *vcard;
	char *vcard_tmp;
	gboolean complete = FALSE;

	osync_trace(TRACE_ENTRY, "%s", __func__);

	if(!is_receiving) {
		if(!sock_send(uxsock, ":request_contacts:\n"))
			return NULL;
		is_receiving = TRUE;
	}

	vcard = g_strdup("");
	do {
		line = sock_get_next_line(uxsock);

		if(line) {

			if(g_str_has_prefix(line,":contacts_done:")) {
				is_receiving = FALSE;
				g_free(vcard);
				return NULL;
			}

			vcard_tmp = vcard;
			vcard = g_strconcat(vcard_tmp,line,NULL);
			g_free(vcard_tmp);

			if(g_str_has_prefix(line, "END:VCARD"))
				complete = TRUE;
		}
	} while(!complete && line);

	osync_trace(TRACE_EXIT, "%s(%s)", __func__,vcard);

	return vcard;
}
