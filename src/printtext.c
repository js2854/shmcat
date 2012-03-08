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
 * printtext.c: Implements the printText helper function
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "printtext.h"
#include "translate.h"


ShmcatStatus printText(const char *text, const char *programName)
{
	const char *textEnd = text + strlen(text);
	const char *pos = text;
	ssize_t numPrinted;

	while(pos != textEnd)
	{
		numPrinted = write(STDOUT_FILENO, pos, textEnd-pos);
		if(numPrinted == -1)
		{
			fprintf(stderr, _("%s: Error writing to standard output: %s\n"),
					programName, strerror(errno));
			return SHMCAT_PANIC;
		}
		else
			pos += numPrinted;
	}

	return SHMCAT_OK;
}
