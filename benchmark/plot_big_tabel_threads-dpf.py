import os
import pandas as pd
import matplotlib.pyplot as plt
from time import gmtime, strftime
import sys
import numpy as np
import os

if(len(sys.argv)!=2):
	print("Please provide path to csv table")
	sys.exit(1);
path=sys.argv[1]
no_filename=path.split(".")[0]
splits=no_filename.split("_")
meas_type=splits[1]
img_path=splits[1]+"/plots/"
if not os.path.exists(img_path):
    os.makedirs(img_path)
    
df = pd.read_csv(path)
df.sort_values(by=['threads'])
n_pow=np.log2(df["size"].unique()).astype(int)
n_pow_str=list()
for n in n_pow:
	n_pow_str.append("2^"+str(n))
sizes=df["size"].unique().astype(int)
thread_no=df["threads"].unique().astype(int)

fig1 = plt.figure(1)
for theads in thread_no :
	df_means=df.loc[df['threads'] == theads]
	df_means=df_means.groupby("size").agg({"runtime":['mean','std']})
	df_means=df_means.reset_index()
	df_means.columns=["size","mean_runtime","std_runtime"]
	df_means.to_csv(meas_type+"/evaluation_"+meas_type+"_threads-to-runtime")
	plt.errorbar(df_means["size"], df_means['mean_runtime'], yerr=df_means['std_runtime'], marker='x')
	print(df_means)
ax = fig1.gca()
fig1.autofmt_xdate()
plt.ylabel('Mean Runtime')
plt.xlabel('Number of Used Threads')
ax.set_xticks(sizes)
ax.set_xticklabels(n_pow_str)
plt.margins(0.05, 0.1)
plt.title('Number of Used Threads to Mean Runtime:\n n=2^20, t=1422, cprg ')
plt.savefig(img_path+strftime("%Y-%m-%d_%H:%M:%S", gmtime())+"_mean-runtime-vs-theads_"+meas_type)	
plt.show()


