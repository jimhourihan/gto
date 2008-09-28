#!/bin/tcsh -f
#*******************************************************************************
# Copyright (c) 2001-2006 Tweak Inc. All rights reserved.
#*******************************************************************************

if ( ! $?H ) then
    echo Houdini environment variables not set
    exit -1
endif

setenv LD_LIBRARY_PATH ${HDSO}:${LD_LIBRARY_PATH}

if ( $# != 1 ) then
    echo "Usage: ./test_ggto.csh <path_to_ggto>"
    exit 0
endif
rm -f convertedFromGto.geo convertedFromGeo.gto

set ggto = $1
set verbose=1

$ggto -a -i particles.gto > convertedFromGto.geo

cat particles.geo | $ggto -a -o convertedFromGeo.gto

#$ggto -a -i convertedFromGeo.gto > convertedFromGto.geo

