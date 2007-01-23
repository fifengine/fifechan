AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_srcdir)/include

SUBDIRS = images sound

bin_PROGRAMS = guichanffdemo

guichanffdemodir = $(top_srcdir)

guichanffdemo_HEADERS =			\
	include/ffcharacterchooser.hpp	\
	include/ffcontainer.hpp		\
	include/ffdemo.hpp		\
	include/fflistbox.hpp		\
	include/ffscrollarea.hpp	\
	include/stringlistmodel.hpp	

guichanffdemo_SOURCES = 		\
	src/ffcharacterchooser.cpp	\
	src/ffcontainer.cpp		\
	src/ffdemo.cpp			\
	src/fflistbox.cpp		\
	src/ffscrollarea.cpp		\
	src/guichanffdemo.cpp

guichanffdemo_LDADD = @GUICHANFFDEMO_LIBS@


