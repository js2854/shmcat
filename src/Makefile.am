# shmcat: Dump Shared Memory Segments and more
# (C) 2012 by Stefan Gast

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

# Makefile.am: Input file for automake

AM_CPPFLAGS = -I$(top_srcdir)/intl
LDADD = @LIBINTL@

bin_PROGRAMS = shmcat $(FTOK)
EXTRA_PROGRAMS = ftok

shmcat_SOURCES = filedumper.c filedumper.h gettext.h \
		 keythelper.h shmcat.c printtext.c printtext.h \
		 shmdumper.c shmdumper.h status.h translate.h
shmcat_LDADD = @SHMCAT_EXTRA_LIBS@ @LIBINTL@

ftok_SOURCES = ftok.c gettext.h keythelper.h translate.h

datadir = @datadir@
localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@
LIBS = @LIBINTL@ @LIBS@
