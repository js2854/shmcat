/*
 * shmcat: Dump Shared Memory Segments and more
 * (C) 2012 by Stefan Gast
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * status.h: Defines a status enumeration for shmcat
 */

#ifndef SHMCAT_STATUS_H
#define SHMCAT_STATUS_H

/* This one is sorted in ascending order. The main getopt loop just has
 * to update its status if a dumper function returns a status with a higher
 * value than the current one. */
typedef enum
{
	SHMCAT_NOTHING_DONE	= 0,
	SHMCAT_OK		= 1,
	SHMCAT_ERROR		= 2,
	SHMCAT_PANIC		= 3
} ShmcatStatus;

#endif
