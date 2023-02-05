#!/usr/bin/python

import optparse
import os, sys  


model_name="demo3.smv"

property="INVARSPEC !( (in_storm_c.NuVstorm = TRUE) &(root_fallback_c.Nuplace = NuSls) )"


# Open the smv file to write
po = open(model_name, "a")
po.write(property);

# Close opend file
po.close()
