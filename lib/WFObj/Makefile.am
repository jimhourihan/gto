#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/lib

lib_LTLIBRARIES = libWFObj.la

libWFObj_la_SOURCES = Reader.cpp Grammar.cpp Lexer.cpp
noinst_HEADERS = Grammar.h Reader.h

libWFObj_la_LIBS = @LIBS@

EXTRA_DIST = Grammar.y Lexer.l 


