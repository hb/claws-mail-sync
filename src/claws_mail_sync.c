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

#include <opensync/opensync.h>

#include "claws_mail_sync.h"
#include "claws_mail_contact.h"
#include "claws_mail_connect.h"

#include <glib.h>

static void free_claws_mail_env(ClawsMailSyncEnv*);


static void free_claws_mail_env(ClawsMailSyncEnv *env)
{
	g_free(env);
}

static void *initialize(OSyncMember *member, OSyncError **error)
{
	osync_trace(TRACE_ENTRY, "%s(%p, %p)", __func__, member, error);

	char *configdata = NULL;
	int configsize;

	ClawsMailSyncEnv *env = g_new0(ClawsMailSyncEnv,1);

	if(!osync_member_get_config(member, &configdata, &configsize, error)) {
	  osync_error_update(error, "Unable to get config data: %s",
			     osync_error_print(error));
	  free_claws_mail_env(env);
	  return NULL;
	}

	//Process the config data here and set the options on your environment
	if(configdata)
		g_free(configdata);
	env->member = member;

	// create the hashtable
	env->hashtable = osync_hashtable_new();

	osync_trace(TRACE_EXIT, "%s: %p", __func__, env);
	return (void *)env;
}

static void connect(OSyncContext *ctx)
{
	osync_trace(TRACE_ENTRY, "%s(%p)", __func__, ctx);

	ClawsMailSyncEnv *env=(ClawsMailSyncEnv*)osync_context_get_plugin_data(ctx);

	/* connect to claws mail */
	if(claws_mail_connect())
	  osync_context_report_success(ctx);
	else {
	  osync_context_report_error(ctx, OSYNC_ERROR_GENERIC, "Connection failed");
	  free_claws_mail_env(env);
	  return;
	}

	// load hashtable
	OSyncError *error = NULL;
	if(!osync_hashtable_load(env->hashtable, env->member, &error)) {
		osync_context_report_osyncerror(ctx, &error);
		return;
	}

	osync_trace(TRACE_EXIT, "%s", __func__);
}

static void get_changeinfo(OSyncContext *ctx)
{
  gboolean contact_changes = TRUE;

	osync_trace(TRACE_ENTRY, "%s(%p)", __func__, ctx);

	ClawsMailSyncEnv *env=(ClawsMailSyncEnv*)osync_context_get_plugin_data(ctx);

	// get changes of contacts
	if(osync_member_objtype_enabled(env->member, "contact"))
	  contact_changes = claws_mail_contact_get_changeinfo(ctx);

	if(contact_changes)
	  osync_context_report_success(ctx);

	osync_trace(TRACE_EXIT, "%s", __func__);
}

static void sync_done(OSyncContext *ctx)
{
	osync_trace(TRACE_ENTRY, "%s(%p)", __func__, ctx);

	ClawsMailSyncEnv *env=(ClawsMailSyncEnv*)osync_context_get_plugin_data(ctx);

	// forget reported changes
	osync_hashtable_forget(env->hashtable);

	osync_context_report_success(ctx);

	osync_trace(TRACE_EXIT, "%s", __func__);
}

static void disconnect(OSyncContext *ctx)
{
	osync_trace(TRACE_ENTRY, "%s(%p)", __func__, ctx);

	ClawsMailSyncEnv *env=(ClawsMailSyncEnv*)osync_context_get_plugin_data(ctx);

	claws_mail_disconnect();

	// close the hashtable
	osync_hashtable_close(env->hashtable);

	osync_context_report_success(ctx);
	osync_trace(TRACE_EXIT, "%s", __func__);
}

static void finalize(void *data)
{
	osync_trace(TRACE_ENTRY, "%s(%p)", __func__, data);
	ClawsMailSyncEnv *env = (ClawsMailSyncEnv*)data;

	// close and free hashtable
	osync_hashtable_close(env->hashtable);
	osync_hashtable_free(env->hashtable);

	free_claws_mail_env(env);
	osync_trace(TRACE_EXIT, "%s", __func__);
}

void get_info(OSyncEnv *env)
{
	OSyncPluginInfo *info = osync_plugin_new_info(env);

	//Tell opensync something about (y)our plugin
	info->name = "claws-mail-sync";
	info->longname = "Claws-Mail email program";
	info->description = "Sync with Claws-Mail addressbook";
	//the version of the api we are using, (1 at the moment)
	info->version = 1;

	//Now set the function we made earlier
	info->functions.initialize = initialize;
	info->functions.connect = connect;
	info->functions.sync_done = sync_done;
	info->functions.disconnect = disconnect;
	info->functions.finalize = finalize;
	info->functions.get_changeinfo = get_changeinfo;

	info->timeouts.connect_timeout = 10;

	info->timeouts.sync_done_timeout      = 60;
	info->timeouts.disconnect_timeout     = 60;
	info->timeouts.get_changeinfo_timeout = 60;
	info->timeouts.get_data_timeout       = 60;
	info->timeouts.commit_timeout         = 60;
	info->timeouts.read_change_timeout    = 60;

	osync_plugin_accept_objtype(info, "contact");
	osync_plugin_accept_objformat(info, "contact", "vcard21", NULL);
	osync_plugin_set_commit_objformat(info, "contact", "vcard21",
	                                  claws_mail_contact_commit);
}
