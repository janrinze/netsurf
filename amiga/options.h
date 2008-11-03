/*
 * Copyright 2008 Chris Young <chris@unsatisfactorysoftware.co.uk>
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

#ifndef AMIGA_OPTIONS_H
#define AMIGA_OPTIONS_H
#include "desktop/options.h"

extern bool option_verbose_log;
extern char *option_url_file;
extern char *option_hotlist_file;
extern bool option_use_wb;
extern char *option_modeid;
extern int option_screen_depth;
extern char *option_theme;
extern bool option_no_iframes;
extern bool option_utf8_clipboard;
extern bool option_context_menu;
extern bool option_sticky_context_menu;
extern bool option_truecolour_mouse_pointers;
extern bool option_use_os_pointers;
extern bool option_force_tabs;
extern bool option_new_tab_active;
extern bool option_kiosk_mode;
extern char *option_recent_file;
extern char *option_arexx_dir;
extern char *option_download_dir;

#define EXTRA_OPTION_DEFINE \
bool option_verbose_log = false; \
char *option_url_file = 0; \
char *option_hotlist_file = 0; \
bool option_use_wb = false; \
char *option_modeid = 0; \
int option_screen_depth = 16; \
char *option_theme = 0; \
bool option_no_iframes = false; \
bool option_utf8_clipboard = false; \
bool option_context_menu = true; \
bool option_sticky_context_menu = true; \
bool option_truecolour_mouse_pointers = false; \
bool option_use_os_pointers = true; \
bool option_force_tabs = false; \
bool option_new_tab_active = false; \
bool option_kiosk_mode = false; \
char *option_recent_file = 0; \
char *option_arexx_dir = 0; \
char *option_download_dir = 0; \

#define EXTRA_OPTION_TABLE \
{ "verbose_log",	OPTION_BOOL,	&option_verbose_log}, \
{ "url_file",		OPTION_STRING,	&option_url_file }, \
{ "hotlist_file",		OPTION_STRING,	&option_hotlist_file }, \
{ "use_workbench",	OPTION_BOOL,	&option_use_wb}, \
{ "screen_modeid",	OPTION_STRING,	&option_modeid}, \
{ "screen_depth",	OPTION_INTEGER,	&option_screen_depth}, \
{ "theme",		OPTION_STRING,	&option_theme}, \
{ "no_iframes",	OPTION_BOOL,	&option_no_iframes}, \
{ "clipboard_write_utf8",	OPTION_BOOL,	&option_utf8_clipboard}, \
{ "context_menu",	OPTION_BOOL,	&option_context_menu}, \
{ "sticky_context_menu",	OPTION_BOOL,	&option_sticky_context_menu}, \
{ "truecolour_mouse_pointers",	OPTION_BOOL,	&option_truecolour_mouse_pointers}, \
{ "os_mouse_pointers",	OPTION_BOOL,	&option_use_os_pointers}, \
{ "always_open_tabs",	OPTION_BOOL,	&option_force_tabs}, \
{ "new_tab_is_active",	OPTION_BOOL,	&option_new_tab_active}, \
{ "kiosk_mode",	OPTION_BOOL,	&option_kiosk_mode}, \
{ "recent_file",		OPTION_STRING,	&option_recent_file }, \
{ "arexx_dir",		OPTION_STRING,	&option_arexx_dir }, \
{ "download_dir",		OPTION_STRING,	&option_download_dir },
#endif
