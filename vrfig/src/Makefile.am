## Process this file with automake to produce Makefile.in
## $Id: Makefile.am,v 1.9 2001-05-19 21:15:16 jle Exp $

srcdir = @srcdir@
VPATH = @srcdir@

SUBDIRS = elements tools espws

LDADD = tools/libtools.a elements/libelements.a espws/libespws-fc.a

bin_PROGRAMS = vrfig

vrfig_SOURCES = vrfig.hpp vrfig.cc \
	mathutil.hpp mathutil.cc \
	flext.hpp flext.cc \
	Element.hpp Element.cc \
	Figure.hpp Figure.cc \
	FigureView.hpp FigureView.cc \
	Tool.hpp \
	Selectable.hpp Selectable.cc \
	Movable.hpp \
	Editor.hpp Editor.cc \
	ToolFactory.hpp ToolFactory.cc \
	ToolsChoice.hpp ToolsChoice.cc \
	MainView.hpp MainView.cc \
	main.cc
