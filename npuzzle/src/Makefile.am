## Process this file with automake to produce Makefile.in
## $Id: Makefile.am,v 1.3 2000-10-16 18:47:37 jle Exp $

srcdir = @srcdir@
VPATH = @srcdir@

FLUID = @FLUID@

bin_PROGRAMS = npuzzle

npuzzle_SOURCES = PuzzleGame.cc PuzzleGame.hpp MainWindow.cc MainWindow.hpp \
	main.cc

EXTRA_DIST = MainWindow.cc MainWindow.hpp

%.hpp: %.fl
	$(FLUID) -c $<

%.cc: %.fl
	$(FLUID) -c $<
