/**
 * Copyright (c) 2015 Fredrik Wikstrom <fredrik@a500.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __amigaos4__
#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef UTILITY_HOOKS_H
#include <utility/hooks.h>
#endif

struct SplayTree;

struct SplayTree *CreateSplayTree(struct Hook *hook);
void DeleteSplayTree(struct SplayTree *tree);
BOOL InsertSplayNode(struct SplayTree *tree, APTR key, APTR data);
APTR FindSplayNode(struct SplayTree *tree, APTR key);
BOOL RemoveSplayNode(struct SplayTree *tree, APTR key);

#endif
#endif // __amigaos4__

