#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/lib -I$(gto_rman_location)/include

AM_CXXFLAGS = 

plugindir = $(gto_rman_location)/bin/plug-ins

if GTO_BUILD_RMAN

plugin_PROGRAMS = RiGto.so

RiGto_so_SOURCES = plugin.cpp 

RiGto_so_LDADD = $(top_srcdir)/lib/Gto/libGto.la \
                 $(top_srcdir)/lib/RiGto/libRiGto.la \
                 $(top_srcdir)/lib/RiGtoStub/libRiGtoStub.la 

RiGto_so_LDFLAGS = -shared 

endif # GTO_BUILD_RMAN
