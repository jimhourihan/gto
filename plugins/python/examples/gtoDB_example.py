
import sys
from gtoDB import *

db = GtoDB()
db.read(sys.argv[1])

for obj in db:
    print obj
    for comp in obj:
        print "  ", comp
        for prop in comp:
            print "    ", prop, prop[:]
    print
