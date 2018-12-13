#%%
import random
import os
import glob
import subprocess
import numpy as np
from datetime import datetime


#%%
files = os.listdir("tests_cases_tiempo")
ext = ".cvs"
#%%
t_max = 10
max = 10
modes = ["savings", "sweep", "radial", "goloso"]
i = 0
for f in files:
    print (i)
    i = i+1
    number = ''
    for char in f:
        if char.isnumeric():
            number = number +char
    number = number[0:len(number)-1]
    for mode in range(0, 4):
        t_mode = 0
        l = len(f)
        resultFile = f[0:l-4] + "_" + modes[mode] + ".cvs"
        file_n = "res_n="+number+".cvs"
        for iter in range(0,max):
            start_time = datetime.now()    
            subprocess.call(["./build/tp","./tests_cases_tiempo/"+f,str(mode),str(0)])
            time_elapsed = datetime.now() - start_time
            t_mode = t_mode + (time_elapsed.total_seconds()/t_max)
            if iter == t_max-1:
                file = open('test_results_error/'+file_n, "a+")
                lines = []
                with open('test_results/'+resultFile, 'r') as results:
                    with open('tests_cases_tiempo/'+f, 'r') as results2:
                        lines = results.read().splitlines()
                file.write( modes[mode] + " "+lines[-1]+" " + str(t_mode) + "\n")