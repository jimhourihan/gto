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

dist_mel_SCRIPTS = AEGtoParticleDisplayCommon.mel			\
AEGtoParticleDisplayTemplate.mel AEGtoParticleDisplayInclude.mel	\
doGtoParticleDisplay.mel

plugin_PROGRAMS = GtoParticleDisplay.so

GtoParticleDisplay_so_SOURCES = GtoParticleDisplay.cpp  pluginMain.cpp

noinst_HEADERS = GtoParticleDisplay.h

GtoParticleDisplay_so_LDFLAGS = $(LFLAGS) \
                   -L$(top_srcdir)/lib/Gto \
                   -L$(gto_maya_location)/lib \
                   -lGto \
                   -lImage \
                   -lOpenMaya \
                   -lOpenMayaAnim \
                   -lOpenMayaRender 
