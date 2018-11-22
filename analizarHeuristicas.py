#%%
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

#%%
camionUnico = pd.read_csv('camionUnico_result.txt', delim_whitespace=True)
an32 = pd.read_csv('A-n32-k5_result.txt', delim_whitespace=True)
depoExtremo = pd.read_csv('depotExtremoN80_result.txt', delim_whitespace=True)
pn16 = pd.read_csv('P-n16-k8_result.txt', delim_whitespace=True)
pn60 = pd.read_csv('P-n60-k15_result.txt', delim_whitespace=True)
pn76 = pd.read_csv('P-n76-k5_result.txt', delim_whitespace=True)
pn101 = pd.read_csv('P-n101-k4_result.txt', delim_whitespace=True)

print(camionUnico)
#%%

new_df = pd.DataFrame({'metodo': ['Savings','CfRs (Sweep)','CfRs', 'Goloso']})
camionUnico.update(new_df)
an32.update(new_df)
depoExtremo.update(new_df)
pn16.update(new_df)
pn60.update(new_df)
pn76.update(new_df)
pn101.update(new_df)
# camionUnico['metodo'].replace(['savings'], 'Savings')
# print(camionUnico)


#camionUnico
#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="valor", data=camionUnico)
ax.set(xlabel='Metodo', ylabel='Costo')
fig = ax.get_figure()
fig.savefig("imgs/comparacionesHeuristicas/camionUnicoCosto.png")


#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="tiempo", data=camionUnico)
ax.set(yscale="log")
ax.set(xlabel='Metodo', ylabel='Tiempo (ms)')
fig = ax.get_figure()
fig.savefig("imgs/comparacionesHeuristicas/camionUnicoTiempo.png")

################an32
#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="valor", data=an32)
ax.set(xlabel='Metodo', ylabel='Costo')
fig = ax.get_figure()
fig.savefig("imgs/comparacionesHeuristicas/an32Costo.png")


#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="tiempo", data=an32)
ax.set(xlabel='Metodo', ylabel='Tiempo (ms)')
ax.set(yscale="log")
fig = ax.get_figure()
fig.savefig("imgs/comparacionesHeuristicas/an32Tiempo.png")

################extremo
#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="valor", data=depoExtremo)
ax.set(xlabel='Metodo', ylabel='Costo')
fig = ax.get_figure()
fig.savefig("imgs/comparacionesHeuristicas/depoExtremoCosto.png")

#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="tiempo", data=depoExtremo)
ax.set(xlabel='Metodo', ylabel='Tiempo (ms)')
ax.set(yscale="log")
fig = ax.get_figure()
fig.savefig("imgs/comparacionesHeuristicas/depoExtremoTiempo.png")


################pn16
#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="valor", data=pn16)
ax.set(xlabel='Metodo', ylabel='Costo')
fig = ax.get_figure()
fig.savefig("imgs/comparacionesHeuristicas/pn16Costo.png")

#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="tiempo", data=pn16)
ax.set(xlabel='Metodo', ylabel='Tiempo (ms)')
ax.set(yscale="log")
fig = ax.get_figure()
fig.savefig("imgs/comparacionesHeuristicas/pn16Tiempo.png")

################pn60
#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="valor", data=pn60)
ax.set(xlabel='Metodo', ylabel='Costo')
fig = ax.get_figure()
fig.savefig("imgs/comparacionesHeuristicas/pn60Costo.png")

#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="tiempo", data=pn60)
ax.set(xlabel='Metodo', ylabel='Tiempo (ms)')
ax.set(yscale="log")
fig = ax.get_figure()
fig.savefig("imgs/comparacionesHeuristicas/pn60Tiempo.png")


################pn76
#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="valor", data=pn76)
ax.set(xlabel='Metodo', ylabel='Costo')
fig = ax.get_figure()
fig.savefig("imgs/comparacionesHeuristicas/pn76Costo.png")

#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="tiempo", data=pn76)
ax.set(xlabel='Metodo', ylabel='Tiempo (ms)')
ax.set(yscale="log")
fig = ax.get_figure()
fig.savefig("imgs/comparacionesHeuristicas/pn76Tiempo.png")


################pn101
#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="valor", data=pn101)
ax.set(xlabel='Metodo', ylabel='Costo')
fig = ax.get_figure()
fig.savefig("imgs/comparacionesHeuristicas/pn101Costo.png")

#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="tiempo", data=pn101)
ax.set(xlabel='Metodo', ylabel='Tiempo (ns)')
ax.set(yscale="log")
fig = ax.get_figure()
fig.savefig("imgs/comparacionesHeuristicas/pn101Tiempo.png")
