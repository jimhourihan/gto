#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/lib

SUBDIRS = test

lib_LTLIBRARIES = libGto.la

libGto_la_SOURCES = FlexLexer.cpp Parser.cpp Writer.cpp Reader.cpp	\
RawData.cpp Utilities.cpp

noinst_HEADERS = Parser.h FlexLexer.h

libGto_la_LIBS = @LIBS@

EXTRA_DIST = Parser.y Lexer.l 
