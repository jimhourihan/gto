#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/lib

if GTO_BUILD_GTOIMAGE

bin_PROGRAMS = gtoimage

gtoimage_SOURCES = main.cpp
gtoimage_LDADD = $(top_srcdir)/lib/Gto/libGto.la -ltiff @LIBS@ 

endif
