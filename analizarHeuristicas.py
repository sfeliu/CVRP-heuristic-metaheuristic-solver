#%%
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

#%%
camionUnico = pd.read_csv('camionUnico_result.txt', delim_whitespace=True)
an32 = pd.read_csv('A-n32-k5_result.txt', delim_whitespace=True)
depoExtremo = pd.read_csv('depotExtremoN80_result.txt', delim_whitespace=True)
pn16 = pd.read_csv('P-n16-k18_result.txt', delim_whitespace=True)
pn60 = pd.read_csv('P-n60-k15_result.txt', delim_whitespace=True)
pn76 = pd.read_csv('P-n76-k5_result.txt', delim_whitespace=True)
pn101 = pd.read_csv('P-n101-k4_result.txt', delim_whitespace=True)


#camionUnico
#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="valor", data=camionUnico)

#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="tiempo", data=camionUnico)
ax.set(yscale="log")

################an32
#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="valor", data=an32)

#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="tiempo", data=an32)
ax.set(yscale="log")

################extremo
#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="valor", data=depoExtremo)

#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="tiempo", data=depoExtremo)
ax.set(yscale="log")

################pn16
#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="valor", data=pn16)

#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="tiempo", data=pn16)
ax.set(yscale="log")

################pn60
#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="valor", data=pn60)

#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="tiempo", data=pn60)
ax.set(yscale="log")


################pn76
#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="valor", data=pn76)

#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="tiempo", data=pn76)
ax.set(yscale="log")


################pn101
#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="valor", data=pn101)

#%%
sns.set(style="whitegrid")
ax = sns.barplot(x="metodo", y="tiempo", data=pn101)
ax.set(yscale="log")
