#!/bin/sh
# guichan.sh by Danny Angelo Carminati Grein (danny@bitcrafters.com.br)

## Configure the build.
LDFLAGS="-L$(psp-config --pspsdk-path)/lib -fno-exceptions -lc -lc -lpspuser" ./configure --host psp --prefix=$(psp-config --psp-prefix) --enable-pspirkeyb --enable-sdl --enable-sdlimage --enable-psp || { exit 1; }

## Compile and install.
make clean && make -j2 && make install && make clean || { exit 1; }
