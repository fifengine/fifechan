#!/bin/sh
#
echo "Generating build information using aclocal, autoheader, automake and autoconf"
echo "This may take a while ..."

# Touch the timestamps on all the files since CVS messes them up
directory=`dirname $0`

touch $directory/*
touch $directory/examples/*
touch $directory/include/*
touch $directory/include/fifechan/*
touch $directory/include/fifechan/allegro/*
touch $directory/include/fifechan/opengl/*
touch $directory/include/fifechan/sdl/*
touch $directory/include/fifechan/widgets/*
touch $directory/src/*
touch $directory/src/allegro/*
touch $directory/src/opengl/*
touch $directory/src/sdl/*
touch $directory/src/widgets/*

# Regenerate configuration files
aclocal
autoheader
automake --foreign --include-deps --add-missing --copy
libtoolize --force
autoreconf -i

# Run configure for this platform
#./configure $*
echo "Now you are ready to run ./configure"
