#!/bin/sh

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

# release.sh: Script to pack a release

function terminate ()
{
	# move dummy ChangeLog back to place, git doesn't need to track itself
	test -f ChangeLog.dummy && mv ChangeLog.dummy ChangeLog
	exit $1
}

srcdir=$(dirname $0)
test -z "$srcdir" && srcdir=.

cd "$srcdir"

version=$(sed -n -e '/^AC_INIT(.*)/s/AC_INIT(shmcat, *\([^,]*\).*/\1/p' configure.ac)
echo "packing shmcat-$version release tarballs..."

# create ChangeLog from git log
mv ChangeLog ChangeLog.dummy
git log > ChangeLog || terminate 1

# invoke configure if we don't have a Makefile (did a distclean before)
if [ ! -r Makefile ]
then
	./configure || terminate 1
fi

# pack release
make dist-bzip2	|| terminate 1

terminate 0
