#!/usr/bin/env python2
import sys
import gc

import gto

#
# This example can be used to verify that reading a file & writing it
# back out produces the same file.
#

if( len( sys.argv ) != 3 ):
    print
    print "Usage: example2.py infile.gto outfile.gto"
    print
    sys.exit(0)

gc.set_debug( gc.DEBUG_LEAK )

# *****************************************************************************
#
#  Read arg[1] & store in python variables
#

objects = {}

class myGtoReader( gto.Reader ):
    
    def __init__(self):
        #
        # Defining __init__ is NOT required, but if you do, you MUST
        # call gto.Reader.__init(self) from it
        #
        gto.Reader.__init__( self )
    
    def object( self, name, protocol, protocolVersion, oinfo ):
        objects[name] = { "_protocol" : protocol,
                          "_protocolVersion" : protocolVersion,
                          "_objectInfo" : oinfo }
        return 1

    def component( self, name, interp, cinfo ):
        objName = cinfo.object.name
        objects[objName][name] = { "_componentInfo" : cinfo }
        return 1

    def property( self, name, interp, pinfo ):
        objName = pinfo.component.object.name
        compName = pinfo.component.name
        objects[objName][compName][name] = { "_propertyInfo" : pinfo }
        return 1

    def dataRead( self, name, dataTuple, pinfo ):
        objName = pinfo.component.object.name
        compName = pinfo.component.name
        objects[objName][compName][name]["_data"] = dataTuple


reader = myGtoReader()
print "Reading",sys.argv[1]
sys.stdout.flush()
reader.open( sys.argv[1] )

# *****************************************************************************
#
# Write it back out as arg[2]
#

def strip( x ):
    if( x[0] == "_" ):
        return 0
    else:
        return 1

writer = gto.Writer()
writer.open( sys.argv[2] )
print "Writing",sys.argv[2]
sys.stdout.flush()

for objName in objects.keys():
    objProtocol = objects[objName]["_protocol"]
    objProtocolVersion = objects[objName]["_protocolVersion"]
    objNumComponents = objects[objName]["_objectInfo"].numComponents
    writer.beginObject( objName, objProtocol, objProtocolVersion )
    for compName in filter( strip, objects[objName] ):
        compNumProps = objects[objName][compName]["_componentInfo"].numProperties
        compFlags = objects[objName][compName]["_componentInfo"].flags
        writer.beginComponent( compName, compFlags )
        for propName in filter( strip, objects[objName][compName] ):
            propType = objects[objName][compName][propName]["_propertyInfo"].type
            propSize = objects[objName][compName][propName]["_propertyInfo"].size
            propWidth = objects[objName][compName][propName]["_propertyInfo"].width
            writer.property( propName, propType, propSize, propWidth )
            if( propType == gto.STRING ):
                writer.intern( objects[objName][compName][propName]["_data"][0] )
        writer.endComponent()
    writer.endObject()

writer.beginData()
for objName in objects.keys():
    for compName in filter( strip, objects[objName] ):
        for propName in filter( strip, objects[objName][compName] ):
            propData = objects[objName][compName][propName]["_data"]
            writer.propertyData( propData )
writer.endData()

writer.close()

print
