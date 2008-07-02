#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/lib -I$(gto_maya_location)/include

AM_CXXFLAGS = -fno-exceptions -D_BOOL -D__USER__=\"$$USER\"

plugindir = $(gto_maya_location)/bin/plug-ins
meldir = $(gto_maya_location)/scripts/others

if GTO_BUILD_MAYA

dist_mel_SCRIPTS = AEGtoParticleDisplayCommon.mel			\
AEGtoParticleDisplayTemplate.mel AEGtoParticleDisplayInclude.mel	\
doGtoParticleDisplay.mel

plugin_PROGRAMS = GtoParticleDisplay.so

GtoParticleDisplay_so_SOURCES = GtoParticleDisplay.cpp  pluginMain.cpp

noinst_HEADERS = GtoParticleDisplay.h

GtoParticleDisplay_so_LDFLAGS = -shared \
                   -L$(top_srcdir)/lib/Gto \
                   -L$(gto_maya_location)/lib \
                   -lGto \
                   -lImage \
                   -lOpenMaya \
                   -lOpenMayaAnim \
                   -lOpenMayaRender 

endif
