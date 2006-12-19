#!/bin/sh
#
echo "Generating build information using aclocal, autoheader, automake and autoconf"
echo "This may take a while ..."

# Touch the timestamps on all the files since CVS messes them up
directory=`dirname $0`

touch $directory/*
touch $directory/examples/*
touch $directory/include/*
touch $directory/include/guichan/*
touch $directory/include/guichan/allegro/*
touch $directory/include/guichan/opengl/*
touch $directory/include/guichan/sdl/*
touch $directory/include/guichan/widgets/*
touch $directory/include/guichan/x/*
touch $directory/src/*
touch $directory/src/allegro/*
touch $directory/src/opengl/*
touch $directory/src/sdl/*
touch $directory/src/widgets/*
touch $directory/src/x/*

# Regenerate configuration files
aclocal
autoheader
automake --foreign --include-deps --add-missing --copy
libtoolize --force
autoreconf -i

# Run configure for this platform
#./configure $*
echo "Now you are ready to run ./configure"
