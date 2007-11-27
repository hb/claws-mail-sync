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

#ifndef CLAWS_MAIL_CONTACT_H
#define CLAWS_MAIL_CONTACT_H

#include <opensync/opensync.h>

osync_bool claws_mail_contact_commit(OSyncContext*, OSyncChange*);
osync_bool claws_mail_contact_get_changeinfo(OSyncContext*);

#endif /*CLAWS_MAIL_CONTACT_H*/
