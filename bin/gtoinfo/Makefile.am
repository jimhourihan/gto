#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_srcdir)/lib
LIBS = -L$(top_srcdir)/lib/Gto

bin_PROGRAMS = gtoinfo

gtoinfo_SOURCES = main.cpp
gtoinfo_LDADD = -lgto @LIBS@
