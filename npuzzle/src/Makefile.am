## Process this file with automake to produce Makefile.in
## $Id: Makefile.am,v 1.9 2001-01-28 15:49:20 jle Exp $

srcdir = @srcdir@
VPATH = @srcdir@

FLUID = @FLUID@

bin_PROGRAMS = npuzzle

npuzzle_SOURCES = PuzzleGame.cc PuzzleGame.hpp \
	InstructionsWindow.cc InstructionsWindow.hpp \
	AboutWindow.cc AboutWindow.hpp \
	PuzzleBox.cc PuzzleBox.hpp \
	RightAlignedOutput.cc RightAlignedOutput.hpp \
	MainWindow.cc MainWindow.hpp \
	main.cc

EXTRA_DIST = MainWindow.fl MainWindow.cc MainWindow.hpp \
	InstructionsWindow.fl InstructionsWindow.cc InstructionsWindow.hpp \
	AboutWindow.fl AboutWindow.cc AboutWindow.hpp

MainWindow.cc: InstructionsWindow.hpp AboutWindow.hpp
MainWindow.hpp: InstructionsWindow.hpp AboutWindow.hpp

%.hpp: %.fl
	$(FLUID) -c $<

%.cc: %.fl
	$(FLUID) -c $<
