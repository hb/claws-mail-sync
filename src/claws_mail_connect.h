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

#ifndef CLAWS_MAIL_CONNECT_H
#define CLAWS_MAIL_CONNECT_H

#include <glib.h>

#define BUFFSIZE 8192

gboolean claws_mail_connect(void);
void     claws_mail_disconnect(void);

gchar*   claws_mail_connect_get_contacts(void);
gchar*   claws_mail_connect_get_next_contact(void);

gchar*   claws_mail_connect_modify_contact(gchar*,gchar*);
gboolean claws_mail_connect_delete_contact(const gchar*);
gchar*   claws_mail_connect_add_contact(gchar*);

gchar*   claws_mail_connect_get_events(void);
gchar*   claws_mail_connect_get_next_event(void);

gchar*   claws_mail_connect_modify_event(gchar*,gchar*);
gboolean claws_mail_connect_delete_event(const gchar*);
gchar*   claws_mail_connect_add_event(gchar*);

#endif
