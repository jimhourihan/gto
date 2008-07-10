#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/lib

bin_PROGRAMS = gto2obj

gto2obj_SOURCES = main.cpp
gto2obj_LDADD = $(top_builddir)/lib/Gto/libGto.la \
                $(top_builddir)/lib/WFObj/libWFObj.la \
                @LIBS@ 
