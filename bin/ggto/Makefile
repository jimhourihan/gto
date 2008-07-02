#
# This Makefile relies on the Houdini toolkit's $(HFS)/toolkit/makefiles/Makefile.(linux|win) rules. 
#
# Drew.Whitehouse@anu.edu.au July 2008
#
#Set OPTIMIZER to override the optimization level (defaults to -O2)
#Set INCDIRS to specify any additional include directories.
#Set LIBDIRS to specify any addition library include directories
#Set SOURCES to list all .C files required to build the DSO or App
#Set DSONAME to the name of the desired output DSO file (if applicable)
#Set APPNAME to the name of the desires output application (if applicable)

SOURCES = main.cpp Object.cpp Particle.cpp Poly.cpp NURBS.cpp

# where to install the application/dso ?
#HDIR = $(HOME)/houdini$(HOUDINI_MAJOR_RELEASE).$(HOUDINI_MINOR_RELEASE)/dso
HDIR = $(HOME)/bin

ifeq ($(OS), Windows_NT)
    # Win32 not supported at the moment ...
    APPNAME = 
    INCDIRS = 
    LIBDIRS = 
    ifndef OPTIMIZER
        OPTIMIZER = -Ox
    endif
    include $(HFS)/toolkit/makefiles/Makefile.win
else
	OPTIMIZER = -O
	APPNAME = ggto
	GTO=$(HOME)/venvs/cgkit
	LIBDIRS = -L$(GTO)/lib -lGto
    INCDIRS = -I$(GTO)/include -I.
    include $(HFS)/toolkit/makefiles/Makefile.linux
endif

OBJECTS = $(SOURCES:.cpp=.o)

ifdef DSONAME
TAGINFO = $(shell (echo -n "Compiled on:" `date`"\n         by:" `whoami`@`hostname`"\n$(SESI_TAGINFO)") | sesitag -m)

%.o:		%.cpp
	$(CC) $(OBJFLAGS) -DMAKING_DSO $(TAGINFO) $< $(OBJOUTPUT)$@

$(DSONAME):	$(OBJECTS)
	$(LINK) $(SHAREDFLAG) $(OBJECTS) $(DSOFLAGS) $(DSOOUTPUT)$@
else
%.o:		%.cpp
	$(CC) $(OBJFLAGS) $< $(OBJOUTPUT)$@

$(APPNAME):	$(OBJECTS)
	$(LINK) $(OBJECTS) $(SAFLAGS) $(SAOUTPUT)$@
endif

default:	$(DSONAME) $(APPNAME)

clean:
	rm -f $(OBJECTS) $(APPNAME) $(DSONAME)


install: $(APPNAME)
	mkdir -p $(HDIR)
	cp $(APPNAME) $(HDIR)
