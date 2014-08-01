/*
 * Copyright 2014 Vincent Sanders <vince@netsurf-browser.org>
 *
 * This file is part of NetSurf, http://www.netsurf-browser.org/
 *
 * NetSurf is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * NetSurf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file generic data viewer implementation.
 *
 * This viewer can be used for utf-8 encoded chunk of data. Thie data
 * might be page source or the debugging of dom or box trees. It will
 * show the data in a tab, window or editor as per user configuration.
 */

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#include "utils/log.h"
#include "utils/nsoption.h"
#include "utils/utf8.h"
#include "utils/messages.h"
#include "utils/url.h"
#include "utils/utils.h"
#include "utils/file.h"
#include "desktop/netsurf.h"
#include "desktop/browser.h"
#include "render/html.h"
#include "content/hlcache.h"
#include "content/content.h"

#include "gtk/dialogs/about.h"
#include "gtk/fetch.h"
#include "gtk/compat.h"
#include "gtk/gui.h"
#include "gtk/viewdata.h"

struct nsgtk_viewdata_ctx {
	char *data;
	size_t data_len;
	char *filename;

	GtkBuilder *builder; /**< The gtk builder that built the widgets. */
	GtkWindow *window; /**< handle to gtk window (builder holds reference) */
	GtkTextView *gv; /**< handle to gtk text view (builder holds reference) */

	struct nsgtk_viewdata_ctx *next;
	struct nsgtk_viewdata_ctx *prev;
};

struct menu_events {
	const char *widget;
	GCallback handler;
};

static struct nsgtk_viewdata_ctx *nsgtk_viewdata_list = NULL;
static char viewdata_zoomlevel = 10;

#define MENUEVENT(x) { #x, G_CALLBACK(nsgtk_on_##x##_activate) }
#define MENUPROTO(x) static gboolean nsgtk_on_##x##_activate(	\
		GtkMenuItem *widget, gpointer g)

MENUPROTO(viewdata_save_as);
MENUPROTO(viewdata_print);
MENUPROTO(viewdata_close);
MENUPROTO(viewdata_select_all);
MENUPROTO(viewdata_cut);
MENUPROTO(viewdata_copy);
MENUPROTO(viewdata_paste);
MENUPROTO(viewdata_delete);
MENUPROTO(viewdata_zoom_in);
MENUPROTO(viewdata_zoom_out);
MENUPROTO(viewdata_zoom_normal);
MENUPROTO(viewdata_about);

static struct menu_events viewdata_menu_events[] = {
	MENUEVENT(viewdata_save_as),
	MENUEVENT(viewdata_print),
	MENUEVENT(viewdata_close),
	MENUEVENT(viewdata_select_all),
	MENUEVENT(viewdata_cut),
	MENUEVENT(viewdata_copy),
	MENUEVENT(viewdata_paste),
	MENUEVENT(viewdata_delete),
	MENUEVENT(viewdata_zoom_in),
	MENUEVENT(viewdata_zoom_out),
	MENUEVENT(viewdata_zoom_normal),
	MENUEVENT(viewdata_about),
	{NULL, NULL}
};

static void nsgtk_attach_viewdata_menu_handlers(GtkBuilder *xml, gpointer g)
{
	struct menu_events *event = viewdata_menu_events;

	while (event->widget != NULL)
	{
		GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(xml, event->widget));
		g_signal_connect(G_OBJECT(w), "activate", event->handler, g);
		event++;
	}
}

static gboolean nsgtk_viewdata_destroy_event(GtkBuilder *window, gpointer g)
{
	struct nsgtk_viewdata_ctx *vdctx = (struct nsgtk_viewdata_ctx *)g;

	if (vdctx->next != NULL) {
		vdctx->next->prev = vdctx->prev;
	}

	if (vdctx->prev != NULL) {
		vdctx->prev->next = vdctx->next;
	} else {
		nsgtk_viewdata_list = vdctx->next;
	}

	/* release the data */
	free(vdctx->data);

	/* free the builder */
	g_object_unref(G_OBJECT(vdctx->builder));

	/* free the context structure */
	free(vdctx);

	return FALSE;
}

static gboolean nsgtk_viewdata_delete_event(GtkWindow * window, gpointer g)
{
	return FALSE;
}



static void nsgtk_viewdata_file_save(GtkWindow *parent, const char *filename,
				     const char *data, size_t data_size)
{
	FILE *f;
	GtkWidget *notif;
	GtkWidget *label;

	f = fopen(filename, "w+");
	if (f != NULL) {
		fwrite(data, data_size, 1, f);
		fclose(f);
		return;
	}

	/* inform user of faliure */
	notif = gtk_dialog_new_with_buttons(messages_get("gtkSaveFailedTitle"),
					    parent,
					    GTK_DIALOG_MODAL, GTK_STOCK_OK,
					    GTK_RESPONSE_NONE, NULL);

	g_signal_connect_swapped(notif, "response",
				 G_CALLBACK(gtk_widget_destroy), notif);

	label = gtk_label_new(messages_get("gtkSaveFailed"));
	gtk_container_add(GTK_CONTAINER(nsgtk_dialog_get_content_area(GTK_DIALOG(notif))), label);
	gtk_widget_show_all(notif);

}


gboolean nsgtk_on_viewdata_save_as_activate(GtkMenuItem *widget, gpointer g)
{
	struct nsgtk_viewdata_ctx *nsg = (struct nsgtk_viewdata_ctx *) g;
	GtkWidget *fc;

	fc = gtk_file_chooser_dialog_new(messages_get("gtkSaveFile"),
					 nsg->window,
					 GTK_FILE_CHOOSER_ACTION_SAVE,
					 GTK_STOCK_CANCEL,
					 GTK_RESPONSE_CANCEL,
					 GTK_STOCK_SAVE,
					 GTK_RESPONSE_ACCEPT,
					 NULL);

	gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(fc), nsg->filename);

	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(fc),
						       TRUE);

	if (gtk_dialog_run(GTK_DIALOG(fc)) == GTK_RESPONSE_ACCEPT) {
		char *filename;
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fc));
		nsgtk_viewdata_file_save(nsg->window, filename, nsg->data, nsg->data_len);
		g_free(filename);
	}

	gtk_widget_destroy(fc);

	return TRUE;
}


gboolean nsgtk_on_viewdata_print_activate( GtkMenuItem *widget, gpointer g)
{
	/* correct printing */

	return TRUE;
}

gboolean nsgtk_on_viewdata_close_activate( GtkMenuItem *widget, gpointer g)
{
	struct nsgtk_viewdata_ctx *nsg = (struct nsgtk_viewdata_ctx *) g;

	gtk_widget_destroy(GTK_WIDGET(nsg->window));

	return TRUE;
}



gboolean nsgtk_on_viewdata_select_all_activate (GtkMenuItem *widget, gpointer g)
{
	struct nsgtk_viewdata_ctx *nsg = (struct nsgtk_viewdata_ctx *) g;
	GtkTextBuffer *buf = gtk_text_view_get_buffer(nsg->gv);
	GtkTextIter start, end;

	gtk_text_buffer_get_bounds(buf, &start, &end);

	gtk_text_buffer_select_range(buf, &start, &end);

	return TRUE;
}

gboolean nsgtk_on_viewdata_cut_activate(GtkMenuItem *widget, gpointer g)
{
	return TRUE;
}

gboolean nsgtk_on_viewdata_copy_activate(GtkMenuItem *widget, gpointer g)
{
	struct nsgtk_viewdata_ctx *nsg = (struct nsgtk_viewdata_ctx *) g;
	GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(nsg->gv));

	gtk_text_buffer_copy_clipboard(buf,
		gtk_clipboard_get(GDK_SELECTION_CLIPBOARD));

	return TRUE;
}

gboolean nsgtk_on_viewdata_paste_activate(GtkMenuItem *widget, gpointer g)
{
	return TRUE;
}

gboolean nsgtk_on_viewdata_delete_activate(GtkMenuItem *widget, gpointer g)
{
	return TRUE;
}

static void nsgtk_viewdata_update_zoomlevel(gpointer g)
{
	struct nsgtk_viewdata_ctx *nsg;
	GtkTextBuffer *buf;
	GtkTextTagTable *tab;
	GtkTextTag *tag;

	nsg = nsgtk_viewdata_list;
	while (nsg) {
		if (nsg->gv) {
			buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(nsg->gv));

			tab = gtk_text_buffer_get_tag_table(
				GTK_TEXT_BUFFER(buf));

			tag = gtk_text_tag_table_lookup(tab, "zoomlevel");
			if (!tag) {
				tag = gtk_text_tag_new("zoomlevel");
				gtk_text_tag_table_add(tab, GTK_TEXT_TAG(tag));
			}

			gdouble fscale = ((gdouble) viewdata_zoomlevel) / 10;

			g_object_set(GTK_TEXT_TAG(tag), "scale", fscale, NULL);

			GtkTextIter start, end;

			gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buf),
						   &start, &end);
			gtk_text_buffer_remove_all_tags(GTK_TEXT_BUFFER(buf),
							&start,	&end);
			gtk_text_buffer_apply_tag(GTK_TEXT_BUFFER(buf),
						  GTK_TEXT_TAG(tag), &start, &end);
		}
		nsg = nsg->next;
	}
}

gboolean nsgtk_on_viewdata_zoom_in_activate(GtkMenuItem *widget, gpointer g)
{
	viewdata_zoomlevel++;
	nsgtk_viewdata_update_zoomlevel(g);

	return TRUE;
}

gboolean nsgtk_on_viewdata_zoom_out_activate(GtkMenuItem *widget, gpointer g)
{
	if (viewdata_zoomlevel > 1) {
		viewdata_zoomlevel--;
		nsgtk_viewdata_update_zoomlevel(g);
	}

	return TRUE;
}


gboolean nsgtk_on_viewdata_zoom_normal_activate(GtkMenuItem *widget, gpointer g)
{
	viewdata_zoomlevel = 10;
	nsgtk_viewdata_update_zoomlevel(g);

	return TRUE;
}

gboolean nsgtk_on_viewdata_about_activate(GtkMenuItem *widget, gpointer g)
{
	struct nsgtk_viewdata_ctx *nsg = (struct nsgtk_viewdata_ctx *) g;

	nsgtk_about_dialog_init(nsg->window, netsurf_version);

	return TRUE;
}

/**
 * View the data in a gtk text window.
 */
static nserror
window_init(const char *title,
	    const char *filename,
	    char *ndata,
	    size_t ndata_len)
{
	GError* error = NULL;
	GtkWindow *window;
	GtkWidget *cutbutton;
	GtkWidget *pastebutton;
	GtkWidget *deletebutton;
	GtkWidget *printbutton;
	GtkTextView *dataview;
	PangoFontDescription *fontdesc;
	GtkTextBuffer *tb;
	struct nsgtk_viewdata_ctx *newctx;

	newctx = malloc(sizeof(struct nsgtk_viewdata_ctx));
	if (newctx == NULL) {
		return NSERROR_NOMEM;
	}

	newctx->builder = gtk_builder_new();
	if (newctx->builder == NULL) {
		free(newctx);
		return NSERROR_INIT_FAILED;
	}

	if (!gtk_builder_add_from_file(newctx->builder,
				       glade_file_location->viewdata,
				       &error)) {
		LOG(("Couldn't load builder file: %s", error->message));
		g_error_free(error);
		free(newctx);
		return NSERROR_INIT_FAILED;
	}


	window = GTK_WINDOW(gtk_builder_get_object(newctx->builder, "ViewDataWindow"));

	if (window == NULL) {
		LOG(("Unable to find window in builder "));

		/* free the builder */
		g_object_unref(G_OBJECT(newctx->builder));

		/* free the context structure */
		free(newctx);

		return NSERROR_INIT_FAILED;
	}

	cutbutton = GTK_WIDGET(gtk_builder_get_object(newctx->builder, "viewdata_cut"));
	pastebutton = GTK_WIDGET(gtk_builder_get_object(newctx->builder, "viewdata_paste"));
	deletebutton = GTK_WIDGET(gtk_builder_get_object(newctx->builder, "viewdata_delete"));
	printbutton = GTK_WIDGET(gtk_builder_get_object(newctx->builder, "viewdata_print"));
	gtk_widget_set_sensitive(cutbutton, FALSE);
	gtk_widget_set_sensitive(pastebutton, FALSE);
	gtk_widget_set_sensitive(deletebutton, FALSE);
	/* for now */
	gtk_widget_set_sensitive(printbutton, FALSE);


	newctx->filename = strdup(filename);

	newctx->data = ndata;
	newctx->data_len = ndata_len;

	newctx->window = window;

	newctx->next = nsgtk_viewdata_list;
	newctx->prev = NULL;
	if (nsgtk_viewdata_list != NULL) {
		nsgtk_viewdata_list->prev = newctx;
	}
	nsgtk_viewdata_list = newctx;

	nsgtk_attach_viewdata_menu_handlers(newctx->builder, newctx);

	gtk_window_set_title(window, title);

	g_signal_connect(G_OBJECT(window), "destroy",
			 G_CALLBACK(nsgtk_viewdata_destroy_event),
			 newctx);
	g_signal_connect(G_OBJECT(window), "delete-event",
			 G_CALLBACK(nsgtk_viewdata_delete_event),
			 newctx);

	dataview = GTK_TEXT_VIEW(gtk_builder_get_object(newctx->builder,
							"viewdata_view"));

	fontdesc = pango_font_description_from_string("Monospace 8");

	newctx->gv = dataview;
	nsgtk_widget_modify_font(GTK_WIDGET(dataview), fontdesc);

	tb = gtk_text_view_get_buffer(dataview);
	gtk_text_buffer_set_text(tb, newctx->data, -1);

	gtk_widget_show(GTK_WIDGET(window));

	return NSERROR_OK;
}

/**
 * create a new tab with page source
 */
static nserror
tab_init(const char *title,
	 const char *filename,
	 char *ndata,
	 size_t ndata_len)
{
	nsurl *url;
	nserror ret;
	gchar *fname;
	gint handle;
	FILE *f;

	handle = g_file_open_tmp("nsgtksourceXXXXXX", &fname, NULL);
	if ((handle == -1) || (fname == NULL)) {
		return NSERROR_SAVE_FAILED;
	}
	close(handle); /* in case it was binary mode */

	/* save data to temportary file */
	f = fopen(fname, "w");
	if (f == NULL) {
		warn_user(messages_get("gtkSourceTabError"), 0);
		g_free(fname);
		return NSERROR_SAVE_FAILED;
	}
	fprintf(f, "%s", ndata);
	fclose(f);

	/* Open tab on temporary file */
	ret = netsurf_path_to_nsurl(fname, &url);
	g_free(fname);
	if (ret != NSERROR_OK) {
		return ret;
	}

	/* open tab on temportary file */
	ret = browser_window_create(BW_CREATE_TAB | BW_CREATE_HISTORY, url, NULL, NULL, NULL);
	nsurl_unref(url);
	if (ret != NSERROR_OK) {
		return ret;
	}

	free(ndata);

	return NSERROR_OK;
}

/**
 * create a new tab with page source
 */
static nserror
editor_init(const char *title,
	    const char *filename,
	    char *ndata,
	    size_t ndata_len)
{
/* find user configured app for opening text/plain */

/*
 * serach path is ${XDG_DATA_HOME:-$HOME/.local/share}:${XDG_DATA_DIRS:-/usr/local/share:/usr/share}
 *
 * $XDG_DATA_HOME if empty use $HOME/.local/share
 *
 * XDG_DATA_DIRS if empty use /usr/local/share/:/usr/share/
 *
 * search path looking for applications/defaults.list
 *
 * look for [Default Applications]
 * search lines looking like mime/type=Desktop
 *
 * if mimetype is found
 * use search path with applications/application.desktop
 *
 * search desktop file for:
 * Exec=gedit %U
 *
 * execute target app on saved data
 */

	free(ndata);

	return NSERROR_OK;
}

/* exported interface documented in gtk/viewdata.h */
nserror
nsgtk_viewdata(const char *title,
	       const char *filename,
	       char *ndata,
	       size_t ndata_len)
{
	nserror ret;

	switch (nsoption_int(developer_view)) {
	case 0:
		ret = window_init(title, filename, ndata, ndata_len);
		break;

	case 1:
		ret = tab_init(title, filename, ndata, ndata_len);
		break;

	case 2:
		ret = editor_init(title, filename, ndata, ndata_len);
		break;

	default:
		ret = NSERROR_BAD_PARAMETER;
		break;
	}
	if (ret != NSERROR_OK) {
		/* release the data */
		free(ndata);
	}


	return ret;
}