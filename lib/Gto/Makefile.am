#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

SUBDIRS = test

INCLUDES = -I$(top_srcdir)/lib
LIBS = -L$(top_srcdir)/lib

#include_HEADERS 
noinst_HEADERS = EXTProtocols.h Header.h Protocols.h RawData.h Reader.h Utilities.h Writer.h

lib_LIBRARIES = libgto.a

libgto_a_SOURCES = Writer.cpp Reader.cpp RawData.cpp Utilities.cpp
libgto_a_LIBS = @LIBS@

