/*
 * shmcat: Dump System V Shared Memory Objects
 * (C) 2012 by Stefan Gast (sgdev@arcor.de)
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
 * filedumper.c: Implementation of the file dumper
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "filedumper.h"
#include "translate.h"


ShmcatStatus dumpFd(int fd, const char *filename, const char *programName)
{
	/* Place the buffer in the data segment, so we can make it quite big.
	 * We use a 16kB buffer at the moment. */
	static char buffer[16*1024];

	ssize_t bytesToDump;
	ssize_t bytesDumped;

	while((bytesToDump = read(fd, buffer, sizeof(buffer))) > 0)
	{
		while(bytesToDump > 0)
		{
			bytesDumped = write(STDOUT_FILENO, buffer, bytesToDump);
			if(bytesDumped <= 0)
			{
				fprintf(stderr, _("%s: Error writing to standard output: %s\n"),
						programName, strerror(errno));
				return SHMCAT_PANIC;
			}
			else
				bytesToDump -= bytesDumped;
		}
	}

	if(bytesToDump == -1)
	{
		if(filename)
			fprintf(stderr, _("%s: Error reading file \"%s\": %s\n"),
					programName, filename, strerror(errno));
		else
			fprintf(stderr, _("%s: Error reading standard input: %s\n"),
					programName, strerror(errno));

		return SHMCAT_ERROR;
	}
	else
		return SHMCAT_OK;
}


ShmcatStatus dumpFilename(const char *filename, const char *programName)
{
	int fd = open(filename, O_RDONLY);
	ShmcatStatus ret;

	if(fd == -1)
	{
		fprintf(stderr, _("%s: Cannot open file \"%s\": %s\n"),
				programName, filename, strerror(errno));
		return SHMCAT_ERROR;
	}

	ret = dumpFd(fd, filename, programName);

	if(close(fd) == -1)
	{
		fprintf(stderr, _("%s: Error closing file \"%s\": %s\n"),
				programName, filename, strerror(errno));
		return SHMCAT_ERROR;
	}
	else
		return ret;
}
