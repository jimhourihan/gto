#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/lib -I$(gto_rman_location)/include

AM_CXXFLAGS = 

plugindir = $(gto_rman_location)/bin/plug-ins

if GTO_BUILD_RMAN

plugin_PROGRAMS = RiGto.so

RiGto_so_SOURCES = plugin.cpp \
                   RiGtoDataBase.cpp \
                   RiGtoNURBS.cpp \
                   RiGtoObject.cpp \
                   RiGtoOnOffList.cpp \
                   RiGtoPlugin.cpp \
                   RiGtoPoly.cpp \
                   RiGtoReader.cpp \
                   RiGtoSet.cpp \
                   RiGtoStrand.cpp \
                   RiGtoSubd.cpp \
                   Stubs.cpp 

noinst_HEADERS = RiGtoDataBase.h \
                 RiGtoException.h \
                 RiGtoNURBS.h \
                 RiGtoObject.h \
                 RiGtoOnOffList.h \
                 RiGtoPlugin.h \
                 RiGtoPoly.h \
                 RiGtoReader.h \
                 RiGtoSet.h \
                 RiGtoStrand.h \
                 RiGtoSubd.h \
                 Stubs.h 

RiGtoRibOut_LDADD = $(top_srcdir)/lib/Gto/libGto.la \
                    $(top_srcdir)/lib/RiGto/libRiGto.la \
                    $(top_srcdir)/lib/RiGtoStub/libRiGtoStub.la \

RiGto_so_LDFLAGS = -shared 

endif # GTO_BUILD_RMAN
