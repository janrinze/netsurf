/*
 * This file is part of NetSurf, http://netsurf.sourceforge.net/
 * Licensed under the GNU General Public License,
 *                http://www.opensource.org/licenses/gpl-license
 * Copyright 2004 John M Bell <jmb202@ecs.soton.ac.uk>
 */

/** \file
 * Save HTML document with dependencies (interface).
 */

#ifndef _NETSURF_RISCOS_SAVE_COMPLETE_H_
#define _NETSURF_RISCOS_SAVE_COMPLETE_H_

struct content;

void save_complete_init(void);
void save_complete(struct content *c, const char *path);

#endif
