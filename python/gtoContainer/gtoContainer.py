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

""" 
This module defines a generic gtoContainer class.  This class can be used to
work with GTO data at an extremely high level, without the need to derive a new
reader class or change your workflow to accomodate the somewhat restrictive GTO
I/O routines.  Perhaps the best introduction to what gtoContainer can do is by
example.  Here's a simple one that prints the names of every object, component,
and property in a file:


    from gtoContainer import *

    myGtoFile = gtoContainer( "foo.gto" )

    for object in myGtoFile:
        for component in object:
            for property in component:
                print object, component, property


Let's say that you want to check every object for the existence of a particular
component/property and print the stored data if it's there:


    for object in myGtoFile:
        if "assignments.texture" in object:
            print object.assignments.texture()


Here's another example that changes the value of a 1x1 Int property,
"dolphin.smoothing.method", that is already known to exist, then saves the
file:


    myGtoFile.dolphin.smoothing.method.setData( 2 )
    myGtoFile.write( "newfoo.gto" )


But, you say, what if the name of the object/component/property isn't known
until runtime?  No problem:


    print myGtoFile[someObject][someComponent][someProperty].name()


or mix and match:    

    
    myGtoFile[someObject].surface.texture.setData( "foo.tex" )
    

Let's say we want to start fresh and make a really simple gto file.  Here we
illustrate three different ways to create and add Objects, Components, and
Properties to a gtoContainer:


    #!/usr/bin/env python
    
    import gto
    from gtoContainer import *

    myGtoFile = gtoContainer()
    myGtoFile.myobj = Object( "myobj", "objprotocol", 1 )
    myGtoFile.myobj.append( Component( "mycomponent", "compinterp" ) )
    
    prop = Property( "myprop", gto.String, size = 2, width = 1,
                     data = ["Hello","world"] )
    myGtoFile.myobj.mycomponent.append( prop )

    myGtoFile.write( "helloworld.gto" )


One last thing.  It is important to note that because Objects, Components,
and Properties are python classes, assigning one to a variable does not make
a copy, it makes a reference.  For example:

    >>> print myGtoFile.dolphin.smoothing.method()
    (0,)
    >>> m = myGtoFile.dolphin.smoothing.method
    >>> m.setData( (1,) )
    >>> print myGtoFile.dolphin.smoothing.method()
    (1,)

You you can create a true copy of an Object, Component, or Property by using
the .copy() method:

    >>> print myGtoFile.dolphin.smoothing.method()
    (0,)
    >>> m = myGtoFile.dolphin.smoothing.method.copy()
    >>> m.setData( (1,) )
    >>> print myGtoFile.dolphin.smoothing.method()
    (0,)
    >>> print m()
    (1,)

"""

from types import *
import re
import gto

#############################################
class Property:
    """ 
    Represents a single GTO property.  All data access is done via function
    calls, which I like to think makes resulting code a little clearer.
    """
    def __init__( self, name, pType, size = None, width = None, data = None, 
                  interp = None ):
        self.__name = name
        self.__pType = pType
        self.__size = size
        self.__width = width
        self.__data = data
        self.__interp = interp
        self.__component = None

    def name(self):
        return self.__name
    
    def setName(self, value):
        self.__name = value

    def type(self):
        return self.__pType
    
    def setType(self, value):
        self.__pType = value

    def size(self):
        return self.__size
    
    def setSize(self, value):
        self.__size = value
    
    def width(self):
        return self.__width
    
    def setWidth(self, value):
        self.__width = value
    
    def interp(self):
        return self.__interp
    
    def setInterp(self, value):
        self.__interp = value

    def data(self):
        """ 
        This method always returns a single tuple, even if there is only 
        one value.  See the gto.Reader class for more info.
        """
        return self.__data
    
    def setData( self, value, size = None, width = None ):
        """
        WARNING: Make sure the data layout matches the size and width 
        arguments or there will be hell to pay.  If they aren't specified
        here, the Property object's current size and/or width are used.
        There is currently no size/width checking done here, but the writer
        will throw when the file is written if they don't match.
        """
        self.__data = value
        if size != None:
            self.__size = size
        if width != None:
            self.__width = width

    def component(self):
        """ 
        Returns the Component instance to which this property belongs.  This
        can be None if the property hasn't been added to a component yet.
        """
        return self.__component
        
    def __setComponent(self, value):
        """ For internal use only """
        if not isinstance(value, Component):
            raise TypeError, "%s must be a Component object" % value
        self.__component = value

    def copy(self):
        """
        Returns a new Property object that is an exact copy.
        """
        prop = Property( self.__name, self.__pType, 
                         self.__size, self.__width, 
                         self.__data, self.__interp )
        prop._Property__setComponent( self.__component )
        return prop

    def __call__(self):
        """ 
        Calling the property object itself will return its data.  It is
        exactly the same as calling data().
        """
        return self.__data

    def __repr__(self):
        return ( "<gtoContainer Property: %s>" % self.__name )

    def __str__(self):
        return ( "Property: %s" % self.__name )

    def __eq__(self, other):
        if( not isinstance( other, Property ) ):
            return False
        if( ( self.__name == other.name() )
            and ( self.__pType == other.type() )
            and ( self.__size == other.size() )
            and ( self.__width == other.width() )
            and ( self.__data == other.data() )
            and ( self.__interp == other.interp() ) ):
            return True
        return False
    

#############################################
class Component:
    """
    Represents a single GTO component which may contain any number of 
    properties.  Information about the component itself is had via
    method calls (name(), interp(), flags(), and object()).
    
    Properties in this component can be accessed either as attributes or 
    as items.  Any of the following will work:
    
        propertyInstance = compInstance.existingPropertyName
        propertyInstance = compInstance[0]
        propertyInstanceList = compInstance[0:1]
        propertyInstance = compInstance["existingPropertyName"]
        propertyInstance = compInstance[existingPropertyInstance]

        compInstance.existingPropertyName = propertyInstance
        compInstance.newPropertyName = propertyInstance
        compInstance[0] = propertyInstance
        compInstance["existingPropertyName"] = propertyInstance
        compInstance[existingPropertyInstance] = propertyInstance
        compInstance[newPropertyName] = propertyInstance
    
    Use the builtin del() to remove a Property instance from a Component
    instance:

        del( compInstance.existingPropertyName )
        del( compInstance[0] )
        del( compInstance["existingPropertyName"] )

    The builtin "in" can be used to test for the existence of a Property
    instance:
    
        if( "points" in componentInstance ):
            ....
    
    """
    def __init__( self, name, interp = None, flags = 0, 
                  object = None ):
        self.__name = name
        self.__interp = interp
        self.__flags = flags
        self.__object = object
        self.__properties = []

    def name(self):
        return self.__name

    def setName(self, value):
        self.__name = value
    
    def interp(self):   
        return self.__interp

    def setInterp( self, value):
        self.__interp = value
    
    def flags(self):
        return self.__flags

    def setFlags(self, value):
        self.__flags = value
    
    def object(self):
        """ 
        Returns the Object instance to which this component belongs.  This
        can be None if the component hasn't been added to an object yet.
        """
        return self.__object
    
    def __setObject(self, value):
        """ For internal use only. """
        if not isinstance(value, Object):
            raise TypeError, "Must be an Object instance" 
        self.__object = value
    
    def properties(self):
        """
        Return a list of all the Property objects in this Component instance.
        """
        return self.__properties

    def propertiesMatching(self, regex):
        """
        Return a list of all the Property instances in this Component instance
        whose names match the given regular expression.
        """
        regexComp = re.compile( "^%s$" % regex )
        return filter( lambda(x): regexComp.match(x.name()), self.__properties )

    def __repr__(self):
        return ( "<gtoContainer Component: %s>" % self.__name )

    def __str__(self):
        return ( "Component: %s" % self.__name )

    def __len__(self):
        """
        Returns the number of Property instances in this Component instance.
        """
        return len(self.__properties)
        
    def __getattr__(self, name):
        if( name in self.__dict__ ):
            return self.__dict__[name]
        for prop in self.__properties:
            if prop.name() == name:
                return prop
        raise AttributeError, "Container instance has no attribute '%s'" % name           

    def __setattr__(self, name, value):
        if isinstance( value, Property ):
            if not name == value.name():
                raise ValueError, "name mismatch: %s and %s" % (name, value.name() )        
            for p in self.__properties:
                if( p.name() == name ):
                    value._Property__setComponent( self )
                    self.__properties[self.__properties.index(p)] = value
                    return
            self.__properties.append( value )
        self.__dict__[name] = value

    def __delattr__(self, name):
        if( name in self.__dict__ ):
            del self.__dict__[name]
        for p in range( 0, len( self.__properties ) ):
            if self.__properties[p].name() == name:
                del self.__properties[p]
                return
        raise AttributeError, "Component instance has no property '%s'" % name           

    def __getitem__(self,key):
        if( type(key) == StringType ):
            for prop in self.__properties:
                if prop.name() == key:
                    return prop
            raise KeyError, key
        if( type(key) == IntType ):
            return self.__properties[key]
        if( type(key) == SliceType ):
            return self.__properties[key.start:key.stop]
        if( isinstance(key, Property) ):
            for prop in self.__properties:
                if prop.name() == key.name():
                    return prop
            raise KeyError, key
        raise TypeError, "Properties cannot be indexed by keys of this type"
        
    def __setitem__(self,key,value):
        if( not isinstance( value, Property ) ):
            raise TypeError, "Must be a Property instance"
        if( type(key) == StringType ):
            if not key == value.name():
                raise ValueError, "name mismatch: %s and %s" % (key, value.name() )        
            for p in self.__properties:
                if( p.name() == key ):
                    self.__properties[self.__properties.index(p)] = value
                    return
            self.__properties.append( value )
            value._Property__setComponent( self )
        if( type(key) == IntType ):
            self.__properties[key] = value
            value._Property__setComponent( self )
            return
        if( isinstance(key, Property) ):
            self.__properties[self.__properties.index(key)] = value
            value._Property__setComponent( self )
            return
        raise KeyError

    def __delitem__(self,key):
        if( type(key) == StringType ):
            for p in self.__properties:
                if( p.name() == key ):
                    self.__properties.remove( p )
                    return
        elif( type(key) == IntType ):
            del self.__properties[key]
            return
        if( type(key) == SliceType ):
            del self.__properties[key.start:key.stop]
            return
        raise KeyError

    def append(self, what):
        """ Adds a Property instance to this Component instance """
        if( not isinstance( what, Property ) ):
            raise TypeError, "Must be a Property instance"
        what._Property__setComponent(self)
        self.__properties.append( what )

    def copy(self):
        """
        Returns a new Component instance with a new copy of each of the 
        properties in self.properties().
        """
        comp = Component( self.__name, self.__interp, 
                          self.__flags )
        comp._Component__setObject( self.__object )
        for prop in self.__properties:
            comp.append( prop.copy() )
        return comp

    def __contains__(self, item):
        if( type(item) == StringType ):
            for prop in self.__properties:
                if prop.name() == item:
                    return True
        if( isinstance(item, Property) ):
            for prop in self.__properties:
                if prop.name() == item.name():
                    return True
        return False

    
#############################################
class Object:
    """
    Represents a single GTO Object which may contain any number of 
    components.  Information about the object itself is had via
    method calls (name(), protocol(), and protocolVersion()).
    
    Components in this object can be accessed either as attributes or 
    as items.  Any of the following will work:
    
        componentInstance = objInstance.existingComponentName
        componentInstance = objInstance[0]
        componentInstanceList = objInstance[0:1]
        componentInstance = objInstance["existingComponentName"]
        componentInstance = objInstance[existingComponentInstance]

        objInstance.existingComponentName = propertyInstance
        objInstance.newComponentName = propertyInstance
        objInstance[0] = propertyInstance
        objInstance["existingComponentName"] = propertyInstance
        objInstance[existingComponentInstance] = propertyInstance
        objInstance[newComponentName] = propertyInstance

    Use the builtin del() to remove a Component instance from an Object
    instance:

        del( objInstance.existingComponentName )
        del( objInstance[0] )
        del( objInstance["existingComponentName"] )

    The builtin "in" can be used to test for the existence of a Component
    or a Component/Property pair within an Object instance:

        if( "points" in objectInstance ):
            ....

        if( "points.position" in objectInstance ):
            ....

    
    """
    def __init__( self, name, protocol, version ):
        self.__name = name
        self.__protocol = protocol
        self.__protocolVersion = version
        self.__components = []
    
    def name(self):
        return self.__name
    
    def setName(self, value):
        self.__name = value
    
    def protocol(self):
        return self.__protocol
    
    def setProtocol(self, value):
        self.__protocol = value
    
    def protocolVersion(self):
        return self.__protocolVersion
        
    def setProtocolVersion(self, value):
        self.__protocolVersion = value

    def components(self):
        """
        Return a list of all the Component instances in this Object instance.
        """
        return self.__components

    def componentsMatching(self, regex):
        """
        Return a list of all the Component instances in this Object instance
        whose names match the given regular expression.
        """
        regexComp = re.compile( "^%s$" % regex )
        return filter( lambda(x): regexComp.match(x.name()), self.__components )
    
    def __repr__(self):
        return ( "<gtoContainer Object: %s>" % self.__name )

    def __str__(self):
        return ( "Object: %s" % self.__name )

    def __len__(self):
        """ Returns the number of Component objects in this Object instance. """
        return len(self.__components)
        
    def __getattr__(self, name):
        if( name in self.__dict__ ):
            return self.__dict__[name]
        for comp in self.__components:
            if comp.name() == name:
                return comp
        raise AttributeError, "Object instance has no attribute '%s'" % name

    def __setattr__(self, name, value):
        if isinstance( value, Component ):
            if not name == value.name():
                raise ValueError, "name mismatch: %s and %s" % (name, value.name() )        
            for c in self.__components:
                if( c.name == name ):
                    value._Component__setObject(self)
                    self.__components[self.__components.index(c)] = value
                    return
            self.__components.append( value )
        self.__dict__[name] = value

    def __delattr__(self, name):
        if( name in self.__dict__ ):
            del self.__dict__[name]
        for c in range( 0, len( self.__components ) ):
            if self.__components[c].name() == name:
                del self.__components[c]
                return
        raise AttributeError, "Object instance has no component '%s'" % name           

    def __getitem__(self,key):
        if( type(key) == StringType ):
            for comp in self.__components:
                if comp.name() == key:
                    return comp
            raise KeyError, key
        if( type(key) == IntType ):
            return self.__components[key]
        if( type(key) == SliceType ):
            return self.__components[key.start:key.stop]
        if( isinstance(key, Component) ):
            for comp in self.__components:
                if comp.name() == key.name():
                    return comp
            raise KeyError, key
        raise TypeError, "Components cannot be indexed by keys of this type"
        
    def __setitem__(self,key,value):
        if( not isinstance( value, Component ) ):
            raise TypeError, "Must be a Component instance"
        if( type(key) == StringType ):
            if not key == value.name():
                raise ValueError, "name mismatch: %s and %s" % (key, value.name() )        
            for c in self.__components:
                if( c.name() == key ):
                    value._Component__setObject( self )
                    self.__components[self.__components.index(c)] = value
                    return
            self.__components.append( value )
            value._Component__setObject( self )
        if( type(key) == IntType ):
            self.__components[key] = value
            value._Component__setObject( self )
            return
        if( isinstance(key, Component) ):
            self.__components[self.__components.index(key)] = value
            value._Component__setObject( self )
            return
        raise KeyError

    def __delitem__(self,key):
        if( type(key) == StringType ):
            for c in self.__components:
                if( c.name() == key ):
                    self.__components.remove( c )
                    return
        elif( type(key) == IntType ):
            del self.__components[key]
            return
        elif( type(key) == SliceType ):
            del self.__components[key.start:key.stop]
            return
        raise KeyError

    def copy(self):
        """
        Returns a new Object instance with a new copy of each of the 
        components and properties in self.components().
        """
        obj = Object( self.__name, self.__protocol, 
                      self.__protocolVersion )
        for comp in self.__components:
            obj.append( comp.copy() )
        return obj

    def append(self, what):
        """ Adds a Component instance to this Object instance """
        if( not isinstance( what, Component ) ):
            raise TypeError, "Must be a Component instance"
        what._Component__setObject(self)
        self.__components.append( what )

    def __contains__(self, item):
        if( type(item) == StringType ):
            parts = item.split(".")
            if( len( parts ) == 1 or len( parts ) == 2 ):
                for comp in self.__components:
                    if comp.name() == parts[0]:
                        if( len( parts ) == 2 ):
                            return parts[1] in comp
                        return True

        if( isinstance(item, Component) ):
            for comp in self.__components:
                if comp.name() == item.name():
                    return True
        return False


#############################################
class gtoContainer( gto.Reader ):
    """
    Represents a single GTO file which may contain any number of 
    objects.  Information about the file itself is had via
    method calls (filename() and objects()).
    
    Objects in this gtoContainer can be accessed either as attributes or 
    as items.  Any of the following will work:
    
        objectInstance = gtoContainerInstance.existingObjectName
        objectInstance = gtoContainerInstance[0]
        objectInstanceList = gtoContainerInstance[0:1]
        objectInstance = gtoContainerInstance["existingObjectName"]
        objectInstance = gtoContainerInstance[existingObjectInstance]

        gtoContainerInstance.existingObjectName = objectInstance
        gtoContainerInstance.newObjectName = objectInstance
        gtoContainerInstance[0] = objectInstance
        gtoContainerInstance["existingObjectName"] = objectInstance
        gtoContainerInstance[existingObjectInstance] = objectInstance
        gtoContainerInstance[newObjectName] = objectInstance

    Use the builtin del() to remove an Object instance from a gtoContainer
    instance:

        del( gtoContainerInstance.existingObjectName )
        del( gtoContainerInstance[0] )
        del( gtoContainerInstance["existingObjectName"] )

    The builtin "in" can be used to test for the existence of an Object, an
    Object/Component pair, or an Object/Component/Property triplet within an
    gtoContainer instance:

        if( "myobjName" in gtoContainerInstance ):
            ....

        if( "myobjName.points" in gtoContainerInstance ):
            ....

        if( "myobjName.points.position" in gtoContainerInstance ):
            ....
    
    """

    def __init__( self, filename = None ):
        self.__objects = []
        self.__filename = filename
        gto.Reader.__init__(self)
        if( filename != None ):
            self.open( self.__filename )
    
    def objects(self):
        """
        Return a list of all the Object instances in this gtoContainer instance.
        """
        return self.__objects

    def objectsMatching(self, regex):
        """
        Return a list of all the Object instances in this gtoContainer instance
        whose names match the given regular expression.
        """
        regexComp = re.compile( "^%s$" % regex )
        return filter( lambda(x): regexComp.match(x.name()), self.__objects )
    
    def filename(self):
        return self.__filename
    
    def append( self, what ):
        """ Adds an Object instance to this gtoContainer instance """
        self.__objects.append( what )

    def __repr__(self):
        return "<gtoContainer file: %s>" % self.__filename

    def __str__(self):
        return "gtoContainer file: %s" % self.__filename

    def __len__(self):
        """
        Returns the number of Object instances in this gtoContainer instance.
        """
        return len(self.__objects)

    def __getattr__(self, name):
        if( name in self.__dict__ ):
            return self.__dict__[name]
        for obj in self.__objects:
            if obj.name() == name:
                return obj
        raise AttributeError, "gtoContainer instance has no attribute '%s'" % name           
    
    def __setattr__(self, name, value):
        if isinstance( value, Object ):
            if not name == value.name():
                raise ValueError, "name mismatch: %s and %s" % (name, value.name() )        
            for o in self.__objects:
                if( o.name() == name ):
                    self.__objects[self.__objects.index(o)] = value
                    return
            self.__objects.append( value )
        self.__dict__[name] = value

    def __delattr__(self, name):
        if( name in self.__dict__ ):
            del self.__dict__[name]
        for o in range( 0, len( self.__objects ) ):
            if self.__objects[o].name() == name:
                del self.__objects[o]
                return
        raise AttributeError, "gtoContainer instance has no object '%s'" % name           

    def __getitem__(self,key):
        if( type(key) == StringType ):
            for obj in self.__objects:
                if obj.name() == key:
                    return obj
            raise KeyError, key
        if( type(key) == IntType ):
            return self.__objects[key]
        if( type(key) == SliceType ):
            return self.__objects[key.start:key.stop]
        if( isinstance(key, Object) ):
            for obj in self.__objects:
                if obj.name() == key.name():
                    return obj
            raise KeyError, key
        raise TypeError, "Objects cannot be indexed by keys of this type"
        
    def __setitem__(self,key,value):
        if( not isinstance( value, Object ) ):
            raise TypeError, "gtoContainer can only contain Objects"
        if( type(key) == StringType ):
            for o in self.__objects:
                if( o.name() == key ):
                    o = value
                    return
        self.__objects.append( value )
        self.__objects[-1].setName( key )
        

    def __delitem__(self,key):
        if( type(key) == StringType ):
            for o in self.__objects:
                if( o.name() == key ):
                    self.__objects.remove( o )
                    return
        elif( type(key) == IntType ):
            del self.__objects[key]
            return
        elif( type(key) == SliceType ):
            del self.__objects[key.start:key.stop]
            return

        raise KeyError, key

    def __contains__(self, item):
        if( type(item) == StringType ):
            parts = item.split(".")
            if len( parts ) == 1:
                for obj in self.__objects:
                    if obj.name() == parts[0]:
                        return True
            elif len( parts ) == 2:
                for obj in self.__objects:
                    if obj.name() == parts[0]:
                        return parts[1] in obj
            elif len( parts ) == 3:
                for obj in self.__objects:
                    if obj.name() == parts[0]:
                        return ".".join( parts[1:] ) in obj
        if( isinstance(item, Object) ):
            for obj in self.__objects:
                if obj.name() == item.name():
                    return True
        return False

    #
    # GTO Reader virtual functions
    # 

    def object( self, objName, protocol, protocolVersion, oinfo ):
        """
        Overloaded method of gto.Reader.  Do not call directly.
        """
        obj = Object( objName, protocol, protocolVersion )
        self.append( obj )
        return 1

    def component( self, compName, interp, cinfo ):
        """
        Overloaded method of gto.Reader.  Do not call directly.
        """
        obj = self[cinfo.object.name]
        c = Component( compName, interp, cinfo.flags )
        obj.append( c )
        return 1

    def property( self, propName, interp, pinfo ):
        """
        Overloaded method of gto.Reader.  Do not call directly.
        """
        objName = pinfo.component.object.name
        compName = pinfo.component.name
        newProp = Property( propName, pinfo.type, pinfo.size, pinfo.width,
                            interp = interp )
        self[objName][compName].append( newProp )
        return 1

    def dataRead( self, propName, dataTuple, pinfo ):
        """
        Overloaded method of gto.Reader.  Do not call directly.
        """
        objName = pinfo.component.object.name
        compName = pinfo.component.name
        self[objName][compName][propName].setData( dataTuple )

    #
    # End of GTO Reader virtual functions
    #

    def write( self, filename = None, compress = True ):
        """
        Writes the current contents of this gtoContainer instance to the
        given filename.  If no filename is given, the filename given to
        the constructor is used.
        """
        if( filename == None ):
            filename = self.__filename
        else:
            self.__filename = filename
        
        if filename == None:
            raise ValueError, "No filename specified"

        writer = gto.Writer()
        writer.open( filename, compress )

        for obj in self.__objects:
            objName = obj.name()
            objProtocol = obj.protocol()
            objProtocolVersion = obj.protocolVersion()
            objNumComponents = len(obj)
            writer.beginObject( objName, objProtocol, objProtocolVersion )
            for comp in obj.components():
                compName = comp.name()
                compNumProps = len(comp)
                compFlags = comp.flags()
                writer.beginComponent( compName, compFlags )
                for prop in comp.properties():
                    propName = prop.name()
                    propType = prop.type()
                    propSize = prop.size()
                    propWidth = prop.width()
                    writer.property( propName, propType, propSize, propWidth )
                    if( propType == gto.String ):
                        writer.intern( prop() )
                writer.endComponent()
            writer.endObject()

        writer.beginData()
        for obj in self.objects():
            for comp in obj.components():
                for prop in comp.properties():
                    writer.propertyData( prop() )
        writer.endData()
        writer.close()

