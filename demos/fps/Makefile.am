AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_srcdir)/include

SUBDIRS = images sound

bin_PROGRAMS = guichanfpsdemo

guichanfpsdemodir = $(top_srcdir)

guichanfpsdemo_HEADERS = 		\
	include/fpsbutton.hpp		\
	include/fpscheckbox.hpp		\
	include/fpsdemo.hpp

guichanfpsdemo_SOURCES = 		\
	src/guichanfpsdemo.cpp		\
	src/fpscheckbox.cpp		\
	src/fpsdemo.cpp			\
	src/fpsbutton.cpp

guichanfpsdemo_LDADD = @GUICHANFPSDEMO_LIBS@


