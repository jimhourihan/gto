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

RiGto_so_LDFLAGS = -shared \
                   -L$(top_srcdir)/lib/Gto \
                   -L$(gto_rman_location)/lib \
                   -lGto 

endif
