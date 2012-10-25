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
 * shmcat.c: shmcat main program
 */

#include "config.h"
#include <stdio.h>
#include <locale.h>
#include <unistd.h>
#include "translate.h"

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#include "status.h"
#include "filedumper.h"
#include "printtext.h"
#include "shmdumper.h"


enum
{
	EXITCODE_OK		= 0,
	EXITCODE_ERROR		= 1,
	EXITCODE_FATAL		= 2,
	EXITCODE_USAGE		= 10
};


static void usage(const char *progname)
{
	printf(_("Usage: %s [OPTION]... [OPERAND]...\n"), progname);
	puts(_("Print files, System V shared memory objects and/or the given text\n"
	       "to standard output.\n"));

	puts(_("Following OPTIONs are supported:\n"));

#ifdef HAVE_GETOPT_LONG
	puts(_("-h, --help               display this help and exit.\n"
	       "-?                       same as -h.\n"
	       "-V, --version            output version information and exit.\n"));
#else /* !HAVE_GETOPT_LONG */
	puts(_("-h             display this help and exit.\n"
	       "-?             same as -h.\n"
	       "-V             output version information and exit.\n"));
#endif /* HAVE_GETOPT_LONG */

	puts(_("An OPERAND may be one of:\n"));

#ifdef HAVE_GETOPT_LONG
	puts(_("-f, --file=filename      dump contents of the given file.\n"
	       "-i, --stdin              dump standard input.\n"
	       "-M, --shmkey=key         dump the shared memory object with the given key.\n"
	       "-m, --shmid=id           dump the shared memory object with the given id.\n"
	       "-n, --newline            add a line feed."));
#ifdef ENABLE_POSIX_SHM
	puts(_("-p, --posix-shm=name     dump the POSIX shared memory object with the\n"
	       "                         given name."));
#endif /* ENABLE_POSIX_SHM */
	puts(_("-t, --text=text          print the given text.\n"));

#else /* !HAVE_GETOPT_LONG */
	puts(_("-f filename    dump contents of the given file.\n"
	       "-i             dump standard input.\n"
	       "-M key         dump the shared memory object with the given key.\n"
	       "-m id          dump the shared memory object with the given id.\n"
	       "-n             add a line feed."));
#ifdef ENABLE_POSIX_SHM
	puts(_("-p name        dump the POSIX shared memory object with the given name."));
#endif /* ENABLE_POSIX_SHM */
	puts(_("-t text        print the given text.\n"));
#endif /* HAVE_GETOPT_LONG */

	puts(_("If no operand is given, read standard input."));
}


static void show_version(void)
{
	printf("%s ", PACKAGE_STRING);
#ifdef ENABLE_BIG_BUFFER
	puts(_("(Compiled with big buffer)"));
#else
	puts(_("(Compiled with small buffer)"));
#endif
	puts(_("Copyright (C) 2012 by Stefan Gast"));
	puts(_("This is free software.  You may redistribute copies of it under the terms of\n"
               "the GNU General Public License <http://www.gnu.org/licenses/gpl.html>.\n"
               "There is NO WARRANTY, to the extent permitted by law.\n"));

	/* TRANSLATORS: The placeholder indicates the bug-reporting address
           for this package.  Please add _another line_ saying
           "Report translation bugs to <...>\n" with the address for translation
           bugs (typically your translation team's web or email address).  */
	printf(_("Report bugs to: %s\n"), PACKAGE_BUGREPORT);
}


int main(int argc, char **argv)
{
#ifdef ENABLE_POSIX_SHM
	const char *optstr = "hVf:iM:m:np:t:";
#else
	const char *optstr = "hVf:iM:m:nt:";
#endif
#if defined HAVE_GETOPT_LONG && defined HAVE_GETOPT_H
	static const struct option long_options[] =
	{
		{ "help", no_argument, NULL, 'h' },
		{ "version", no_argument, NULL, 'V' },
		{ "file", required_argument, NULL, 'f' },
		{ "stdin", no_argument, NULL, 'i' },
		{ "shmkey", required_argument, NULL, 'M' },
		{ "shmid", required_argument, NULL, 'm' },
		{ "newline", no_argument, NULL, 'n' },
#ifdef ENABLE_POSIX_SHM
		{ "posix-shm", required_argument, NULL, 'p' },
#endif
		{ "text", required_argument, NULL, 't' }
	};
#endif
	int opt;
	ShmcatStatus status = SHMCAT_NOTHING_DONE;
	ShmcatStatus dumperStatus;

	/* Initialize i18n support */
	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	/* Do a first run of gettext/gettext_long to check the commandline for
	 * validity and to handle help or version info options. */
	opterr = 0;
	optind = 1;
#if defined HAVE_GETOPT_LONG && defined HAVE_GETOPT_H
	while((opt = getopt_long(argc, argv, optstr, long_options, NULL)) != -1)
#else
	while((opt = getopt(argc, argv, optstr)) != -1)
#endif
	{
		switch(opt)
		{
			case 'h':
				usage(argv[0]);
				return EXITCODE_OK;

			case 'V':
				show_version();
				return EXITCODE_OK;

			case '?':
				if(optopt == '?')
				{
					usage(argv[0]);
					return EXITCODE_OK;
				}
				else
				{
					fprintf(stderr, _("%s: Wrong usage, try \"%s -h\" for more information!\n"), argv[0], argv[0]);
					return EXITCODE_USAGE;
				}
		}
	}
	if(argc != optind)
	{
		fprintf(stderr, _("%s: Wrong usage, try \"%s -h\" for more information!\n"), argv[0], argv[0]);
		return EXITCODE_USAGE;
	}

	/* Second getopt run does the actual work */
	optind = 1;
#if defined HAVE_GETOPT_LONG && defined HAVE_GETOPT_H
	while((opt = getopt_long(argc, argv, optstr, long_options, NULL)) != -1 && status != SHMCAT_PANIC)
#else
	while((opt = getopt(argc, argv, optstr)) != -1 && status != SHMCAT_PANIC)
#endif
	{
		switch(opt)
		{
			case 'f':
				dumperStatus = dumpFilename(optarg, argv[0]);
				break;
			case 'i':
				dumperStatus = dumpFd(STDIN_FILENO, NULL, argv[0]);
				break;
			case 'M':
				dumperStatus = dumpShmKey(optarg, argv[0]);
				break;
			case 'm':
				dumperStatus = dumpShmId(optarg, argv[0]);
				break;
			case 'n':
				dumperStatus = printText("\n", argv[0]);
				break;
#ifdef ENABLE_POSIX_SHM
			case 'p':
				dumperStatus = dumpPosixShm(optarg, argv[0]);
				break;
#endif
			case 't':
				dumperStatus = printText(optarg, argv[0]);
				break;
			default:
				/* Notify the user someting is wrong, but do not handle this
				 * as a fatal error. */
				fprintf(stderr, _("%s: BUG: Unhandled command line option: %c\n"), argv[0], opt);
				dumperStatus = SHMCAT_ERROR;
				break;
		}

		/* The ShmcatStatus enum is ordered ascendingly, so we just have to
		 * increase our status variable if the current run of the loop led
		 * to a higher status value. */
		if(status < dumperStatus)
			status = dumperStatus;
	}

	/* If we haven't done anything until here, just dump stdin. */
	if(status == SHMCAT_NOTHING_DONE)
		status = dumpFd(STDIN_FILENO, NULL, argv[0]);

	switch(status)
	{
		case SHMCAT_OK:
			return EXITCODE_OK;
		case SHMCAT_ERROR:
			return EXITCODE_ERROR;
		case SHMCAT_PANIC:
			return EXITCODE_FATAL;
		default:
			fprintf(stderr, _("%s: BUG: One of the program routines returned an invalid value (%i)!\n"), argv[0], (int)status);
			return EXITCODE_ERROR;
	}
}
