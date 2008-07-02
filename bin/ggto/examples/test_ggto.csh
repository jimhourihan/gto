#!/bin/tcsh -f
#*******************************************************************************
# Copyright (c) 2001-2006 Tweak Inc. All rights reserved.
#*******************************************************************************

if ( ! $?H ) then
    echo Houdini environment variables not set
    exit -1
endif

#setenv LD_LIBRARY_PATH ${HDSO}:${LD_LIBRARY_PATH}

if ( $# != 1 ) then
    echo "Usage: ./test_ggto.csh <path_to_ggto>"
    exit 0
endif
rm -f convertedFromGto.geo convertedFromGeo.gto

set ggto = $1
set verbose=1

# alternatively 
# $ggto -a particles.gto convertedFromGto.geo
$ggto -a particles.gto stdout.geo > convertedFromGto.geo

# alternatively 
# $ggto -a particles.geo convertedFromGeo.gto
cat particles.geo | $ggto -a stdin.geo convertedFromGeo.gto

#$ggto -a -i convertedFromGeo.gto > convertedFromGto.geo

