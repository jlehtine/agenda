## Process this file with automake to produce Makefile.in
## $Id: Makefile.am,v 1.1 2001-05-19 19:12:40 jle Exp $

srcdir = @srcdir@
VPATH = @srcdir@

FLUID = @FLUID@

noinst_LIBRARIES = libespws-fc.a

libespws_fc_a_SOURCES = FileIcon.h FileIcon.cxx \
	FileBrowser.h FileBrowser.cxx \
	FileInput.h FileInput.cxx \
	FileChooser.h FileChooser.cxx FileChooser2.cxx

%.hpp: %.fl
	$(FLUID) -c $<

%.cc: %.fl
	$(FLUID) -c $<
