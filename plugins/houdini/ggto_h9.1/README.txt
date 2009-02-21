Houdini GTO input/output converter
Version 0.3.0
Copyright (c) 2001-2006 Tweak Films. All rights reserved.

-a                      Make output ascii (ascii gto)
-v                      Verbose

Supported object types:

    * Particles, including all int, float and vector per-particle attributes.
      NOTE: Attributes *should* work, but are currently untested!
    
    * Polygons: All polys are combined into a single GTO object.
      Currently, only vertices and faces are written out--no normals or
      custom data, but it's coming soon.  Same is true for input from GTO.
      

To install, make sure the ggto binary is in your path, and add the 
following line to the GEOio file :

.gto "ggto %s stdout.bgeo" "ggto stdin.bgeo %s"


-Mike  

-Drew.Whitehouse@anu.edu.au (ported to Houdini 9.1.179 on x64 Ubuntu 8.0.4, July 2008)

