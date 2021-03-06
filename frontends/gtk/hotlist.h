/*
 * Copyright 2009 Paul Blokus <paul_pl@users.sourceforge.net>
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

/** \file
 * GTK hotlist (interface).
 */

#ifndef __NSGTK_HOTLIST_H__
#define __NSGTK_HOTLIST_H__

#include <gtk/gtk.h>

extern GtkWindow *wndHotlist;

/**
 * Initialise the gtk specific hotlist (bookmarks) display.
 *
 * \return NSERROR_OK on success else appropriate error code on faliure.
 */
nserror nsgtk_hotlist_init(void);


void nsgtk_hotlist_destroy(void);

#endif /* __NSGTK_HOTLIST_H__ */
