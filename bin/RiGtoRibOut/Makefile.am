#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/lib

bin_PROGRAMS = RiGtoRibOut

RiGtoRibOut_SOURCES = main.cpp
RiGtoRibOut_LDADD = $(top_srcdir)/lib/Gto/libGto.la \
                    $(top_srcdir)/lib/RiGto/libRiGto.la \
                    $(top_srcdir)/lib/RiGtoStub/libRiGtoStub.la \
                     @LIBS@ 
