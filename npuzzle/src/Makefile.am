## Process this file with automake to produce Makefile.in
## $Id: Makefile.am,v 1.6 2000-10-28 11:23:59 jle Exp $

srcdir = @srcdir@
VPATH = @srcdir@

FLUID = @FLUID@

bin_PROGRAMS = npuzzle

npuzzle_SOURCES = PuzzleGame.cc PuzzleGame.hpp \
	InstructionsWindow.cc InstructionsWindow.hpp \
	AboutWindow.cc AboutWindow.hpp \
	SettingsWindow.cc SettingsWindow.hpp \
	MainWindow.cc MainWindow.hpp \
	PuzzleBox.cc PuzzleBox.hpp \
	main.cc

EXTRA_DIST = MainWindow.cc MainWindow.hpp \
	InstructionsWindow.cc InstructionsWindow.hpp \
	AboutWindow.cc AboutWindow.hpp \
	SettingsWindow.cc SettingsWindow.hpp

MainWindow.cc: InstructionsWindow.hpp AboutWindow.hpp SettingsWindow.hpp
MainWindow.hpp: InstructionsWindow.hpp AboutWindow.hpp SettingsWindow.hpp

%.hpp: %.fl
	$(FLUID) -c $<

%.cc: %.fl
	$(FLUID) -c $<
