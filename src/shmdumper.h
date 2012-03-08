/*
 * shmcat: Dump System V Shared Memory Objects
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
 * shmdumper.h: Header for the shared memory dumper
 */

#ifndef SHMCAT_SHMDUMPER_H
#define SHMCAT_SHMDUMPER_H

#include "status.h"

ShmcatStatus dumpShmKey(const char *keystr, const char *programName);
ShmcatStatus dumpShmId(const char *idstr, const char *programName);

#endif

