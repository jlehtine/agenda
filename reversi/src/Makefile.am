## Process this file with automake to produce Makefile.in
## $Id: Makefile.am,v 1.2 2000-11-12 06:47:40 jle Exp $

srcdir = @srcdir@
VPATH = @srcdir@

FLUID = @FLUID@

bin_PROGRAMS = reversi

reversi_SOURCES = ReversiGame.cc ReversiGame.hpp \
	MainWindowBase.hpp \
	MainWindow.cc MainWindow.hpp \
	main.cc

main.cc: MainWindow.hpp

%.hpp: %.fl
	$(FLUID) -c $<

%.cc: %.fl
	$(FLUID) -c $<
