#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

EXTRA_PROGRAMS = test

AM_CPPFLAGS = -I$(top_srcdir)/lib
LIBS = -L$(top_srcdir)/lib/Gto

TESTS = test

test_SOURCES = main.cpp
test_LDADD = -lgto -lz

