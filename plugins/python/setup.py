#!/usr/bin/env python

from distutils.core import setup, Extension

setup( name="gto",
       version="3.00",
       description="Gto I/O module",
       author="Tweak Films",
       author_email="gto@tweakfilms.com",
       url="http://www.tweakfilms.com/",
       ext_modules = [ Extension( "gto", 
                                [ "src/gtoHeader.cpp",        
                                  "src/gtomodule.cpp",        
                                  "src/gtoReader.cpp",        
                                  "src/gtoWriter.cpp" ],
                                  include_dirs=["../../lib"],
                                  library_dirs=["../../lib/Gto/.libs"],
                                  libraries=["Gto", "z"],
                                  extra_compile_args=["-Wno-unused-variable"],
                                )
                     ],
       data_files = [( 'examples', [ "examples/example.py", 
                                     "examples/example2.py" ])
                    ],
     )
