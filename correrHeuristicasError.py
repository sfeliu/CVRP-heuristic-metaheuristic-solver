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
modes = ["savings", "sweep", "radial", "goloso"]
for f in files:
    l = len(f)
    for mode in range(0, 4):
        resultFile = f[0:l-4] + "_" + modes[mode] + ".cvs"
        start_time = datetime.now()
        
        subprocess.call(["./build/tp","./tests_cases/"+f,str(mode),str(0)])

        time_elapsed = datetime.now() - start_time
        file = open('test_results_error/'+resultFile, "a+")

        with open('test_results/'+resultFile, 'r') as results:
            with open('tests_cases/'+f, 'r') as results2:
                lines = results2.read().splitlines()
            file.write( modes[mode] + " "+lines[-1]+" " + str(time_elapsed) + "\n")
