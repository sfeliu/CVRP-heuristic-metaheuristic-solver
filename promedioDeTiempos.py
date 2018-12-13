#%%
import random
import os
import glob
import subprocess
import numpy as np
from datetime import datetime
#import regex

#%%
files = os.listdir("tests_cases_tiempo")
ext = ".cvs"
#%%
t_max = 10
max = 10
modes = ["savings", "sweep", "radial", "goloso"]
i = 0
cantidad_casos = 2
for f in files:
    #print (f)
    number = ''
    for char in f:
        if char.isnumeric():
            number = number + char
    number = number[0:len(number)-1]           
    #print (number)
    promedios = [0.0,0.0,0.0,0.0]
    costo_circuito = [0.0,0.0,0.0,0.0]
    file_n = "res_n="+number+".cvs"
    file = open('tests_results_promedio/'+"res.csv", "a+")
    t_mode = 0
    lines = []
    with open('test_results_error/'+file_n, 'r') as results:
        with open('tests_results_promedio/'+"res.csv", 'r') as results2:
            lines = results.read().splitlines()
            for line in lines:
                #print (i)
                arreglo_res = line.split(" ")
                #print (arreglo_res[2])
                promedios[i % 4] = promedios[i % 4] + (float(arreglo_res[2])/cantidad_casos)
                i = i+1
                costo_circuito [i % 4] = arreglo_res[1]
                #print (promedios[0])
                #print (promedios[1])
                #print (promedios[2])
                #print (promedios[3])
    file.write( number + " "+ modes[0] + " "+str(promedios[0])+" " + str(costo_circuito[0]) + "\n")
    file.write( number + " "+ modes[1] + " "+str(promedios[1])+" " + str(costo_circuito[1]) + "\n")
    file.write( number + " "+ modes[2] + " "+str(promedios[2])+" " + str(costo_circuito[2]) + "\n")
    file.write( number + " "+ modes[3] + " "+str(promedios[3])+" " + str(costo_circuito[3]) + "\n")