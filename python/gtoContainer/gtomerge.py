#!/usr/bin/env python
#
# Copyright (C) 2004 Tweak Films
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA
#

# 
# Here is a re-implementation of gtomerge in Python, using gtoContainer.
#

import sys
from gtoContainer import *

try:
    i = sys.argv.index("-o")
    outFileName = sys.argv[i+1]
    del sys.argv[i]
    del sys.argv[i]
except:
    print "USAGE: "
    print "gtomerge.py -o outfile.gto infile1.gto infile2.gto ...\n"
    sys.exit(-1)

inputFileNames = sys.argv[1:]


outFile = gtoContainer()

for filename in inputFileNames:
    print "Reading input file %s..." % filename

    for object in gtoContainer( filename ):
        if object not in outFile:
            outFile.append( object.copy() )
            continue
        for component in object:
            if component not in outFile[object]:
                outFile[object].append( component.copy() )
                continue
            for property in component:
                if property not in outFile[object][component]:
                    outFile[object][component].append( property.copy() )


print "Writing output file %s..." % outFileName
outFile.write( outFileName )
