AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_srcdir)/include

SUBDIRS = images sound

bin_PROGRAMS = fifechanfpsdemo

fifechanfpsdemodir = $(top_srcdir)

fifechanfpsdemo_HEADERS = 		\
	include/fpsbutton.hpp		\
	include/fpscheckbox.hpp		\
	include/fpsdemo.hpp

fifechanfpsdemo_SOURCES = 		\
	src/fifechanfpsdemo.cpp		\
	src/fpscheckbox.cpp		\
	src/fpsdemo.cpp			\
	src/fpsbutton.cpp

fifechanfpsdemo_LDADD = @FIFECHANFPSDEMO_LIBS@


