#noinst_LTLIBRARIES = libsdl.la

lib_LTLIBRARIES = libfifechan_sdl.la

INCLUDES = -I$(top_srcdir)/include

libfifechan_sdl_la_LDFLAGS = -no-undefined -release $(LT_RELEASE) -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libfifechan_sdl_la_SOURCES =	\
	sdl.cpp			\
	sdlgraphics.cpp		\
	sdlimage.cpp		\
	sdlimageloader.cpp	\
	sdlinput.cpp

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = fifechan_sdl.pc

EXTRA_DIST = fifechan_sdl.pc.in
