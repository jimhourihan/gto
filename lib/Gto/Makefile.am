#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

SUBDIRS = test

INCLUDES = -I$(top_srcdir)/lib
LIBS = -L$(top_srcdir)/lib

lib_LTLIBRARIES = libgto.la

libgto_la_SOURCES = Writer.cpp Reader.cpp RawData.cpp Utilities.cpp
libgto_la_LIBS = @LIBS@

