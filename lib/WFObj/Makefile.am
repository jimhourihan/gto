#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/lib


SUBDIRS = test

lib_LTLIBRARIES = libWFObj.la

libWFObj_la_SOURCES = Writer.cpp Reader.cpp Grammar.cpp Lexer.cpp
libWFObj_la_EXTRA = Grammar.y Lexer.l
libWFObj_la_LIBS = @LIBS@

