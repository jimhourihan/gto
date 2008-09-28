#!/usr/bin/env python

from distutils.core import setup, Extension

gtoDB_module = Extension('_gtoDB',
                           sources=['src/gtoDB/gtoDB.cpp',
                                    'src/gtoDB/swig.cpp'],
                           include_dirs=["../../lib"],
                           library_dirs=["../../lib/Gto/.libs"],
                           libraries=["Gto", "z"],
                           extra_compile_args=["-Wno-unused-variable"],
                           )

gto_module = Extension( "gto", 
                  [ "src/gto/gtoHeader.cpp", 
                    "src/gto/gtomodule.cpp", 
                    "src/gto/gtoReader.cpp", 
                    "src/gto/gtoWriter.cpp" ],
                    include_dirs=["../../lib"],
                    library_dirs=["../../lib/Gto/.libs"],
                    libraries=["Gto", "z"],
                    extra_compile_args=["-Wno-unused-variable"],
                  )

setup( name="gto",
       version="3.51",
       description="Gto I/O module",
       author="Tweak Films",
       author_email="gto@tweaksoftware.com",
       url="http://sourceforge.net/projects/gto/",
       package_dir = {'': 'src/gtoDB'},
       py_modules = [ "gtoDB" ],
       ext_modules = [ gto_module,
                       gtoDB_module,
                     ],
       data_files = [( 'examples', [ "examples/example.py", 
                                     "examples/example2.py" ])
                    ],
     )
