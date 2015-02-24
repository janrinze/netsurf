/*
 * Copyright 2009 Mark Benjamin <netsurf-browser.org.MarkBenjamin@dfgh.net>
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
#ifndef _NETSURF_GTK_MENU_H_
#define _NETSURF_GTK_MENU_H_

#include <gtk/gtk.h>

struct nsgtk_file_menu {
	GtkMenuItem                     *file; /* File menu item on menubar */
	GtkMenu				*file_menu;
	GtkMenuItem			*newwindow_menuitem;
	GtkMenuItem			*newtab_menuitem;
	GtkMenuItem			*openfile_menuitem;
	GtkMenuItem			*closewindow_menuitem;
	GtkMenuItem			*savepage_menuitem;
	GtkMenuItem			*export_menuitem;
	struct nsgtk_export_submenu	*export_submenu;
	GtkMenuItem			*printpreview_menuitem;
	GtkMenuItem			*print_menuitem;
	GtkMenuItem			*quit_menuitem;
};

struct nsgtk_edit_menu {
	GtkMenuItem             *edit; /* Edit menu item on menubar */
	GtkMenu			*edit_menu;
	GtkMenuItem		*cut_menuitem;
	GtkMenuItem		*copy_menuitem;
	GtkMenuItem		*paste_menuitem;
	GtkMenuItem		*delete_menuitem;
	GtkMenuItem		*selectall_menuitem;
	GtkMenuItem		*find_menuitem;
	GtkMenuItem		*preferences_menuitem;
};

struct nsgtk_view_menu {
	GtkMenuItem             *view; /* View menu item on menubar */
	GtkMenu			*view_menu; /* gtk menu attached to menu item */
	GtkMenuItem			*stop_menuitem;
	GtkMenuItem			*reload_menuitem;
	GtkMenuItem			*scaleview_menuitem;
	struct nsgtk_scaleview_submenu	*scaleview_submenu;
	GtkMenuItem			*fullscreen_menuitem;
	GtkMenuItem			*images_menuitem;
	struct nsgtk_images_submenu	*images_submenu;
	GtkMenuItem			*toolbars_menuitem;
	struct nsgtk_toolbars_submenu	*toolbars_submenu;
	GtkMenuItem			*tabs_menuitem;
	struct nsgtk_tabs_submenu	*tabs_submenu;
	GtkMenuItem			*savewindowsize_menuitem;
};

struct nsgtk_nav_menu {
	GtkMenuItem             *nav; /* Nav menu item on menubar */
	GtkMenu			*nav_menu;
	GtkMenuItem		*back_menuitem;
	GtkMenuItem		*forward_menuitem;
	GtkMenuItem		*home_menuitem;
	GtkMenuItem		*localhistory_menuitem;
	GtkMenuItem		*globalhistory_menuitem;
	GtkMenuItem		*addbookmarks_menuitem;
	GtkMenuItem		*showbookmarks_menuitem;
	GtkMenuItem		*openlocation_menuitem;
};

struct nsgtk_tools_menu {
	GtkMenuItem  *tools; /* Tools menu item on menubar */
	GtkMenu	*tools_menu;

	GtkMenuItem *showcookies_menuitem;
	GtkMenuItem *downloads_menuitem;
	GtkMenuItem *developer_menuitem;
	struct nsgtk_developer_submenu *developer_submenu;
};

struct nsgtk_help_menu {
	GtkMenuItem             *help; /* Help menu item on menubar */
	GtkMenu			*help_menu;
	GtkMenuItem		*contents_menuitem;
	GtkMenuItem		*guide_menuitem;
	GtkMenuItem		*info_menuitem;
	GtkMenuItem		*about_menuitem;
};


struct nsgtk_export_submenu {
	GtkMenu			*export_menu;
	GtkMenuItem		*plaintext_menuitem;
	GtkMenuItem		*drawfile_menuitem;
	GtkMenuItem		*postscript_menuitem;
	GtkMenuItem		*pdf_menuitem;
};

struct nsgtk_scaleview_submenu {
	GtkMenu			*scaleview_menu;
	GtkMenuItem		*zoomplus_menuitem;
	GtkMenuItem		*zoomminus_menuitem;
	GtkMenuItem		*zoomnormal_menuitem;
};

struct nsgtk_tabs_submenu {
	GtkMenu			*tabs_menu;
	GtkMenuItem		*nexttab_menuitem;
	GtkMenuItem		*prevtab_menuitem;
	GtkMenuItem		*closetab_menuitem;
};

struct nsgtk_images_submenu {
	GtkMenu			*images_menu;
	GtkCheckMenuItem	*foregroundimages_menuitem;
	GtkCheckMenuItem	*backgroundimages_menuitem;
};

struct nsgtk_toolbars_submenu {
	GtkMenu			*toolbars_menu;
	GtkCheckMenuItem	*menubar_menuitem;
	GtkCheckMenuItem	*toolbar_menuitem;
};

struct nsgtk_developer_submenu {
	GtkMenu			*developer_menu;

	GtkMenuItem		*viewsource_menuitem;
	GtkMenuItem		*toggledebugging_menuitem;
	GtkMenuItem		*debugboxtree_menuitem;
	GtkMenuItem		*debugdomtree_menuitem;
};


struct nsgtk_bar_submenu {
	GtkMenuBar		*bar_menu;
	struct nsgtk_file_menu	*file_submenu;
	struct nsgtk_edit_menu	*edit_submenu;
	struct nsgtk_view_menu	*view_submenu;
	struct nsgtk_nav_menu	*nav_submenu;
	struct nsgtk_tabs_submenu	*tabs_submenu;
	struct nsgtk_tools_menu	*tools_submenu;
	struct nsgtk_help_menu	*help_submenu;
};

struct nsgtk_popup_menu {
	GtkMenu	*popup_menu;

	GtkMenuItem *file_menuitem;
	struct nsgtk_file_menu *file_submenu;

	GtkMenuItem *edit_menuitem;
	struct nsgtk_edit_menu *edit_submenu;

	GtkMenuItem *view_menuitem;
	struct nsgtk_view_menu *view_submenu;

	GtkMenuItem *nav_menuitem;
	struct nsgtk_nav_menu *nav_submenu;

	GtkMenuItem *tabs_menuitem;
	struct nsgtk_tabs_submenu *tabs_submenu;

	GtkMenuItem *tools_menuitem;
	struct nsgtk_tools_menu *tools_submenu;

	GtkMenuItem *help_menuitem;
	struct nsgtk_help_menu *help_submenu;

	GtkWidget *first_separator;

	/* navigation entries */
	GtkMenuItem *back_menuitem;
	GtkMenuItem *forward_menuitem;

	GtkWidget *third_separator;

	/* view entries */
	GtkMenuItem *stop_menuitem;
	GtkMenuItem *reload_menuitem;

	GtkMenuItem *cut_menuitem;
	GtkMenuItem *copy_menuitem;
	GtkMenuItem *paste_menuitem;
	GtkMenuItem *customize_menuitem;

};

struct nsgtk_link_menu {
	GtkMenu	*link_menu;

	GtkMenuItem *opentab_menuitem;
	GtkMenuItem *openwin_menuitem;

	GtkMenuItem *save_menuitem;
	GtkMenuItem *bookmark_menuitem;
	GtkMenuItem *copy_menuitem;
};

/**
 * Create main menu bar.
 */
struct nsgtk_bar_submenu *nsgtk_menu_bar_create(GtkMenuShell *menubar, GtkAccelGroup *group);

/**
 * Generate right click menu menu.
 *
 */
struct nsgtk_popup_menu *nsgtk_popup_menu_create(GtkAccelGroup *group);

/**
 * Generate context sensitive popup menu for link.
 *
 */
struct nsgtk_link_menu *nsgtk_link_menu_create(GtkAccelGroup *group);


#endif
