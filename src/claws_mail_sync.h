/* Claws Mail plugin for OpenSync
 * Copyright (C) 2007 Holger Berndt
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CLAWSMAILSYNC_H
#define CLAWSMAILSYNC_H

#include <glib.h>

#include <opensync/opensync.h>
#include <opensync/opensync-helper.h>

typedef struct {
	OSyncMember *member;
	OSyncHashTable *hashtable;
	OSyncObjTypeSink *mainsink;
	GList *sinks;
} ClawsMailEnv;

typedef struct {
	OSyncObjTypeSink *sink;
	OSyncObjFormat *objformat;
	OSyncHashTable *hashtable;
} ClawsMailSinkEnv;

#endif /*CLAWSMAILSYNC_H*/
