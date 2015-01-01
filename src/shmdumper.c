/*
 * shmcat: Dump Shared Memory Segments and more
 * (C) 2012, 2015 by Stefan Gast
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
 * shmdumper.c: Implementation of the shared memory dumper
 */

#include "config.h"
#include "keythelper.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#ifdef ENABLE_POSIX_SHM
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#include "shmdumper.h"
#include "translate.h"


static ShmcatStatus dumpMem(const char *start, const char *stop, const char *programName)
{
	const char *pos = start;
	ssize_t bytesWritten;

	/* If we have a huge memory area, we have to be careful
	 * with the size we pass to write. Otherwise the ssize_t
	 * might overflow. So we do the huge chunks first with
	 * this special loop. */
	while((uintptr_t)stop-(uintptr_t)pos > (uintptr_t)SSIZE_MAX)
	{
		bytesWritten = write(STDOUT_FILENO, pos, SSIZE_MAX);
		if(bytesWritten <= 0)
		{
			fprintf(stderr, _("%s: Error writing to standard output: %s\n"),
					programName, strerror(errno));
			return SHMCAT_PANIC;
		}
		else
			pos += bytesWritten;
	}

	/* Now dump the rest. */
	while(pos != stop)
	{
		bytesWritten = write(STDOUT_FILENO, pos, stop-pos);
		if(bytesWritten <= 0)
		{
			fprintf(stderr, _("%s: Error writing to standard output: %s\n"),
					programName, strerror(errno));
			return SHMCAT_PANIC;
		}
		else
			pos += bytesWritten;
	}

	/* If we reach this, everything worked fine. */
	return SHMCAT_OK;
}


static ShmcatStatus dumpShm(int id, int byKey, const char *param, const char *programName)
{
	struct shmid_ds infoBuffer;
	const char *shm;
	ShmcatStatus status;

	/* Put the segment into our address space. We do this even before
	 * we query its size, to make sure the segment does not disappear
	 * (i.e. get deleted) between getting the size and attaching it. */
	shm = shmat(id, NULL, SHM_RDONLY);
	if(shm == (const char *)-1)
	{
		if(byKey)
			fprintf(stderr, _("%s: Cannot attach the shared memory segment with the key \"%s\": %s\n"),
					programName, param, strerror(errno));
		else
			fprintf(stderr, _("%s: Cannot attach the shared memory segment with the id \"%s\": %s\n"),
					programName, param, strerror(errno));
		return SHMCAT_ERROR;
	}

	/* Now we need to know how big our segment is. */
	if(shmctl(id, IPC_STAT, &infoBuffer) == -1)
	{
		if(byKey)
			fprintf(stderr, _("%s: Cannot determine the size of the shared memory segment with the key \"%s\": %s\n"),
					programName, param, strerror(errno));
		else
			fprintf(stderr, _("%s: Cannot determine the size of the shared memory segment with the id \"%s\": %s\n"),
					programName, param, strerror(errno));

		shmdt(shm);
		return SHMCAT_ERROR;
	}

	/* Dump memory and save status */
	status = dumpMem(shm, shm + infoBuffer.shm_segsz, programName);

	shmdt(shm);
	return status;
}


ShmcatStatus dumpShmKey(const char *keystr, const char *programName)
{
	char *numEnd;
	intmax_t key_signed;
	uintmax_t key_unsigned;
	int id;

	/* Try to interpret the keystr as a signed value first */
	errno = 0;
	key_signed = strtoimax(keystr, &numEnd, 0);
	if(*numEnd != '\0' || key_signed < KEY_T_MIN || key_signed > KEY_T_MAX || errno != 0)
	{
		/* Failed? Then try unsigned, but ensure it is not too large for the unsigned key type */
		errno = 0;
		key_unsigned = strtoumax(keystr, &numEnd, 0);
		if(*numEnd != '\0' || key_unsigned > KEY_T_UNSIGNED_MAX || errno != 0)
		{
			fprintf(stderr, _("%s: Invalid shared memory key: %s\n"),
					programName, keystr);
			return SHMCAT_ERROR;
		}
		key_signed = (intmax_t)key_unsigned;
	}
	if(key_signed == (intmax_t)IPC_PRIVATE)
	{
		fprintf(stderr, _("%s: \"%s\" is the key for private shared memory segments! Try passing the segment by ID instead.\n"),
				programName, keystr);
		return SHMCAT_ERROR;
	}

	id = shmget((key_t)key_signed, 0, 0);
	if(id == -1)
	{
		fprintf(stderr, _("%s: Cannot open a shared memory segment with the key \"%s\": %s\n"),
				programName, keystr, strerror(errno));
		return SHMCAT_ERROR;
	}

	return dumpShm(id, 1, keystr, programName);
}


ShmcatStatus dumpShmId(const char *idstr, const char *programName)
{
	char *numEnd;
	long id;

	errno = 0;
	id = strtol(idstr, &numEnd, 0);
	/* POSIX man page shmget: "Upon successful completion, shmget() shall return a _non-negative_
	 * integer." So we check:
	 * a) if everything could be converted,
	 * b) if the value is non-negative,
	 * c) if the value fits into an integer, and
	 * d) if the conversion did not fail otherwise. */
	if(*numEnd != '\0' || id < 0 || id > INT_MAX || errno != 0)
	{
		fprintf(stderr, _("%s: Invalid shared memory id: %s\n"),
				programName, idstr);
		return SHMCAT_ERROR;
	}

	return dumpShm((int)id, 0, idstr, programName);
}


#ifdef ENABLE_POSIX_SHM
ShmcatStatus dumpPosixShm(const char *shmName, const char *programName)
{
	int fd;
	struct stat infoBuffer;
	char *shm;
	ShmcatStatus status;

	/* Open POSIX shared memory object. */
	if((fd = shm_open(shmName, O_RDONLY, (mode_t)0)) == -1)
	{
		fprintf(stderr, _("%s: Cannot open the shared memory segment with the name \"%s\": %s\n"),
				programName, shmName, strerror(errno));
		return SHMCAT_ERROR;
	}

	/* Get the size of it. */
	if(fstat(fd, &infoBuffer) == -1)
	{
		fprintf(stderr,
			_("%s: Cannot determine the size of the shared memory segment with the name \"%s\": %s\n"),
			programName, shmName, strerror(errno));
		close(fd);
		return SHMCAT_ERROR;
	}

	/* Map it into our address space. */
	shm = mmap(NULL, (size_t)infoBuffer.st_size, PROT_READ, MAP_SHARED, fd, (off_t)0);
	if((void *)shm == MAP_FAILED)
	{
		fprintf(stderr, _("%s: Cannot attach the shared memory segment with the name \"%s\": %s\n"),
				programName, shmName, strerror(errno));
		close(fd);
		return SHMCAT_ERROR;
	}

	/* Dump memory and save status */
	status = dumpMem(shm, shm + infoBuffer.st_size, programName);

	munmap(shm, (size_t)infoBuffer.st_size);
	close(fd);
	return status;
}
#endif /* ENABLE_POSIX_SHM */
