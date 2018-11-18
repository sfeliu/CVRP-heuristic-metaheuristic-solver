#%%
import random
import os
import glob
import subprocess
import numpy as np
from datetime import datetime


#%%
files = ["A-n32-k5", "depotExtremoN80", "P-n101-k4",
         "P-n16-k8", "P-n60-k15", "P-n76-k5"]
ext = ".txt"

#%%
modes = ["savings", "sweep", "radial", "goloso"]
for f in files:
    for mode in range(0, 4):
        resultFile = f + "_" + modes[mode] + ".cvs"
        start_time = datetime.now()
        
        subprocess.call(["./build/tp","tests_cases/"+f+ext,str(mode),str(0)])
        # os.system("./tp tests_cases/" + f + ext + " " + str(mode) + " 0")

        time_elapsed = datetime.now() - start_time
        file = open(f + "_result" + ext, "a+")

        with open('test_results/'+resultFile, 'r') as results:
            lines = results.read().splitlines()
            valor = lines[-1]
        file.write( modes[mode] + " " + valor + " " + str(time_elapsed) + "\n")

        os.system("python3 plot_results.py " + resultFile)

