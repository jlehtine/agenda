## Process this file with automake to produce Makefile.in
## $Id: Makefile.am,v 1.1.1.1 2000-11-03 20:18:24 jle Exp $

srcdir = @srcdir@
VPATH = @srcdir@

FLUID = @FLUID@

bin_PROGRAMS = reversi

reversi_SOURCES = ReversiGame.cc ReversiGame.hpp

%.hpp: %.fl
	$(FLUID) -c $<

%.cc: %.fl
	$(FLUID) -c $<
