#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

include $(DEVKIT_LOCATION)/plug-ins/buildconfig

AM_CPPFLAGS = -I$(top_srcdir)/lib \
              $(CFLAGS)

AM_CXXFLAGS = -fno-exceptions -D__USER__=\"$$USER\"

plugindir = $(gto_maya_location)/bin/plug-ins
meldir = $(gto_maya_location)/scripts/others

dist_mel_SCRIPTS = gtoExportOptions.mel

plugin_PROGRAMS = GtoIO.so

GtoIO_so_SOURCES = GtoInDataBase.cpp \
                   GtoInNURBS.cpp \
                   GtoInObject.cpp \
                   GtoInParticle.cpp \
                   GtoInPoly.cpp \
                   GtoInReader.cpp \
                   GtoInSet.cpp \
                   GtoInTransform.cpp \
                   GtoInCamera.cpp \
                   GtoIO.cpp \
                   GtoOut.cpp \
                   version.cpp \
                   GtoMayaAttribute.cpp \
                   GtoAnimCurve.cpp \
                   TwkMaya.cpp

noinst_HEADERS = GtoInDataBase.h \
                 GtoInNURBS.h \
                 GtoInObject.h \
                 GtoInParticle.h \
                 GtoInPoly.h \
                 GtoInReader.h \
                 GtoInSet.h \
                 GtoInTransform.h \
                 GtoInCamera.h \
                 GtoIO.h \
                 GtoOut.h \
                 version.h \
                 GtoMayaAttribute.h \
                 GtoAnimCurve.h \
                 TwkMaya.h

GtoIO_so_LDFLAGS = $(LFLAGS) \
                   -L$(top_srcdir)/lib/Gto \
                   -L$(gto_maya_location)/lib \
                   -lGto \
                   -lImage \
                   -lOpenMaya \
                   -lOpenMayaAnim \
                   -lOpenMayaRender 

plugin_DATA = $(DEVKIT_LOCATION)/plug-ins/buildconfig
