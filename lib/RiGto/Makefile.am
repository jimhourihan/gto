#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/lib

lib_LTLIBRARIES = libRiGto.la

libRiGto_la_SOURCES = RiGtoDataBase.cpp \
                      RiGtoNURBS.cpp \
                      RiGtoObject.cpp \
                      RiGtoOnOffList.cpp \
                      RiGtoPlugin.cpp \
                      RiGtoPoly.cpp \
                      RiGtoReader.cpp \
                      RiGtoSet.cpp \
                      RiGtoStrand.cpp \
                      RiGtoSubd.cpp

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
                 RiGtoSubd.h

libRiGto_la_LIBS = @LIBS@

