## Process this file with automake to produce Makefile.in
## $Id: Makefile.am,v 1.15 2001-06-10 18:36:43 jle Exp $

srcdir = @srcdir@
VPATH = @srcdir@

SUBDIRS = elements tools espws
EXTRA_DIST = icons/*.xbm

LDADD = tools/libtools.a elements/libelements.a espws/libespws-fc.a

bin_PROGRAMS = vrfig

vrfig_SOURCES = vrfig.hpp vrfig.cc \
	mathutil.hpp mathutil.cc \
	flext.hpp flext.cc \
	Point.hpp Point.cc \
	Element.hpp Element.cc \
	Figure.hpp Figure.cc \
	FigureView.hpp FigureView.cc \
	Tool.hpp \
	Selectable.hpp Selectable.cc \
	Movable.hpp \
	Controllable.hpp \
	Textual.hpp \
	Action.hpp Action.cc \
	ActionBuffer.hpp ActionBuffer.cc \
	Editor.hpp Editor.cc \
	ToolFactory.hpp ToolFactory.cc \
	ToolsChoice.hpp ToolsChoice.cc \
	ElementFactory.hpp ElementFactory.cc \
	MainView.hpp MainView.cc \
	main.cc
