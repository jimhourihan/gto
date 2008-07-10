#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/lib
LIBS = -L$(top_builddir)/lib/Gto

check_PROGRAMS = test
TESTS = $(check_PROGRAMS)

test_SOURCES = main.cpp
test_LDADD = -lGto @LIBS@

