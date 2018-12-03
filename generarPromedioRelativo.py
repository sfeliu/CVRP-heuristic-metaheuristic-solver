#%%
import random
import os
import glob
import subprocess
import numpy as np
from datetime import datetime


#%%
files = os.listdir("tests_cases")
ext = ".cvs"
#%%
objetivo = "error_relativo_"
modes = ["savings", "sweep", "radial", "goloso"]

v_0 = 0
v_1 = 0
v_2 = 0
v_3 = 0
i = 0
file = open('test_results_promedio_relativo/'+'promedio_error_relativo', "a+")
for f in files:
    objetivo_f = objetivo + f[0:len(f)-4] + ".txt"
    i = i+1
    with open('test_results_error_relativo/'+objetivo_f, 'r') as results:
        lines = results.read().splitlines()
        v_0 = v_0 + float(lines[0])
        v_1 = v_1 + float(lines[1])
        v_2 = v_2 + float(lines[2])
        v_3 = v_3 + float(lines[3])
v_0 = v_0 / float(i)
v_1 = v_1 / float(i)
v_2 = v_2 / float(i)
v_3 = v_3 / float(i)
file.write( str(v_0) + "\n")
file.write( str(v_1) + "\n")
file.write( str(v_2) + "\n")
file.write( str(v_3) + "\n")