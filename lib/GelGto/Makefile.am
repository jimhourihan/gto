#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/lib

SUBDIRS = test

lib_LTLIBRARIES = libGelGto.la

libGelGto_la_SOURCES = DataBase.cpp Object.cpp Plugin.cpp Reader.cpp	\
Strand.cpp NURBS.cpp OnOffList.cpp Poly.cpp Set.cpp Subd.cpp

noinst_HEADERS = DataBase.h NURBS.h OnOffList.h Poly.h Set.h Subd.h	\
Exception.h Object.h Plugin.h Reader.h Strand.h

libGelGto_la_LIBS = @LIBS@

EXTRA_DIST = 
