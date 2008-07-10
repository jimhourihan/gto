#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/lib

if GTO_BUILD_RMAN

bin_PROGRAMS = RiGtoRibOut

RiGtoRibOut_SOURCES = main.cpp
RiGtoRibOut_LDADD = $(top_builddir)/lib/RiGto/libRiGto.la \
                    $(top_builddir)/lib/RiGtoStub/libRiGtoStub.la \
                    $(top_builddir)/lib/Gto/libGto.la \
                    $(rman_libs) \
                     @LIBS@ 

endif # GTO_BUILD_RMAN
