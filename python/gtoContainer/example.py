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
# This isn't so much of an example in and of itself.  All it does create a
# gtoContainer called "gc", which loads the GTO file given on the command line.
# It then dumps you into the Python interpreter, so you can play around with
# the file interactively.  When you quit, you are given the option of writing
# the file back out again.  This can be used to manually edit any Object,
# Component, or Property of a GTO file.
# 


import sys
import code
import gto
from gtoContainer import *

#############################################
def interact(banner=None):
    import code, sys
    # use exception trick to pick up the current frame
    try:
        raise None
    except:
        frame = sys.exc_info()[2].tb_frame.f_back

    # evaluate commands in current namespace
    namespace = frame.f_globals.copy()
    namespace.update(frame.f_locals)

    code.interact(banner=banner, local=namespace)


#############################################
# Start here....

if( len(sys.argv) != 2 ):
    print
    print "Usage: example <gtofile>"
    print
    sys.exit(0)

gc = gtoContainer( sys.argv[1] )

for obj in gc:
    print obj
    for comp in obj:
        print "\t",comp
        for prop in comp:
            print "\t\t",prop

print
print ">>> gc"
print "<gtoContainer file: ../../examples/cube.gto>"

interact("")

action = raw_input( "\nWrite changes? (y/N/<filename>) " )

if( action == 'y' ):
    file.write( sys.argv[1] )
    print
    print "Wrote to", sys.argv[1]
    print

if( len(action) > 3 ):
    file.write( action )
    print
    print "Wrote to", action
    print

