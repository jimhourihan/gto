#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/lib -I$(gto_maya_location)/include

AM_CXXFLAGS = -fno-exceptions -D_BOOL -D__USER__=\"$$USER\"

plugindir = $(MAYADIR)/bin/plug-ins/

if GTO_BUILD_MAYA

plugin_LIBRARIES = GtoIO.so

GtoIO_so_SOURCES = GtoInDataBase.cpp \
                   GtoInNURBS.cpp \
                   GtoInObject.cpp \
                   GtoInParticle.cpp \
                   GtoInPoly.cpp \
                   GtoInReader.cpp \
                   GtoInSet.cpp \
                   GtoInTransform.cpp \
                   GtoIO.cpp \
                   GtoOut.cpp \
                   version.cpp 

EXTRA_DIST = GtoInDataBase.h \
             GtoInNURBS.h \
             GtoInObject.h \
             GtoInParticle.h \
             GtoInPoly.h \
             GtoInReader.h \
             GtoInSet.h \
             GtoInTransform.h \
             GtoIO.h \
             GtoOut.h \
             version.h 


GtoIO_so_LIBS = -lGto -lOpenMaya -lOpenMayaAnim -lOpenMayaRender @LIBS@ -lm -lstdc++ 

endif
