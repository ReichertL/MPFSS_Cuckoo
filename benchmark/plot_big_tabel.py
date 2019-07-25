import os
import pandas as pd
import matplotlib.pyplot as plt
from time import gmtime, strftime
import sys
import numpy as np
import os

if (not(len(sys.argv)==2 or len(sys.argv)==3)):
	print("Please provide path to csv table")
	sys.exit(1);
path=sys.argv[1]

threads=""
try:
	threads=sys.argv[2]
except:
	threads="-1"
	print(str(threads))
no_filename=path.split(".")[0]
splits=no_filename.split("_")
meas_type=splits[1]
img_path=splits[1]+"/plots/"
if not os.path.exists(img_path):
    os.makedirs(img_path)
   
df = pd.read_csv(path)
if (threads != "-1"):
	df.loc[df['threads'].isin([threads])]
df.sort_values(by=['t'])
df_means=df.groupby("t").agg({"runtime":['mean','std']})
df_means=df_means.reset_index()
df_means.columns=["t","mean_runtime","std_runtime"]
df_means.to_csv(meas_type+"/evaluation_t-to-runtime")
fig, ax = plt.subplots(1,1)
fig.autofmt_xdate()
plt.errorbar(df_means["t"], df_means['mean_runtime'], yerr=df_means['std_runtime'], marker='x')
plt.ylabel('Mean Runtime in Seconds')
plt.xlabel('Number Indices t')
plt.xticks(df_means["t"])
plt.margins(0.05, 0.1)
titel='Number of Indices to Mean Runtime'
if threads != "-1":
	titel='Number of Indices to Mean Runtime - threads: '+threads
plt.title(titel)
plt.savefig(img_path+strftime("%Y-%m-%d_%H:%M:%S", gmtime())+"_mean-runtime-vs-t_"+meas_type)
plt.show()

fig, ax = plt.subplots(1,1)
df_size=df.groupby("size").agg({"runtime":['mean','std']})
n_pow=np.log2(df["size"].unique()).astype(int)
df_size=df_size.reset_index()
df_size.columns=["size","mean_runtime","std_runtime"]
#n_pow={11,14,16,18,20,24}
n_pow_str=list()
for n in n_pow:
	n_pow_str.append("2^"+str(n))

df_size.to_csv(meas_type+"/evaluation_n-to-runtime")
plt.errorbar(df_size["size"], df_size['mean_runtime'], yerr=df_size['std_runtime'], marker='x')
plt.ylabel('Mean Runtime in Seconds')
plt.xlabel('Size of Field n')
ax.set_xticks(df_size["size"])
ax.set_xticklabels(n_pow_str)
fig.autofmt_xdate()
plt.margins(0.05, 0.1)
titel='Size to Mean Runtime'
if threads != "-1":
	titel='Size to Mean Runtime - threads '+threads
plt.title(titel)
plt.savefig(img_path+strftime("%Y-%m-%d_%H:%M:%S", gmtime())+"_mean-runtime-vs-size_"+meas_type)
plt.show()


fig, ax = plt.subplots(1,1)
df_size=df.groupby("size").agg({"runtime":['mean','std']})
n_pow=np.log2(df["size"].unique()).astype(int)
df_size=df_size.reset_index()
df_size.columns=["size","mean_runtime","std_runtime"]
#n_pow={11,14,16,18,20,24}
n_pow_str=list()
for n in n_pow:
	n_pow_str.append("2^"+str(n))

plt.loglog(df_size["size"], df_size['mean_runtime'],  marker='x')
plt.ylabel('Mean Runtime in Seconds')
plt.xlabel('Size of Field n')
ax.set_xticks(df_size["size"])
ax.set_xticklabels(n_pow_str)
fig.autofmt_xdate()
plt.margins(0.05, 0.1)
titel='Size to Mean Runtime (loglog)'
if threads != "-1":
	titel='Size to Mean Runtime (loglog) - threads '+threads
plt.title(titel)
plt.savefig(img_path+strftime("%Y-%m-%d_%H:%M:%S", gmtime())+"_loglog_mean-runtime-vs-size_"+meas_type)
plt.show()