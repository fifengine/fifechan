SUBDIRS = widgets @FIFECHAN_EXTRADIRS@ hge openlayer contrib

INCLUDES = -I$(top_srcdir)/include

lib_LTLIBRARIES = libfifechan.la

libfifechan_la_LIBADD = widgets/libfifechan_widgets.la

libfifechan_la_LDFLAGS = -no-undefined -release $(LT_RELEASE) -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) 

libfifechan_la_DEPENDENCIES = \
	widgets/libfifechan_widgets.la

# The Gui-chan library sources
libfifechan_la_SOURCES = 	\
	actionevent.cpp		\
	cliprectangle.cpp	\
	color.cpp		\
	containerevent.cpp	\
	defaultfont.cpp		\
	event.cpp		\
	exception.cpp		\
	focushandler.cpp	\
	font.cpp		\
	genericinput.cpp	\
	graphics.cpp		\
	gui.cpp			\
	fifechan.cpp		\
	image.cpp		\
	imagefont.cpp		\
	inputevent.cpp		\
	key.cpp			\
	keyevent.cpp		\
	keyinput.cpp		\
	mouseevent.cpp		\
	mouseinput.cpp		\
	rectangle.cpp		\
	selectionevent.cpp	\
	text.cpp		\
	widget.cpp
