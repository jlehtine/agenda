## Process this file with automake to produce Makefile.in
## $Id: Makefile.am,v 1.8 2000-10-29 18:02:44 jle Exp $

srcdir = @srcdir@
VPATH = @srcdir@

FLUID = @FLUID@

bin_PROGRAMS = npuzzle

npuzzle_SOURCES = PuzzleGame.cc PuzzleGame.hpp \
	InstructionsWindow.cc InstructionsWindow.hpp \
	AboutWindow.cc AboutWindow.hpp \
	SettingsWindow.cc SettingsWindow.hpp \
	PuzzleBox.cc PuzzleBox.hpp \
	RightAlignedOutput.cc RightAlignedOutput.hpp \
	MainWindow.cc MainWindow.hpp \
	main.cc

EXTRA_DIST = MainWindow.fl MainWindow.cc MainWindow.hpp \
	InstructionsWindow.fl InstructionsWindow.cc InstructionsWindow.hpp \
	AboutWindow.fl AboutWindow.cc AboutWindow.hpp \
	SettingsWindow.fl SettingsWindow.cc SettingsWindow.hpp

MainWindow.cc: InstructionsWindow.hpp AboutWindow.hpp SettingsWindow.hpp
MainWindow.hpp: InstructionsWindow.hpp AboutWindow.hpp SettingsWindow.hpp

%.hpp: %.fl
	$(FLUID) -c $<

%.cc: %.fl
	$(FLUID) -c $<
