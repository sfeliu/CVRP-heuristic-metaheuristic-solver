import csv
import matplotlib.pyplot as plt
import networkx as nx
from itertools import count
from random import randint
import os
import sys
import subprocess as sub

# ejecutable = './cmake-build-debug/test-4'
# tsp_full_filepath = sys.argv[1]
# p = sub.Popen([ejecutable, tsp_full_filepath], stderr=sub.PIPE, stdout=sub.PIPE)
# output, errors = p.communicate()
# res = output.decode("utf-8").split('\n')

# file = "/home/cristian/Facu/AlgoritmosIII/Tps/TP3/test_results/P-n16-k8_radial.cvs"


file = "test_results/" + sys.argv[1]
res = []
with open(file, 'r') as f:
    for line in f.readlines():
        res.append(line.replace('\n', ''))

cant_ciclos = res[0]
raw_ciclos = res[1:int(cant_ciclos)+1]
costo_total = res[-1]

with open(file.replace('.cvs', '_n.cvs'), 'r') as csvfile:
    nodos = []
    for row in csv.reader(csvfile, delimiter=','):
        nodos.append((float(row[0]), float(row[1])))

ciclos = []
for index, ciclo in enumerate(raw_ciclos):
    ciclos.append([])
    for nodo in ciclo.split(','):
        ciclos[index].append(nodo)

G = nx.Graph()

for index in range(len(nodos)):
    G.add_node(index, pos=nodos[index])

colors = []
for i in range(len(ciclos) + 1):
    colors.append("#%06X" % randint(0, 0xFFFFFF))

node_coord = []
# mostrar diff (3)
for color_index, ciclo in enumerate(ciclos):
    for index, nodo in enumerate(ciclo[:-1]):
        G.add_edge(int(ciclo[index]), int(ciclo[index + 1]), color=colors[color_index])

edges = G.edges()
colors = [G[u][v]['color'] for u, v in edges]

color_map = []
for n in G:
    color_map.append('black')

pos = nx.get_node_attributes(G, 'pos')
# nx.draw(G, pos, node_size = 15, node_color = color_map, alpha = 0.9, width = 1)

# sinNombres
# nx.draw(G, pos, node_size = 10, edge_color = colors, node_color = color_map, alpha = 0.9, width = 3)
# nombres
nx.draw(G, pos, node_size=300, edge_color=colors, node_color=color_map, alpha=0.9, width=3, with_labels=True,
        font_color='y', font_weight='heavy')

plt.savefig("imgs/" + sys.argv[1] + ".png")
# plt.show()


