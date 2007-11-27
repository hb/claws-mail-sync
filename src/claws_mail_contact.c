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

#include "claws_mail_contact.h"
#include "claws_mail_sync.h"
#include "claws_mail_connect.h"

#include <glib.h>
#include <string.h>

#define VCARD_UID_STR "\nUID:"

static gchar* get_uid_from_vcard(gchar*);

/* The function commits changes of contact entries to Claws-Mail's
 * addressbook.
 * It returns TRUE on success, FALSE on error
 */
osync_bool claws_mail_contact_commit(OSyncContext *ctx, OSyncChange *change)
{
	gchar *vcard;
	const gchar *uid;
	OSyncChangeType change_type;
	OSyncError *error = NULL;

	osync_trace(TRACE_ENTRY, "%s() (%p, %p)", __func__, ctx, change);

	ClawsMailSyncEnv *env=(ClawsMailSyncEnv*)osync_context_get_plugin_data(ctx);

	vcard = osync_change_get_data(change);
	change_type = osync_change_get_changetype(change);
	uid = osync_change_get_uid(change);

	switch(change_type) {
	case CHANGE_DELETED:
		osync_trace(TRACE_INTERNAL, "Want to delete: '%s'",uid);
		break;
	case CHANGE_ADDED:
		osync_trace(TRACE_INTERNAL, "Want to add: '%s'",uid);
		break;
	case CHANGE_MODIFIED:
		osync_trace(TRACE_INTERNAL, "Want to modify: '%s'",uid);
		break;
	default:
		osync_trace(TRACE_INTERNAL, "Unknown change type");
	}

	osync_context_report_success(ctx);
	osync_hashtable_update_hash(env->hashtable, change);

	osync_trace(TRACE_EXIT, "%s", __func__);
	return TRUE;

error:
	osync_context_report_osyncerror(ctx, &error);
	osync_trace(TRACE_EXIT_ERROR, "%s: %s", __func__, osync_error_print(&error));
	osync_error_free(&error);
	return FALSE;
}

/* The function gets all contact entries and checks for
 * changes by comparing old hash with new hash.
 * It returns TRUE on success, FALSE on error
 */
osync_bool claws_mail_contact_get_changeinfo(OSyncContext *ctx)
{
	gchar *vcard;

	osync_trace(TRACE_ENTRY, "%s(%p)", __func__, ctx);

	ClawsMailSyncEnv *env=(ClawsMailSyncEnv*)osync_context_get_plugin_data(ctx);

	// check for slowsync and prepare the "contact" hashtable if needed
	if (osync_member_get_slow_sync(env->member, "contact") == TRUE) {
		osync_trace(TRACE_INTERNAL, "slow sync");
		osync_hashtable_set_slow_sync(env->hashtable, "contact");
	}

	/* While getting all contacts, one at a time */
	while((vcard = claws_mail_connect_get_next_contact()) != NULL) {
		OSyncChange *change = osync_change_new();
		gchar *uid;
		gchar *hash;
		guint ihash;

		osync_change_set_member(change, env->member);

		/* UID */
		uid = get_uid_from_vcard(vcard);
		osync_change_set_uid(change, uid);
		g_free(uid);

		/* hash */
		ihash = g_str_hash(vcard);
		hash = g_strdup_printf("%u",ihash);
		osync_change_set_hash(change, hash);
		g_free(hash);

		osync_change_set_objformat_string(change, "vcard21");
		osync_change_set_objtype_string(change, "contact");

		osync_change_set_data(change, vcard, strlen(vcard), TRUE);
		g_free(vcard);

		if(osync_hashtable_detect_change(env->hashtable, change)) {
			osync_context_report_change(ctx, change);
			osync_hashtable_update_hash(env->hashtable, change);
		}

	}

	osync_hashtable_report_deleted(env->hashtable, ctx, "contact");

	osync_trace(TRACE_EXIT, "%s()", __func__);
	return TRUE;
}

static gchar* get_uid_from_vcard(gchar *vcard)
{
	gchar uid[BUFFSIZE];
	gchar *start;
	int ii;

	start = strstr(vcard,VCARD_UID_STR);
	if(!start) {
		osync_trace(TRACE_INTERNAL, "Claws-Mail: Contact doesn't have a UID.");
		return g_strdup("123");
	}

	start += strlen(VCARD_UID_STR);
	ii = 0;
	while(start && *start && (*start != '\n') && (ii < BUFFSIZE-1))
		uid[ii++] = *start++;
	uid[ii] = '\0';

	return g_strdup(uid);
}
