#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/lib -I$(gto_maya_location)/include

CXXFLAGS = -fno-exceptions -D_BOOL -D__USER__=\"$$USER\"

lib_LTLIBRARIES = libgto.la

libgto_la_SOURCES = Writer.cpp Reader.cpp RawData.cpp Utilities.cpp
libgto_la_LIBS = @LIBS@

