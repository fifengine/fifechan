SUBDIRS = widgets @GUICHAN_EXTRADIRS@

lib_LTLIBRARIES = libguichan.la

libguichan_la_LIBADD = @GUICHAN_EXTRALIBS@ widgets/libguichan_widgets.la

libguichan_la_DEPENDENCIES = \
	widgets/libguichan_widgets.la

# The Gui-chan library sources
libguichan_la_SOURCES = 	\
	cliprectangle.cpp	\
	color.cpp		\
	defaultfont.cpp		\
	exception.cpp		\
	focushandler.cpp	\
	font.cpp		\
	graphics.cpp		\
	gui.cpp			\
	image.cpp		\
	imagefont.cpp		\
	input.cpp		\
	key.cpp			\
	keyinput.cpp		\
	mouseinput.cpp		\
	rectangle.cpp		\
	widget.cpp
