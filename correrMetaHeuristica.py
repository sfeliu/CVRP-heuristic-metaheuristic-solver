#%%
import random
import os
import glob
import subprocess
import numpy as np
from datetime import datetime
# file, heuristica, sa, vecindario, enfriar, cant_iteraciones

#%%
#files = ["depotExtremoN80", "P-n76-k5"]
files = ["P-n19-k2", "P-n45-k5", "P-n70-k10", "P-n101-k4"]
ext = ".txt"

#%%
modes = ["savings", "sweep", "radial", "goloso"]

vecindarios = ["interchange", "2-opt"]

possible_iter = ['1000', '2500', '5000', '7500', '10000']
#possible_iter = ['1000', '2500']
cant_rep = 5
enfriar_modes = 10


file = open('metaheuristica_vec_heu.csv', "w+")
file.write("File heuristica_inicial vecindad break_point valor tiempo\n")
lines_best_res = []
avg_res = 0
best_res = 0
for f in files:
    for mode in range(0, 4):
        for vecindario in range(0,2):
            for enfriar_mode in range(0,enfriar_modes):
                for cant_iteraciones in possible_iter:
                    print('Working with file ' + f + '; with heuristic ' + modes[mode] + '; vecindario ' + vecindarios[vecindario] + '; enfriamiento ' + str(enfriar_mode) + "; cant iteraciones " + cant_iteraciones)
                    avg_res = 0
                    best_res = 0
                    avg_time = 0
                    for repetition in range(0,cant_rep):
                        result_ext = "_sa.cvs"
                        resultFile = f + "_" + modes[mode] + "_sa.cvs"
                        start_time = datetime.now()
                
                        subprocess.call(["./cmake-build-debug/tp","/home/santiago/Documents/algo_3/algo3TP3/tests_cases/"+f+ext,str(mode),str(1),str(vecindario), str(enfriar_mode), str(cant_iteraciones)])
                # os.system("./tp tests_cases/" + f + ext + " " + str(mode) + " 0")

                        time_elapsed = datetime.now() - start_time
                        avg_time += time_elapsed.total_seconds()
                        with open('test_results/'+resultFile, 'r') as results:
                            lines = results.read().splitlines()
                            valor = float(lines[-1])
                            if best_res == 0 or best_res > valor:
                                best_res = valor
                                lines_best_res = lines
                            avg_res += valor
                        print("Get res = " + str(valor)) 
                    with open('test_results/'+resultFile.replace(result_ext, '') + '_' + str(vecindario) + '_' + str(enfriar_mode) + '_' + str(cant_iteraciones) + result_ext, 'w+') as results:
                        lines = results.writelines(lines_best_res)
                    avg_time = avg_time/cant_rep
                    avg_res = avg_res/cant_rep
                    file.write(f + " " + modes[mode] + " " + vecindarios[vecindario] + " " + str(enfriar_mode) + " " + cant_iteraciones + " " + str(avg_res) + " " + str(best_res) + " " + str(avg_time) + "\n")

                # os.system("python3 plot_results.py " + resultFile)

file.close()