## Process this file with automake to produce Makefile.in
## $Id: Makefile.am,v 1.2 2001-05-23 12:30:52 jle Exp $

srcdir = @srcdir@
VPATH = @srcdir@

RM = @RM@
MKDIR = @MKDIR@
FIND = @FIND@
CP = @CP@
DOCPLUSPLUS = @DOCPLUSPLUS@

FIND_DOCPLUSPLUS = src -name '*.hpp'

SUBDIRS = src

docplusplus:
	$(MKDIR) doc || true
	$(MKDIR) doc/developer || true
	$(MKDIR) doc/developer/api || true
	$(RM) -rf doc/developer/api/* || true
	$(DOCPLUSPLUS) -H -R -v -d doc/developer/api `$(FIND) $(FIND_DOCPLUSPLUS) -print`

dist-hook: docplusplus
	$(CP) -pr $(srcdir)/doc $(distdir)/

.PHONY: docplusplus
