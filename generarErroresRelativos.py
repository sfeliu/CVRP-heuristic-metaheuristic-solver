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
objetivo = "objetivo"
modes = ["savings", "sweep", "radial", "goloso"]
for f in files:
    for mode in range(0, 4):
        objetivo_f = f[0:l-4] + "_" + modes[mode] + ".cvs"
        with open('test_results/'+objetivo_f, 'r') as results:
            lines = results.read().splitlines()
            valor = lines[-1]
            valor = float(valor)
        resultFile = objetivo + f
        file = open('test_results_error_relativo/'+'error_relativo_'+f, "a+")
        with open('objetivos/'+resultFile, 'r') as results2:
            lines2 = results2.read().splitlines()
            valor_objetivo = lines2[0]
            valor_objetivo = float(valor_objetivo)
            valor = (valor - valor_objetivo)/valor
            file.write( str(valor) + "\n")
