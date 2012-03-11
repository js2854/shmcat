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
 * ftok.c: ftok main program
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#include "translate.h"


enum
{
	EXITCODE_OK = 0,
	EXITCODE_ERROR = 1,
	EXITCODE_USAGE = 10
};


static void usage(const char *progname)
{
	printf(_("Usage: %s OPTION...\n"), progname);
	printf(_("       %s FILENAME ID\n"), progname);
	puts(_("Calculate a System V interprocess communication key using the\n"
	       "ftok(3) library function and print the result to\n"
	       "standard output.\n"));

	puts(_("Following OPTIONs are supported:\n"));

#ifdef HAVE_GETOPT_LONG
	puts(_("-h, --help               display this help and exit.\n"
	       "-?                       same as -h.\n"
	       "-V, --version            output version information and exit.\n"));
#else
	puts(_("-h             display this help and exit.\n"
	       "-?             same as -h.\n"
	       "-V             output version information and exit.\n"));
#endif

	puts(_("Normal usage (second form) does not support any OPTIONs, but\n"
	       "always needs the arguments FILENAME and ID, with ID being a\n"
	       "number between 0 and 255, inclusively."));
}


static void show_version(const char *progname)
{
	printf(_("%s, part of %s\n"), progname, PACKAGE_STRING);
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


static void wrong_usage(const char *progname)
{
	fprintf(stderr, _("%s: Wrong usage, try \"%s -h\" for more information!\n"),
		progname, progname);
}


int main(int argc, char **argv)
{
	const char *optstr = "hV";
#if defined HAVE_GETOPT_LONG && defined HAVE_GETOPT_H
	static const struct option long_options[] =
	{
		{ "help", no_argument, NULL, 'h' },
		{ "version", no_argument, NULL, 'V' }
	};
#endif
	int opt;

	unsigned long id;
	char *endptr;

	key_t key;

	/* Initialize i18n support */
	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	/* Process options */
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
				show_version(argv[0]);
				return EXITCODE_OK;

			case '?':
				if(optopt == '?')
				{
					usage(argv[0]);
					return EXITCODE_OK;
				}
				else
				{
					wrong_usage(argv[0]);
					return EXITCODE_USAGE;
				}

			default:
				fprintf(stderr, _("%s: BUG: Unhandled command line option: %c\n"), argv[0], opt);
				return EXITCODE_ERROR;
		}
	}

	/* All options we support cause the program to quit in the while loop
	 * above, so everything starting from here is done for the normal usage
	 * case that does not require any options. But we do need the two
	 * arguments FILENAME and ID. */
	if(argc != 3)
	{
		wrong_usage(argv[0]);
		return EXITCODE_USAGE;
	}

	/* Try to get the ID. Note that we also support 0 as an ID, even though
	 * the man page of ftok says the behavior is unspecified in this case.
	 * But on Linux this works as well, so we just assume the user knows
	 * what he is doing in this case. */
	id = strtoul(argv[2], &endptr, 0);
	if(*endptr != '\0' || id > 255)
	{
		fprintf(stderr, _("%s: The ID has to be a number between 0 and 255!\n"), argv[0]);
		return EXITCODE_USAGE;
	}

	/* Now try to get the key from the parameters. */
	key = ftok(argv[1], id);
	if(key == (key_t)-1)
	{
		fprintf(stderr, _("%s: Cannot calculate a key for the given parameters: %s\n"),
			argv[0], strerror(errno));
		return EXITCODE_ERROR;
	}

	/* Finally, print the result
	 * TODO: Also add support for decimal and octal output */
	if(printf("0x%llx\n", (unsigned long long)key) <= 0)
	{
		fprintf(stderr, _("%s: Cannot print to standard output: %s\n"),
			argv[0], strerror(errno));
		return EXITCODE_ERROR;
	}

	return EXITCODE_OK;
}
