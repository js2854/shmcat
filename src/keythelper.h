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
 * keythelper.h: Helper macros for dealing with the key_t data type
 */

#ifndef KEYTHELPER_H
#define KEYTHELPER_H

#include "config.h"
#include "limits.h"

/* Ugly hack to determine some traits of key_t, needed for conversion from/to
 * strings. We assume key_t is signed, which is currently true for Linux and
 * FreeBSD. */
#if SIZEOF_KEY_T == SIZEOF_SHORT
#	define KEY_T_UNSIGNED		unsigned short
#	define KEY_T_MIN		SHRT_MIN
#	define KEY_T_MAX		SHRT_MAX
#	define KEY_T_UNSIGNED_MAX	USHRT_MAX
#elif SIZEOF_KEY_T == SIZEOF_INT
#	define KEY_T_UNSIGNED		unsigned int
#	define KEY_T_MIN		INT_MIN
#	define KEY_T_MAX		INT_MAX
#	define KEY_T_UNSIGNED_MAX	UINT_MAX
#elif SIZEOF_KEY_T == SIZEOF_LONG
#	define KEY_T_UNSIGNED		unsigned long
#	define KEY_T_MIN		LONG_MIN
#	define KEY_T_MAX		LONG_MAX
#	define KEY_T_UNSIGNED_MAX	ULONG_MAX
#elif SIZEOF_KEY_T == SIZEOF_LONG_LONG
#	define KEY_T_UNSIGNED		unsigned long long
#	define KEY_T_MIN		LLONG_MIN
#	define KEY_T_MAX		LLONG_MAX
#	define KEY_T_UNSIGNED_MAX	ULLONG_MAX
#else
#	error	"Could not guess the real data type of key_t! Please edit src/keythelper.h."
#endif

#endif
