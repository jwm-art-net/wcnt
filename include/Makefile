# Generated automatically from Makefile.in by configure.
# Makefile.in generated automatically by automake 1.4 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.


SHELL = /bin/sh

srcdir = .
top_srcdir = ..
prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/wcnt
pkglibdir = $(libdir)/wcnt
pkgincludedir = $(includedir)/wcnt

top_builddir = ..

ACLOCAL = aclocal
AUTOCONF = autoconf
AUTOMAKE = automake
AUTOHEADER = autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL_PROGRAM}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
host_alias = i686-pc-linux-gnu
host_triplet = i686-pc-linux-gnu
AS = @AS@
CC = gcc
CXX = c++
DLLTOOL = @DLLTOOL@
ECHO = echo
EXEEXT = 
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LN_S = ln -s
MAKEINFO = makeinfo
NO_PREFIX_PACKAGE_DATA_DIR = share/wcnt
NO_PREFIX_PACKAGE_DOC_DIR = doc/wcnt
NO_PREFIX_PACKAGE_HELP_DIR = share/wcnt/help
NO_PREFIX_PACKAGE_MENU_DIR = share/wcnt
NO_PREFIX_PACKAGE_PIXMAPS_DIR = share/wcnt/pixmaps
OBJDUMP = @OBJDUMP@
OBJEXT = o
PACKAGE = wcnt
PACKAGE_DATA_DIR = /usr/local/share/wcnt
PACKAGE_DOC_DIR = /usr/local/doc/wcnt
PACKAGE_HELP_DIR = /usr/local/share/wcnt/help
PACKAGE_MENU_DIR = /usr/local/share/wcnt
PACKAGE_PIXMAPS_DIR = /usr/local/share/wcnt/pixmaps
RANLIB = ranlib
STRIP = strip
VERSION = 1.0001c

wcnt_includedir = $(pkgincludedir)

wcnt_include_DATA =  	adsr.h 	adsrcoord.h 	clockclock.h 	conversions.h 	inputnames.h 	iocat.h 	lfoclock.h 	lfocontroller.h 	linkedlist.h 	filtercommon.h 	lowpassfilter.h 	modinput.h 	modinputslist.h 	modoutput.h 	modoutputslist.h 	noisegenerator.h 	nonezero.h 	notedata.h 	oscclock.h 	outputnames.h 	riffdata.h 	sampler.h 	sequencer.h 	sinewave.h 	squarewave.h 	stereoamp.h 	stereochannel.h 	stereomixer.h 	synthmodnames.h 	synthmodule.h 	wavfileheader.h 	wavfilein.h 	wavfileout.h 	dtr.h 	synthfilereader.h 	constmod.h 	freqgenerator.h 	modifier.h 	samplehold.h 	trigger.h 	trianglewave.h 	trianglewave2.h 	userwave.h 	userwavevertex.h 	connector.h 	synthmodulelist.h 	modules.h 	jwmsynth.h 	connectorlist.h 	paramnames.h 	modparam.h 	modparamlist.h 	randomtrigger.h 	logictrigger.h 	switcher.h 	wcntsignal.h 	highpassfilter.h 	combiner.h 	bpmchange.h         meterchange.h 	timemap.h 	sampledata.h 	riffnode.h 	delay.h 	echo.h 	monoamp.h 	wcntmodule.h 	multiplier.h 	rangelimit.h 	pan.h 	rms.h 	dc_filter.h 	dataobjects.h 	dobj.h 	dobjlist.h 	dobjnames.h 	dobjparam.h 	dobjparamlist.h 	dparnames.h 	moddobj.h 	moddobjlist.h


EXTRA_DIST = $(wcnt_include_DATA)
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = ../config.h
CONFIG_CLEAN_FILES = 
DATA =  $(wcnt_include_DATA)

DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = tar
GZIP_ENV = --best
all: all-redirect
.SUFFIXES:
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnu --include-deps include/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


install-wcnt_includeDATA: $(wcnt_include_DATA)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(wcnt_includedir)
	@list='$(wcnt_include_DATA)'; for p in $$list; do \
	  if test -f $(srcdir)/$$p; then \
	    echo " $(INSTALL_DATA) $(srcdir)/$$p $(DESTDIR)$(wcnt_includedir)/$$p"; \
	    $(INSTALL_DATA) $(srcdir)/$$p $(DESTDIR)$(wcnt_includedir)/$$p; \
	  else if test -f $$p; then \
	    echo " $(INSTALL_DATA) $$p $(DESTDIR)$(wcnt_includedir)/$$p"; \
	    $(INSTALL_DATA) $$p $(DESTDIR)$(wcnt_includedir)/$$p; \
	  fi; fi; \
	done

uninstall-wcnt_includeDATA:
	@$(NORMAL_UNINSTALL)
	list='$(wcnt_include_DATA)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(wcnt_includedir)/$$p; \
	done
tags: TAGS
TAGS:


distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)

subdir = include

distdir: $(DISTFILES)
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done
info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
check: check-am
installcheck-am:
installcheck: installcheck-am
install-exec-am:
install-exec: install-exec-am

install-data-am: install-wcnt_includeDATA
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am: uninstall-wcnt_includeDATA
uninstall: uninstall-am
all-am: Makefile $(DATA)
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:
	$(mkinstalldirs)  $(DESTDIR)$(wcnt_includedir)


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-generic mostlyclean-am

clean: clean-am

distclean-am:  distclean-generic clean-am
	-rm -f libtool

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: uninstall-wcnt_includeDATA install-wcnt_includeDATA tags distdir \
info-am info dvi-am dvi check check-am installcheck-am installcheck \
install-exec-am install-exec install-data-am install-data install-am \
install uninstall-am uninstall all-redirect all-am all installdirs \
mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean

	dobjdobj.h\
	dobjdobjlist.h\
	dobjmod.h\
	dynvertex.h\
	dynamic.h\
	spreader.h\
	notetran.h

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
