## Process this file with automake to produce Makefile.in
## $Id: Makefile.am,v 1.13 2001-05-25 22:30:02 jle Exp $

srcdir = @srcdir@
VPATH = @srcdir@

SUBDIRS = elements tools espws
EXTRA_DIST = icons/*.xbm

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
	Controllable.hpp \
	Textual.hpp \
	Editor.hpp Editor.cc \
	ToolFactory.hpp ToolFactory.cc \
	ToolsChoice.hpp ToolsChoice.cc \
	ElementFactory.hpp ElementFactory.cc \
	MainView.hpp MainView.cc \
	main.cc
