AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_srcdir)/include

SUBDIRS = images sound

bin_PROGRAMS = fifechanffdemo

fifechanffdemodir = $(top_srcdir)

fifechanffdemo_HEADERS =			\
	include/ffcharacterchooser.hpp	\
	include/ffcontainer.hpp		\
	include/ffdemo.hpp		\
	include/fflistbox.hpp		\
	include/ffscrollarea.hpp	\
	include/stringlistmodel.hpp	

fifechanffdemo_SOURCES = 		\
	src/ffcharacterchooser.cpp	\
	src/ffcontainer.cpp		\
	src/ffdemo.cpp			\
	src/fflistbox.cpp		\
	src/ffscrollarea.cpp		\
	src/fifechanffdemo.cpp

fifechanffdemo_LDADD = @FIFECHANFFDEMO_LIBS@


