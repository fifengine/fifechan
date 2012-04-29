lib_LTLIBRARIES = libfifechan_opengl.la

INCLUDES = -I$(top_srcdir)/include

libfifechan_opengl_la_LDFLAGS = -no-undefined -release $(LT_RELEASE) -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) 

libfifechan_opengl_la_SOURCES =	\
	opengl.cpp		\
	openglimage.cpp		\
	openglgraphics.cpp

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = fifechan_opengl.pc

EXTRA_DIST = fifechan_opengl.pc.in
