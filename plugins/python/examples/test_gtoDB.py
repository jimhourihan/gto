#!/usr/bin/env python
"""

Python unittest for the GtoDB module.

"""

import sys
import os
import string
import unittest
from random import random
from tempfile import mkstemp

import gtoDB

# ---------------------------------------------------------------------------------------------
# Globals
# ---------------------------------------------------------------------------------------------

randomFloats = [random() for i in range(66)]

test_Property_specs = [

    { 'args'      : ("foo", "bar", gtoDB.String, 1, 1),
      'dataIn'    : ("something",),
      'name'      : "foo",
      'interp'    : "bar",
      'type'      : gtoDB.String,
      'size'      : 1,
      'width'     : 1,
      'dataOut'   : ("something",),
    },

    { 'args'      : ("foo", gtoDB.String, 1, 1),
      'dataIn'    : ("something",),
      'name'      : "foo",
      'interp'    : "",
      'type'      : gtoDB.String,
      'size'      : 1,
      'width'     : 1,
      'dataOut'   : ("something",),
    },

    { 'args'      : ("foo", "bar", ("something",)),
      'name'      : "foo",
      'interp'    : "bar",
      'type'      : gtoDB.String,
      'size'      : 1,
      'width'     : 1,
      'dataOut'   : ("something",),
    },

    { 'args'      : ("foo", "something"),
      'name'      : "foo",
      'interp'    : "",
      'type'      : gtoDB.String,
      'size'      : 1,
      'width'     : 1,
      'dataOut'   : ("something",),
    },

    { 'args'      : ("foo", None),
      'exception' : ValueError,
    },

    { 'args'      : ("foo", "bar", None),
      'exception' : ValueError,
    },
    
    { 'args'      : ("foo", []),
      'exception' : ValueError,
    },

    { 'args'      : ("foo", gtoDB.ErrorType, 0, 0),
      'exception' : ValueError,
    },

    { 'args'      : ("foo", gtoDB.Int, 0, 0),
      'name'      : "foo",
      'interp'    : "",
      'type'      : gtoDB.Int,
      'size'      : 0,
      'width'     : 0,
      'dataOut'   : None,
    },

    { 'args'      : ("foo", ["something"]),
      'name'      : "foo",
      'interp'    : "",
      'type'      : gtoDB.String,
      'size'      : 1,
      'width'     : 1,
      'dataOut'   : ("something",),
    },

    { 'args'      : ("foo", ["something"] * 100),
      'name'      : "foo",
      'interp'    : "",
      'type'      : gtoDB.String,
      'size'      : 100,
      'width'     : 1,
      'dataOut'   : ("something",)*100,
    },

    { 'args'      : ("foo", [range(2)] * 14),
      'name'      : "foo",
      'interp'    : "",
      'type'      : gtoDB.Int,
      'size'      : 14,
      'width'     : 2,
      'dataOut'   : ((0,1),) * 14,
    },

    { 'args'      : ("foo", randomFloats),
      'name'      : "foo",
      'interp'    : "",
      'type'      : gtoDB.Float,
      'size'      : len(randomFloats),
      'width'     : 1,
      'dataOut'   : randomFloats,
    },

    { 'args'      : ("foo", gtoDB.Double, len(randomFloats)),
      'dataIn'    : randomFloats,
      'name'      : "foo",
      'interp'    : "",
      'type'      : gtoDB.Double,
      'size'      : len(randomFloats),
      'width'     : 1,
      'dataOut'   : randomFloats,
    },

    { 'args'      : ("foo", gtoDB.Byte, 4),
      'dataIn'    : (1, -1, 255, 312),
      'name'      : "foo",
      'interp'    : "",
      'type'      : gtoDB.Byte,
      'size'      : 4,
      'width'     : 1,
      'dataOut'   : (1, 255, 255, 56),
    },
    
    { 'args'      : ("foo", gtoDB.Short, 2, 2),
      'dataIn'    : ((0, -1), (32768, 65537)),
      'name'      : "foo",
      'interp'    : "",
      'type'      : gtoDB.Short,
      'size'      : 2,
      'width'     : 2,
      'dataOut'   : ((0, 65535), (32768, 1)),
    },

    { 'args'      : ("foo", xrange(10000)),
      'name'      : "foo",
      'interp'    : "",
      'type'      : gtoDB.Int,
      'size'      : 10000,
      'width'     : 1,
      'dataOut'   : xrange(10000),
    },

    { 'args'      : (string.letters*10, string.punctuation*10, ['a']),
      'name'      : string.letters*10,
      'interp'    : string.punctuation*10,
      'type'      : gtoDB.String,
      'size'      : 1,
      'width'     : 1,
      'dataOut'   : ('a',),
    },

    { 'args'      : ("foo", gtoDB.Int, 4, 1),
      'name'      : "foo",
      'interp'    : "",
      'type'      : gtoDB.Int,
      'size'      : 4,
      'width'     : 1,
      'dataIn'    : (1.1, 1.5, 1.95, 2.001),
      'dataOut'   : (1, 1, 1, 2),   # Cast to int 
    },

    { 'args'      : ("foo", gtoDB.Int, 1, 4),
      'name'      : "foo",
      'interp'    : "",
      'type'      : gtoDB.Int,
      'size'      : 1,
      'width'     : 4,
      'dataIn'    : (1, 2, 3),
      'exception' : ValueError,  # Property width mismatch 
    },

    { 'args'      : ("foo", gtoDB.Int, 2, 2),
      'name'      : "foo",
      'interp'    : "",
      'type'      : gtoDB.Int,
      'size'      : 2,
      'width'     : 2,
      'dataIn'    : ((1, 2), (3, 4, 5)),
      'exception' : ValueError,  # Property width mismatch 
    },

    { 'args'      : ("foo", gtoDB.Int, 2),
      'name'      : "foo",
      'interp'    : "",
      'type'      : gtoDB.Int,
      'size'      : 2,
      'width'     : 1,
      'dataIn'    : (1, 2, 3),
      'exception' : IndexError,  # Too many values 
    },

    { 'args'      : ("foo", 4, ("bar", "baz")),
      'name'      : "foo",
      'interp'    : "",
      'type'      : gtoDB.String,
      'size'      : 2,
      'width'     : 1,
      'exception' : NotImplementedError,  # '4' not of type Gto::DataType 
    },
]


test_Component_specs = [

    { 'args'      : ("foo",),
      'name'      : "foo",
      'interp'    : "",
      'flags'     : 0,
    },

    { 'args'      : ("foo", 1),
      'name'      : "foo",
      'interp'    : "",
      'flags'     : 1,
    },

    { 'args'      : ("foo", "bar"),
      'name'      : "foo",
      'interp'    : "bar",
      'flags'     : 0,
    },

    { 'args'      : ("foo", "bar", 1),
      'name'      : "foo",
      'interp'    : "bar",
      'flags'     : 1,
    },

    { 'args'      : (),
      'exception' : NotImplementedError,
    },

    { 'args'      : (1, 2, "foo"),
      'exception' : NotImplementedError,
    },

    { 'args'      : ("foo",),
      'name'      : "foo",
      'interp'    : "",
      'flags'     : 0,
      'propsIn'   : (gtoDB.Property("foo", 0), gtoDB.Property("bar", "str")),
    },

    { 'args'      : ("foo",),
      'name'      : "foo",
      'interp'    : "",
      'flags'     : 0,
      'propsIn'   : (gtoDB.Property("foo", 0), gtoDB.Property("foo", "str")),
      'exception' : ValueError,  # Duplicate property name
    },

    { 'args'      : ("foo",),
      'name'      : "foo",
      'interp'    : "",
      'flags'     : 0,
      'propsIn'   : ("NotAProperty",),
      'exception' : ValueError,
    },
]


test_Object_specs = [

    { 'args'            : ("foo", "test", 1),
      'name'            : "foo",
      'protocol'        : "test",
      'protocolVersion' : 1,
    },

    { 'args'            : (),
      'exception'       : TypeError,
    },

    { 'args'            : ("foo", "test", 12),
      'name'            : "foo",
      'protocol'        : "test",
      'protocolVersion' : 12,
      'compsIn'         : (gtoDB.Component("foo"), gtoDB.Component("bar")),
    },

    { 'args'            : ("foo", "test", 12),
      'name'            : "foo",
      'protocol'        : "test",
      'protocolVersion' : 12,
      'compsIn'         : (gtoDB.Component("foo"), gtoDB.Component("foo")),
      'exception'       : ValueError,  # Duplicate component name
    },

    { 'args'            : ("foo", "test", 12),
      'name'            : "foo",
      'protocol'        : "test",
      'protocolVersion' : 12,
      'compsIn'         : ("NotAComponent",),
      'exception'       : ValueError,
    },
]



# ---------------------------------------------------------------------------------------------
# Test Class
# ---------------------------------------------------------------------------------------------
class test_gto(unittest.TestCase):

    def assertEqual(self, a, b, msg=None):
        xmsg = "Assertion failed:\n\n"
        if xmsg is not None:
            xmsg += "    test: %s\n\n" % msg
        xmsg += "    expected: %s\n" % repr(b) + \
                "    got:      %s\n" % repr(a)
        unittest.TestCase.assertEqual(self, a, b, xmsg)
    

    def test_Property(self):

        for spec in test_Property_specs:
            exception = spec.get("exception", None)
            try:
                p = gtoDB.Property(*spec['args'])

                if "dataIn" in spec:
                    for i, v in enumerate(spec["dataIn"]):
                        p[i] = v

                    # Excercise slice set/get a little:
                    pp = gtoDB.Property(*spec['args'])
                    pp[:] = spec['dataIn']

                    self.assertEqual(p[:], pp[:], spec)

                self.assertEqual(p.name, spec["name"], spec)
                self.assertEqual(p.interp, spec["interp"], spec)
                self.assertEqual(p.type, spec["type"], spec)
                self.assertEqual(p.size, spec["size"], spec)
                self.assertEqual(p.width, spec["width"], spec)
                self.assertEqual(len(p), spec["size"])

                if spec["type"] == gtoDB.Float:
                    for i, v in enumerate(p):
                        self.assertAlmostEqual(v, spec["dataOut"][i])
                else:
                    for i, v in enumerate(p):
                        self.assertEqual(v, spec["dataOut"][i])

                for v in p:
                    self.assert_(v in p)

                self.assert_(None not in p)
                self.assert_(sys not in p)

            except exception:
                continue
            assert exception is None, "Expected exception %s" % exception


    def test_Component(self):

        for spec in test_Component_specs:
            exception = spec.get("exception", None)
            try:
                c = gtoDB.Component(*spec["args"])
            except exception:
                continue
            
            self.assertEqual(c.name, spec["name"], spec)
            self.assertEqual(c.interp, spec["interp"], spec)
            self.assertEqual(c.flags, spec["flags"], spec)

            if 'propsIn' in spec:
                try:
                    for p in spec['propsIn']:
                        c.append(p)

                    self.assertEqual(len(c), len(spec['propsIn']))
                    self.assertEqual([p.name for p in c], [p.name for p in spec['propsIn']])

                    for p in spec['propsIn']:
                        self.assert_(p.name in c)
                    self.assert_(None not in c)
                    self.assert_(sys not in c)

                    for p in spec['propsIn']:
                        del c[p.name]
                    
                    self.assert_(len(c) == 0)

                    try:
                        del c[0]
                    except IndexError:
                        pass

                    try:
                        del c['__NOT_THERE__']
                    except IndexError:
                        pass

                except exception:
                    continue
            assert exception is None, "Expected exception %s" % exception

            
    def test_Object(self):

        for spec in test_Object_specs:
            exception = spec.get("exception", None)
            try:
                o = gtoDB.Object(*spec["args"])
            except exception:
                continue
            
            self.assertEqual(o.name, spec["name"], spec)
            self.assertEqual(o.protocol, spec["protocol"], spec)
            self.assertEqual(o.protocolVersion, spec["protocolVersion"], spec)

            if 'compsIn' in spec:
                try:
                    for c in spec['compsIn']:
                        o.append(c)

                    self.assertEqual(len(o), len(spec['compsIn']))
                    self.assertEqual([c.name for c in o], 
                                     [c.name for c in spec['compsIn']])

                    for c in spec['compsIn']:
                        self.assert_(c.name in o)
                    self.assert_(None not in o)
                    self.assert_(sys not in o)

                    for c in spec['compsIn']:
                        del o[c.name]
                    
                    self.assert_(len(o) == 0)

                    try:
                        del o[0]
                    except IndexError:
                        pass

                    try:
                        del o['__NOT_THERE__']
                    except IndexError:
                        pass

                except exception:
                    continue
            assert exception is None, "Expected exception %s" % exception
            

    def test_GtoDB(self):
        
        db = gtoDB.GtoDB()
        o = gtoDB.Object("someObject", "someProtocol", 1)
        db.append(o)
        c = gtoDB.Component("someComponent")
        o.append(c)
        p = gtoDB.Property("someProperty", range(10))
        c.append(p)
        
        self.assert_(len(db) == 1)
        self.assert_(db[0] == o)
        self.assert_("someObject" in db)
        self.assert_(None not in db)
        self.assert_(sys not in db)
        
        tempfile = mkstemp(".gto")[1]
        db.write(tempfile, gtoDB.GtoDB.TextGTO)
        
        del db["someObject"]        

        self.assert_(len(db) == 0)
        self.assert_("someObject" not in db)
        
        try:
            db.write("blank.gto", gtoDB.GtoDB.TextGTO)
        except RuntimeError:
            pass # Nothing left to write...

        del db

        db = gtoDB.GtoDB(tempfile)
        self.assert_(len(db) == 1)
        self.assert_("someObject" in db)
        
        os.remove(tempfile)
        
        

# ---------------------------------------------------------------------------------------------
# Execute the main routine and exit with the specified return value.
# ---------------------------------------------------------------------------------------------
if __name__ == "__main__":

    print "Testing %s" % os.path.realpath(gtoDB.__file__)

    unittest.main()

