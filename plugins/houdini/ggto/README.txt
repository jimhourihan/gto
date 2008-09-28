
Houdini GTO input/output converter
Version 0.2.0, built on Sep 20 2006 at 11:54:50. 
Copyright (c) 2001-2006 Tweak Films. All rights reserved.
-i %S                   Input Gto filename (output is stdout)
-o %S                   Output Gto filename (input is stdin)
-a                      Make output ascii (geo/ascii gto)
-v                      Verbose


Supported object types:

    * Particles, including all int, float and vector per-particle attributes.
      NOTE: Attributes *should* work, but are currently untested!
    
    * Polygons: All polys are combined into a single GTO object.
      Currently, only vertices and faces are written out--no normals or
      custom data, but it's coming soon.  Same is true for input from GTO.
      

To install, make sure the ggto binary is in your path, and add the 
following line to the GEOio file :

.gto "ggto -i %s" "ggto -o %s"



-Mike  
