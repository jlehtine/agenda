## Process this file with automake to produce Makefile.in
## $Id: Makefile.am,v 1.7 2001-05-17 19:35:27 jle Exp $

srcdir = @srcdir@
VPATH = @srcdir@

SUBDIRS = elements tools

LDADD = elements/libelements.a tools/libtools.a

bin_PROGRAMS = vrfig

vrfig_SOURCES = vrfig.hpp vrfig.cc \
	mathutil.hpp mathutil.cc \
	flext.hpp flext.cc \
	Element.hpp Element.cc \
	Figure.hpp Figure.cc \
	FigureView.hpp FigureView.cc \
	Tool.hpp \
	Editor.hpp Editor.cc \
	ToolFactory.hpp ToolFactory.cc \
	ToolsChoice.hpp ToolsChoice.cc \
	MainView.hpp MainView.cc \
	main.cc
