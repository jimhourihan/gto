#!/usr/bin/env python2
import sys

from cgtypes import *
# You can get this module from:  http://cgkit.sourceforge.net/
# If you can't get it, just change all the vec3(x,x,x) to (x,x,x) in this file

import gto

print
print gto.__doc__
print

# *****************************************************************************
#
#                               gto.Writer example
#

# create an instance of the gto.Writer class
writer = gto.Writer()

# open a file
writer.open( "test.gto" )

# Declare the data structures -- see the gto file format spec
writer.beginObject( "test", "data", 1 )
writer.beginComponent( "component_1" )
writer.property( "property_1", gto.Float, 4, 4 )
writer.property( "property_2", gto.Int, 5, 2 )
writer.property( "property_3", gto.String, 2, 2 )
writer.endComponent()
writer.endObject()

writer.beginObject( "test2", "data", 1 )
writer.beginComponent( "component_1" )
writer.property( "property_1", gto.Double, 10 )
writer.property( "property_2", gto.Float, 10 )
writer.property( "property_3", gto.Int, 10 )
writer.property( "property_4", gto.Int, 2, 3 )
writer.endComponent()
writer.endObject()

# Make a couple of sets of dummy data.  Some things to note:
#
# - Tuples and lists are flattened out before they are written.  As long
#   as the number of atoms is equal to size x width, it'll work.
# 
# - propertyData must get exactly ONE parameter.  It can be any of the
#   following:
#       * A single int, float, string, etc.
#       * An instance of mat3, vec3, mat4, vec4, or quat
#       * A tuple or list of any combination of the above that 
#         makes sense
#       * DO NOT explicitly cast mat3 or mat4 into a tuple or list:
#         tuple(mat4(1))  It will be silently transposed (a bug in the
#         cgtypes code?).  ADDING it to a tuple or list is fine:  (mat4(1),)
#
adata = ( (1.0, 2.8), (3.6, 4.4), (5.2, 6), 7, vec3( 8, 9, 10 ) )
bdata = [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ]
cdata = ( mat4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16), )

# All strings MUST be registered before beginData() is called.  intern()
# can accept individual strings, tuples, or lists.  Dupes are OK.  Try it.
strings = ["Hello", "what's", "happening", "dude?"]
writer.intern( strings )

# Write the actual data.  Some checks are performed to make sure
# it matches the format declared above.  Try uncommenting some of the
# bogus propertyData() calles below and see.

writer.beginData()

# -- BEGIN BOGUS DATA -- 
#writer.propertyData( ( 1, 2, 3, 4, 5 ) )
#writer.propertyData( ( 1, 2, 3, 4, 5, 6, 7, 8, "bogus", "words" ) )
#writer.propertyData( writer )
# -- END BOGUS DATA -- 

writer.propertyData( cdata )
writer.propertyData( bdata )
writer.propertyData( strings )

writer.propertyData( adata )
writer.propertyData( adata )
writer.propertyData( bdata )
writer.propertyData( ( vec3( 1, 2, 3 ), vec3( 4, 5, 6 )  ) )
writer.endData()

# Close the file. Since Python uses garbage collection, you can never be
# sure when destructors are called, so it is recommended that you explicitly 
# close the file.  You have been warned.
writer.close()


# *****************************************************************************
#
#                               gto.Reader example
#

# For display purposes
types = [ "Int", "Float", "Double", "Half", 
          "String", "Boolean", "Short", "Byte" ]

#
# As in the C++ Gto::Reader class, you must derive from the base class.
# Here we just print the structure and data.  See the gto file format
# documentation for more info.
#
class myGtoReader( gto.Reader ):

    def object( self, name, protocol, protocolVersion, oinfo ):
        xname = self.stringFromId( oinfo.name )
        if( xname != name ):
            raise Exception, "\n\nSomething is broken in the string table!"
        print "object:", xname, " protocol:", protocol,
        print "  version:", protocolVersion,
        print " ", oinfo.numComponents, "components"
        return 1

    def component( self, name, cinfo ):
        print "component",name, cinfo.numProperties, "properties",
        if( cinfo.flags & gto.Transposed ):
            print "transposed",
        if( cinfo.flags & gto.Matrix ):
            print "matrix",
        print
        return 1

    def property( self, name, pinfo ):
        print "property", name, types[pinfo.type]+"["+str(pinfo.width)+"]",
        print  pinfo.size,"elements"
        return 1

    def dataRead( self, name, dataTuple, pinfo ):
        print "DATA: \""+name+"\"", types[pinfo.type]+"["+str(pinfo.width)+"]", 
        print pinfo.size,"elements: ",

        print dataTuple


# create an instance of the myGtoReader class
reader = myGtoReader()

# open the given file and set the wheels in motion...
reader.open("test.gto")

