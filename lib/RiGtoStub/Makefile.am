#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/lib \
              -I$(gto_rman_location)/include

if GTO_BUILD_RMAN

lib_LTLIBRARIES = libRiGtoStub.la

libRiGtoStub_la_SOURCES = Stubs.cpp

noinst_HEADERS = Stubs.h

libRiGtoStub_la_LIBS = $(rman_libs) @LIBS@

endif # GTO_BUILD_RMAN
