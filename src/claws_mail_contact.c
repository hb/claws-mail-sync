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

#include "claws_mail_contact.h"
#include "claws_mail_sync.h"
#include "claws_mail_connect.h"

#include <opensync/opensync.h>
#include <opensync/opensync-plugin.h>
#include <opensync/opensync-data.h>

#include <glib.h>
#include <string.h>

#define VCARD_UID_STR "\nUID:"

static gchar* get_uid_from_vcard(gchar*);
static gchar* contact_hash(gchar*);

static gchar* contact_hash(gchar *vcard)
{
	guint ihash;
	gchar *hash;
	ihash = g_str_hash(vcard);
	hash = g_strdup_printf("%u", ihash);
	return hash;
}

static gchar* get_uid_from_vcard(gchar *vcard)
{
	gchar uid[BUFFSIZE];
	gchar *start;
	int ii;

	start = strstr(vcard, VCARD_UID_STR);
	if (!start) {
		osync_trace(TRACE_INTERNAL, "Claws Mail: Contact doesn't have a UID.");
		return g_strdup("123");
	}

	start += strlen(VCARD_UID_STR);
	ii = 0;
	while (start && *start && (*start != '\n')&& (ii < BUFFSIZE-1))
		uid[ii++] = *start++;
	uid[ii] = '\0';

	return g_strdup(uid);
}

void claws_mail_contact_get_changes(void *userdata, OSyncPluginInfo *info,
		OSyncContext *ctx)
{
	int ii;
	char **uids;
	char *vcard;
	ClawsMailEnv *env = (ClawsMailEnv*)userdata;

	osync_trace(TRACE_ENTRY, "%s(%p, %p, %p)", __func__, userdata, info, ctx);

	OSyncObjTypeSink *sink = osync_plugin_info_get_sink(info);
	ClawsMailSinkEnv *sinkenv = osync_objtype_sink_get_userdata(sink);

	OSyncError *error = NULL;

	/* check for slowsync and prepare the "contact" hashtable if needed */
	if (osync_objtype_sink_get_slowsync(sinkenv->sink)) {
		osync_trace(TRACE_INTERNAL, "Slow sync requested");
		osync_hashtable_reset(sinkenv->hashtable);
	}

	/* While getting all contacts, one at a time */
	while ((vcard = claws_mail_connect_get_contacts()) != NULL) {
		gchar *uid;
		gchar *hash;
		char *data;
		OSyncChangeType changetype;
		OSyncChange *change;
		OSyncData *odata;

		hash = contact_hash(vcard);
		uid = get_uid_from_vcard(vcard);

		/* Now get the data of this change */
		data = vcard;

		/* Report every entry .. every unreported entry got deleted. */
		osync_hashtable_report(sinkenv->hashtable, uid);

		changetype = osync_hashtable_get_changetype(sinkenv->hashtable, uid, hash);

		if (changetype == OSYNC_CHANGE_TYPE_UNMODIFIED) {
			g_free(hash);
			g_free(uid);
			continue;
		}

		/* Set the hash of the object */
		osync_hashtable_update_hash(sinkenv->hashtable, changetype, uid, hash);

		/* Make the new change to report */
		change = osync_change_new(&error);

		if (!change) {
			osync_context_report_osyncwarning(ctx, error);
			osync_error_unref(&error);
			continue;
		}

		/* Now set the uid of the object */
		osync_change_set_uid(change, uid);
		osync_change_set_hash(change, hash);
		osync_change_set_changetype(change, changetype);

		g_free(hash);

		odata = osync_data_new(data, strlen(data), sinkenv->objformat, &error);
		if (!odata) {
			osync_change_unref(change);
			osync_context_report_osyncwarning(ctx, error);
			osync_error_unref(&error);
			continue;
		}

		osync_data_set_objtype(odata, osync_objtype_sink_get_name(sinkenv->sink));

		/* Set the data for the object */
		osync_change_set_data(change, odata);
		osync_data_unref(odata);

		/* Report the change */
		osync_context_report_change(ctx, change);

		osync_change_unref(change);

		g_free(uid);
	}

	/* Check for deleted entries */
	uids = osync_hashtable_get_deleted(sinkenv->hashtable);

	for (ii=0; uids[ii]; ii++) {
	  OSyncData *odata;
		OSyncChange *change = osync_change_new(&error);
		if (!change) {
			g_free(uids[ii]);
			osync_context_report_osyncwarning(ctx, error);
			osync_error_unref(&error);
			continue;
		}

		osync_change_set_uid(change, uids[ii]);
		osync_change_set_changetype(change, OSYNC_CHANGE_TYPE_DELETED);

		odata = osync_data_new(NULL, 0, sinkenv->objformat, &error);
		if (!odata) {
			g_free(uids[ii]);
			osync_change_unref(change);
			osync_context_report_osyncwarning(ctx, error);
			osync_error_unref(&error);
			continue;
		}

		osync_data_set_objtype(odata, osync_objtype_sink_get_name(sinkenv->sink));
		osync_change_set_data(change, odata);
		osync_data_unref(odata);

		osync_context_report_change(ctx, change);

		osync_hashtable_update_hash(sinkenv->hashtable,
				osync_change_get_changetype(change), osync_change_get_uid(change), NULL);

		osync_change_unref(change);
		g_free(uids[ii]);
	}
	g_free(uids);

	osync_context_report_success(ctx);
	osync_trace(TRACE_EXIT, "%s", __func__);
}

void claws_mail_contact_commit_change(void *userdata, OSyncPluginInfo *info,
		OSyncContext *ctx, OSyncChange *change)
{
	gboolean retVal;
	gchar *vcard;
	char *uid;
	char *hash;
	OSyncError *error = NULL;
	ClawsMailEnv *env = (ClawsMailEnv*)userdata;

	OSyncObjTypeSink *sink = osync_plugin_info_get_sink(info);
	ClawsMailSinkEnv *sinkenv = osync_objtype_sink_get_userdata(sink);

	osync_data_get_data(osync_change_get_data(change), &vcard, NULL);

	switch (osync_change_get_changetype(change)) {

	case OSYNC_CHANGE_TYPE_DELETED:

		uid = get_uid_from_vcard(vcard);
		retVal = claws_mail_connect_delete_contact(vcard);
		g_free(uid);
		if (!retVal) {
			osync_error_set(&error, OSYNC_ERROR_GENERIC, "Unable to delete contact.");
			goto error;
		}
		break;

	case OSYNC_CHANGE_TYPE_ADDED:
		//Add the change
		// TODO: Implement adding here
		if (0) {
			osync_error_set(&error, OSYNC_ERROR_GENERIC, "Unable to write contact.");
			goto error;
		}

		uid = get_uid_from_vcard(vcard);

		osync_trace(TRACE_INTERNAL, "Want to add: '%s'", uid);

		osync_change_set_uid(change, uid);
		g_free(uid);

		/* generate and set hash of entry */
		hash = contact_hash(vcard);
		osync_change_set_hash(change, hash);
		g_free(hash);

		break;

	case OSYNC_CHANGE_TYPE_MODIFIED:

		uid = get_uid_from_vcard(vcard);
		retVal = claws_mail_connect_modify_contact(uid);
		g_free(uid);
		if (!retVal) {
			osync_error_set(&error, OSYNC_ERROR_GENERIC, "Unable to modify contact.");
			goto error;
		}
		hash = contact_hash(vcard);
		osync_change_set_hash(change, hash);
		g_free(hash);
		break;

	default:
		osync_trace(TRACE_INTERNAL, "Unknown change type");
		break;
	}

	/* Calculate the hash */
	osync_hashtable_update_hash(sinkenv->hashtable,
			osync_change_get_changetype(change), osync_change_get_uid(change),
			osync_change_get_hash(change));

	/* Answer the call */
	osync_context_report_success(ctx);

	osync_trace(TRACE_EXIT, "%s", __func__);
	return;

	error:

	osync_context_report_osyncerror(ctx, error);
	osync_trace(TRACE_EXIT_ERROR, "%s: %s", __func__, osync_error_print(&error));
	osync_error_unref(&error);
}
