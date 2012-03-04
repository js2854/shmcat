# shmcat: Dump System V Shared Memory Objects
# (C) 2012 by Stefan Gast (sgdev@arcor.de)

#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

# configure.ac: Input file for autoconf


AC_PREREQ([2.65])
AC_INIT(shmcat, 1.2, sgdev@arcor.de)
AM_CONFIG_HEADER(config.h)
AC_LANG(C)

AM_INIT_AUTOMAKE([dist-bzip2 1.11])
AM_SILENT_RULES([yes])

AM_SANITY_CHECK

AC_PROG_CC
AC_GNU_SOURCE
AC_PROG_INSTALL

AC_ISC_POSIX

AC_ARG_ENABLE([big-buffer], AS_HELP_STRING([--disable-big-buffer], [Use a smaller buffer for file dumps (useful on memory constrained systems)]))
AS_IF([test "x$enable_big_buffer" != "xno"],
      AC_DEFINE([ENABLE_BIG_BUFFER], [1], [Define if shmcat should use a 2MB buffer for file dumps]))

AC_CHECK_HEADERS(limits.h stdio.h stdlib.h string.h errno.h locale.h, [], [AC_MSG_ERROR(Some ANSI headers could not be found. Aborting.)])
AC_CHECK_HEADERS(fcntl.h unistd.h sys/types.h sys/stat.h, [], [AC_MSG_ERROR(Some POSIX headers could not be found. Aborting.)])
AC_CHECK_HEADERS(sys/ipc.h sys/shm.h, [], [AC_MSG_ERROR(Could not find the headers for System V Shared Memory. Aborting.)])
AC_CHECK_HEADERS(inttypes.h stdint.h)
AC_CHECK_HEADERS(getopt.h)

AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINTPTR_T

AC_CHECK_FUNCS(fprintf printf puts setlocale strerror strlen strtoul strtoull, [], [AC_MSG_ERROR(Some ANSI functions are not available. Aborting.)])
AC_CHECK_FUNCS(close open read write, [], [AC_MSG_ERROR(Some POSIX functions could not be found. Aborting.)])
AC_CHECK_FUNCS(shmget shmat shmctl shmdt, [], [AC_MSG_ERROR(Some Shared Memory functions are not available. Aborting.)])

dnl Check how the program can parse its commandline. getopt_long is the preferred way.
AC_CHECK_FUNCS(getopt_long, [],
			    [AC_CHECK_FUNC(getopt, [],
			    [AC_MSG_ERROR(None of the functions getopt_long or getopt found. Aborting.)])])

AC_CHECK_FUNCS(puts setlocale, [], [AC_MSG_ERROR(Some ANSI functions are not available. Aborting.)])

AM_GNU_GETTEXT
AM_GNU_GETTEXT_VERSION(0.17)

AC_OUTPUT(Makefile intl/Makefile po/Makefile.in src/Makefile)
