## Process this file with automake to produce Makefile.in
## $Id: Makefile.am,v 1.5 2000-10-17 16:42:32 jle Exp $

srcdir = @srcdir@
VPATH = @srcdir@

FLUID = @FLUID@

bin_PROGRAMS = npuzzle

npuzzle_SOURCES = PuzzleGame.cc PuzzleGame.hpp \
	InstructionsWindow.cc InstructionsWindow.hpp \
	AboutWindow.cc AboutWindow.hpp \
	SettingsWindow.cc SettingsWindow.hpp \
	MainWindow.cc MainWindow.hpp \
	main.cc

EXTRA_DIST = MainWindow.cc MainWindow.hpp

MainWindow.cc: InstructionsWindow.hpp AboutWindow.hpp SettingsWindow.hpp
MainWindow.hpp: InstructionsWindow.hpp AboutWindow.hpp SettingsWindow.hpp

%.hpp: %.fl
	$(FLUID) -c $<

%.cc: %.fl
	$(FLUID) -c $<
