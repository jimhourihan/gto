#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/lib -I$(gto_maya_location)/include

AM_CXXFLAGS = -fno-exceptions -D_BOOL -D__USER__=\"$$USER\"

plugindir = $(gto_maya_location)/bin/plug-ins
meldir = $(gto_maya_location)/scripts/others

if GTO_BUILD_MAYA

dist_mel_SCRIPTS = GtoParticleExportUI.mel

plugin_PROGRAMS = GtoParticleExport.so

GtoParticleExport_so_SOURCES = GtoParticleExport.cpp ParticleAttribute.cpp	\
ParticleBucket.cpp

noinst_HEADERS = GtoParticleExport.h ParticleAttribute.h ParticleBucket.h

GtoParticleExport_so_LDFLAGS = -shared \
                   -L$(top_builddir)/lib/Gto \
                   -L$(gto_maya_location)/lib \
                   -lGto \
                   -lImage \
                   -lOpenMaya \
                   -lOpenMayaAnim \
                   -lOpenMayaRender 

endif
