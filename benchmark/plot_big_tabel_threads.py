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
df_means=df.groupby("threads").agg({"runtime":['mean','std']})
df_means=df_means.reset_index()
df_means.columns=["threads","mean_runtime","std_runtime"]
df_means.to_csv(meas_type+"/evaluation_"+meas_type+"_threads-to-runtime")
fig, ax = plt.subplots(1,1)
fig.autofmt_xdate()
plt.errorbar(df_means["threads"], df_means['mean_runtime'], yerr=df_means['std_runtime'], marker='x')
plt.ylabel('Mean Runtime')
plt.xlabel('Number of Used Threads')
plt.xticks(df_means["threads"])
plt.margins(0.05, 0.1)
plt.title('Number of Used Threads to Mean Runtime:\n n=10^6, t=1 ')
plt.savefig(img_path+strftime("%Y-%m-%d_%H:%M:%S", gmtime())+"_mean-runtime-vs-theads_"+meas_type)
plt.show()


