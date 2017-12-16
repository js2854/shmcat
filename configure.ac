# shmcat: Dump Shared Memory Segments and more
# (C) 2012, 2014, 2016, 2017 by Stefan Gast

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
AC_INIT(shmcat, 1.9, sgdev at arcor dot de)
AM_CONFIG_HEADER(config.h)
AC_LANG(C)

AM_INIT_AUTOMAKE([dist-xz 1.11])
AM_SILENT_RULES([yes])

AM_SANITY_CHECK

AC_PROG_CC
AC_GNU_SOURCE
AC_PROG_INSTALL

AC_ISC_POSIX

AC_ARG_ENABLE([big-buffer], AS_HELP_STRING([--disable-big-buffer], [Use a smaller buffer for file dumps (useful on memory constrained systems)]))
AC_ARG_ENABLE([ftok], AS_HELP_STRING([--disable-ftok], [Do not build the ftok utility]))
AC_ARG_ENABLE([posix-shm], AS_HELP_STRING([--disable-posix-shm], [Disable POSIX shared memory support (shm_open and friends)]))

AC_CHECK_HEADERS(limits.h stdio.h stdlib.h string.h errno.h locale.h, [], [AC_MSG_ERROR(Some ANSI headers could not be found. Aborting.)])
AC_CHECK_HEADERS(inttypes.h, [], [AC_MSG_ERROR(Could not find the header inttypes.h. Aborting.)])
AC_CHECK_HEADERS(fcntl.h unistd.h sys/types.h sys/stat.h, [], [AC_MSG_ERROR(Some POSIX headers could not be found. Aborting.)])
AC_CHECK_HEADERS(sys/ipc.h sys/shm.h, [], [AC_MSG_ERROR(Could not find the headers for System V Shared Memory. Aborting.)])
AC_CHECK_HEADERS(getopt.h)
AS_IF([test "x$enable_posix-shm" != "xno"],
      [AC_CHECK_HEADERS(sys/mman.h, [], [AC_MSG_WARN(Header sys/mman.h not found. POSIX shared memory support will not be available.)])])

AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINTPTR_T
AC_TYPE_INTMAX_T
AC_TYPE_UINTMAX_T

AC_CHECK_SIZEOF(key_t)
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(long long)

AC_CHECK_FUNCS(fprintf printf puts setlocale strerror strlen strtol, [], [AC_MSG_ERROR(Some ANSI functions are not available. Aborting.)])
AC_CHECK_FUNCS(strtoimax strtoumax, [], [AC_MSG_ERROR(Function strtoimax/strtoumax not found. Aborting.)])
AC_CHECK_FUNCS(close open read write, [], [AC_MSG_ERROR(Some POSIX functions could not be found. Aborting.)])
AC_CHECK_FUNCS(shmget shmat shmctl shmdt, [], [AC_MSG_ERROR(Some System V Shared Memory functions are not available. Aborting.)])
dnl POSIX shared memory tests
AS_IF([test "x$enable_posix-shm" != "xno"],
      [AC_CHECK_FUNC(shm_open,
                     [shmcat_have_shm_open="yes"],
		     [AC_CHECK_LIB(rt, shm_open,
		                   [shmcat_have_shm_open="yes"
				    shmcat_shm_open_libs='-lrt'
				   ],
				   AC_MSG_WARN(Function shm_open not found. POSIX shared memory support will not be available.)
				  )
		     ]
		    )
       AC_CHECK_FUNC(mmap,
		     [shmcat_have_mmap="yes"],
		     [AC_MSG_WARN(Function mmap not found. POSIX shared memory support will not be available.)]
		    )
       AC_CHECK_FUNC(fstat,
		     [shmcat_have_fstat="yes"],
		     [AC_MSG_WARN(Function fstat not found. POSIX shared memory support will not be available.)]
		    )
      ]
     )

AS_IF([test "x$enable_ftok" != "xno"],
      [AC_CHECK_FUNCS(ftok,
                      [AC_SUBST([FTOK], ['ftok${EXEEXT}'])
		       AC_SUBST([FTOK_MAN], ['ftok.man'])
		      ],
                      [AC_MSG_WARN(The ftok function is not available. Will not build the ftok utility.)])])

dnl Check how the program can parse its commandline. getopt_long is the preferred way.
AC_CHECK_FUNCS(getopt_long, [],
			    [AC_CHECK_FUNC(getopt, [],
			    [AC_MSG_ERROR(None of the functions getopt_long or getopt found. Aborting.)])])

AS_IF([test "x$enable_big_buffer" != "xno"],
      AC_DEFINE([ENABLE_BIG_BUFFER], [1], [Define if shmcat should use a 2MB buffer for file dumps]))

AS_IF([test "x$enable_posix_shm" != "xno" -a "x$ac_cv_header_sys_mman_h" == "xyes" -a "x$shmcat_have_shm_open" == "xyes" -a "x$shmcat_have_mmap" == "xyes" -a "x$shmcat_have_fstat" == "xyes"],
      AC_DEFINE([ENABLE_POSIX_SHM], [1], [Define if the system supports POSIX shared memory and you want support for it in shmcat])
      AC_SUBST([SHMCAT_EXTRA_LIBS], $shmcat_shm_open_libs)
     )

AM_GNU_GETTEXT
AM_GNU_GETTEXT_VERSION(0.18.3)

dnl Define USE_NLS conditional to determine if we have to install translated man pages.
AM_CONDITIONAL(USE_NLS, [test "x$USE_NLS" = "xyes"])

AC_CONFIG_FILES([Makefile intl/Makefile man/Makefile man/de/Makefile po/Makefile.in src/Makefile])
AC_OUTPUT
