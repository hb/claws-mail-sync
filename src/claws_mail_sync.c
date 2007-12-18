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

#include <opensync/opensync.h>
#include <opensync/opensync-plugin.h>
#include <opensync/opensync-format.h>
#include <opensync/opensync-version.h>

#include "claws_mail_sync.h"
#include "claws_mail_contact.h"
#include "claws_mail_connect.h"

#include <glib.h>
#include <string.h>

static void free_claws_mail_env(ClawsMailEnv*);

static void free_claws_mail_env(ClawsMailEnv *env)
{
	osync_trace(TRACE_ENTRY, "%s(%p)", __func__, env);
	while (env->sinks) {
		ClawsMailSinkEnv *sinkenv = env->sinks->data;

		if (sinkenv->sink)
			osync_objtype_sink_unref(sinkenv->sink);

		env->sinks = g_list_remove(env->sinks, sinkenv);
	}
	g_free(env);
	osync_trace(TRACE_EXIT, "%s", __func__);
}

static void connect(void *userdata, OSyncPluginInfo *info, OSyncContext *ctx)
{
	char *tablepath;

	osync_trace(TRACE_ENTRY, "%s(%p, %p, %p)", __func__, userdata, info, ctx);

	ClawsMailEnv *env = (ClawsMailEnv*) userdata;
	OSyncObjTypeSink *sink = osync_plugin_info_get_sink(info);
	ClawsMailSinkEnv *sinkenv = osync_objtype_sink_get_userdata(sink);

	OSyncError *error = NULL;

	/* connect to claws mail */
	if (!claws_mail_connect()) {
	  osync_context_report_error(ctx, OSYNC_ERROR_GENERIC, "Connection failed");
	  free_claws_mail_env(env);
	  return;
	}

	// load hashtable
	tablepath = g_strdup_printf("%s/hashtable.db",
			osync_plugin_info_get_configdir(info));
	sinkenv->hashtable = osync_hashtable_new(tablepath,
			osync_objtype_sink_get_name(sink), &error);
	g_free(tablepath);

	if (!sinkenv->hashtable)
		goto error;

	/* TODO: anchor stuff */
	//	osync_objtype_sink_set_slowsync(sink, TRUE);

	osync_context_report_success(ctx);

	osync_trace(TRACE_EXIT, "%s", __func__);
	return;

	error:

	osync_context_report_osyncerror(ctx, error);
	osync_trace(TRACE_EXIT_ERROR, "%s: %s", __func__, osync_error_print(&error));
	osync_error_unref(&error);
}

static void sync_done(void *userdata, OSyncPluginInfo *info, OSyncContext *ctx)
{
  osync_trace(TRACE_ENTRY, "%s(%p, %p, %p)", __func__, userdata, info, ctx);

  // TODO: Anchor stuff

  osync_context_report_success(ctx);
  osync_trace(TRACE_EXIT, "%s", __func__);
}

static void disconnect(void *userdata, OSyncPluginInfo *info, OSyncContext *ctx)
{
  OSyncObjTypeSink *sink;
  ClawsMailSinkEnv *sinkenv;

  osync_trace(TRACE_ENTRY, "%s(%p, %p, %p)", __func__, userdata, info, ctx);

  sink = osync_plugin_info_get_sink(info);
  sinkenv = osync_objtype_sink_get_userdata(sink);

  claws_mail_disconnect();

  /* Close the hashtable */
  osync_hashtable_free(sinkenv->hashtable);
  sinkenv->hashtable = NULL;

  osync_context_report_success(ctx);
  osync_trace(TRACE_EXIT, "%s", __func__);
}

static void finalize(void *userdata)
{
  ClawsMailEnv *env = (ClawsMailEnv*)userdata;
  osync_trace(TRACE_ENTRY, "%s(%p)", __func__, userdata);

  /* Free all stuff */
  free_claws_mail_env(env);
  osync_trace(TRACE_EXIT, "%s", __func__);
}

static void* initialize(OSyncPlugin *plugin, OSyncPluginInfo *info, OSyncError **error)
{
  const char *configdata = NULL;
  ClawsMailEnv *env;

  osync_trace(TRACE_ENTRY, "%s(%p, %p, %p)", __func__, plugin, info, error);

  env = osync_try_malloc0(sizeof(ClawsMailEnv), error);
  if (!env)
    goto error;
 
  env->sinks = NULL;

  /* get the config file for this plugin */
  configdata = osync_plugin_info_get_config(info);
  if (!configdata) {
    osync_error_set(error, OSYNC_ERROR_GENERIC, "Unable to get config data.");
    goto error_free_env;
  }

  osync_trace(TRACE_INTERNAL, "Config: '%s'", configdata);

  /* Process the configdata and set the options on environment */

  OSyncFormatEnv *formatenv = osync_plugin_info_get_format_env(info);

  ClawsMailSinkEnv *sinkenv = osync_try_malloc0(sizeof(ClawsMailSinkEnv), error);
  if (!sinkenv)
    goto error_free_env;

  sinkenv->sink = osync_objtype_sink_new("contact", error);

  if (!sinkenv->sink)
    goto error_free_env;

  osync_objtype_sink_add_objformat(sinkenv->sink, "vcard21");

  OSyncObjTypeSinkFunctions functions;
  memset(&functions, 0, sizeof(functions));
  functions.connect     = connect;
  functions.disconnect  = disconnect;
  functions.get_changes = claws_mail_contact_get_changes;
  functions.commit      = claws_mail_contact_commit_change;
  functions.sync_done   = sync_done;

  osync_objtype_sink_set_functions(sinkenv->sink, functions, sinkenv);
  osync_plugin_info_add_objtype(info, sinkenv->sink);

  sinkenv->objformat = osync_format_env_find_objformat(formatenv, "vcard21");

  env->sinks = g_list_append(env->sinks, sinkenv);

  osync_trace(TRACE_EXIT, "%s: %p", __func__, env);
  return (void *) env;
  
 error_free_env:
  
  free_claws_mail_env(env);

 error:
  osync_trace(TRACE_EXIT_ERROR, "%s: %s", __func__, osync_error_print(error));
  return NULL;
}

/* Here we actually tell opensync which sinks are available. */
static osync_bool discover(void *userdata, OSyncPluginInfo *info, OSyncError **error)
{
  osync_trace(TRACE_ENTRY, "%s(%p, %p, %p)", __func__, userdata, info, error);

  ClawsMailEnv *env = (ClawsMailEnv*)userdata;

  /* Report avaliable sinks */
  GList *list = NULL;
  for(list = env->sinks; list; list = list->next) {
    ClawsMailSinkEnv *sinkenv = list->data;
    osync_objtype_sink_set_available(sinkenv->sink, TRUE);
  }

  OSyncVersion *version = osync_version_new(error);
  osync_version_set_plugin(version, "claws-mail-sync");
  //osync_version_set_version(version, "version");
  //osync_version_set_modelversion(version, "version");
  //osync_version_set_firmwareversion(version, "firmwareversion");
  //osync_version_set_softwareversion(version, "softwareversion");
  //osync_version_set_hardwareversion(version, "hardwareversion");
  osync_plugin_info_set_version(info, version);
  osync_version_unref(version);

  osync_trace(TRACE_EXIT, "%s", __func__);
  return TRUE;
}

osync_bool get_sync_info(OSyncPluginEnv *env, OSyncError **error)
{
  osync_trace(TRACE_ENTRY, "%s(%p)", __func__, error);

  OSyncPlugin *plugin = osync_plugin_new(error);
  if (!plugin)
    goto error;

  osync_plugin_set_name(plugin, "claws-mail-sync");
  osync_plugin_set_longname(plugin, "Claws Mail -- it bites!");
  osync_plugin_set_description(plugin,
			       "Synchronize with the addressbook "
			       "of the Claws Mail email client.");

  osync_plugin_set_initialize(plugin, initialize);
  osync_plugin_set_finalize(plugin, finalize);
  osync_plugin_set_discover(plugin, discover);

  osync_plugin_env_register_plugin(env, plugin);
  osync_plugin_unref(plugin);

  osync_trace(TRACE_EXIT, "%s", __func__);

  return TRUE;
  
 error:
  osync_trace(TRACE_EXIT_ERROR, "%s: %s", __func__, osync_error_print(error));
  osync_error_unref(error);
  return FALSE;
}

int get_version(void)
{
  return 1;
}
