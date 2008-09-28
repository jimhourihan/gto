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

dist_mel_SCRIPTS = AEGtoDefomerCommon.mel \
		   AEGtoDefomerTemplate.mel \
		   applyGtoDeformer.mel

plugin_PROGRAMS = GtoDeformer.so

GtoDeformer_so_SOURCES = GtoDeformer.cpp \
			GtoPointReader.cpp \
			pluginMain.cpp 

noinst_HEADERS = GtoDeformer.h  GtoPointReader.h

GtoDeformer_so_LDFLAGS = $(LFLAGS) \
                   -L$(top_srcdir)/lib/Gto \
                   -L$(gto_maya_location)/lib \
                   -lGto \
                   -lOpenMaya \
                   -lOpenMayaAnim
 

plugin_DATA = $(DEVKIT_LOCATION)/plug-ins/buildconfig

#######################################################################

# include $(DEVKIT_LOCATION)/plug-ins/buildconfig
# 
# AM_CPPFLAGS = -I$(top_srcdir)/lib \
#               $(CFLAGS)
# 
# AM_CXXFLAGS = -fno-exceptions -D__USER__=\"$$USER\"
# 
# plugindir = $(gto_maya_location)/bin/plug-ins
# meldir = $(gto_maya_location)/scripts/others
# 
# dist_mel_SCRIPTS = AEGtoDefomerCommon.mel \
# 		   AEGtoDefomerTemplate.mel \
# 		   applyGtoDeformer.mel
# 
# plugin_PROGRAMS = GtoDeformer.so
# 
# GtoDeformer_so_SOURCES = GtoDeformer.cpp \
# 			GtoPointReader.cpp \
# 			pluginMain.cpp 
# 
# noinst_HEADERS = GtoDeformer.h  GtoPointReader.h
# 
# GtoDeformer_so_LDFLAGS = $(LFLAGS) \
#                    -L$(top_srcdir)/lib/Gto \
#                    -L$(gto_maya_location)/lib \
#                    -lGto \
#                    -lImage \
#                    -lOpenMaya \
#                    -lOpenMayaAnim \
#                    -lOpenMayaRender 
