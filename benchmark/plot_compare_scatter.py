import os
import pandas as pd
import matplotlib.pyplot as plt
from time import gmtime, strftime
import sys
import numpy as np
import os

if (not(len(sys.argv)==3 or len(sys.argv)==4)):
	print("Please provide path to csv table")
	sys.exit(1);
path1=sys.argv[1]
path2=sys.argv[2]

threads=""
try:
	threads=sys.argv[3]
except:
	threads="-1"
	print(str(threads))

no_filename1=path1.split(".")[0]
splits1=no_filename1.split("_")
meas_type1=splits1[1]
img_path="comp/plots/"
if not os.path.exists(img_path):
    os.makedirs(img_path)
   
df1 = pd.read_csv(path1)
if (threads != "-1"):
	df1.loc[df1['threads'].isin([threads])]
df1.sort_values(by=['t'])
df1_count=df1["t"].value_counts()
df_means1=df1.groupby("t").agg({"runtime":['mean','std']})
df_means1=df_means1.reset_index()
df_means1.columns=["t","mean_runtime","std_runtime"]
#df_means1.to_csv(meas_type1+"/evaluation_t-to-runtime")

no_filename2=path2.split(".")[0]
splits2=no_filename2.split("_")
meas_type2=splits2[1]
img_path="comp/plots/"
if not os.path.exists(img_path):
    os.makedirs(img_path)
   
df2 = pd.read_csv(path2)
if (threads != "-1"):
	df2.loc[df2['threads'].isin([threads])]
df2.sort_values(by=['t'])
df2_count=df2["t"].value_counts()
df_means2=df2.groupby("t").agg({"runtime":['mean','std']})
df_means2=df_means2.reset_index()
df_means2.columns=["t","mean_runtime","std_runtime"]
#df_means2.to_csv(meas_type2+"/evaluation_t-to-runtime")


fig, ax = plt.subplots(1,1)
fig.autofmt_xdate()
plt.scatter(df1["t"], df1['runtime'],  marker='x',  color='blue', label=no_filename1)
plt.plot(df_means1["t"], df_means1['mean_runtime'],  linestyle='-',  color='blue', label=no_filename1)
plt.errorbar(df_means1["t"], df_means1['mean_runtime'], yerr=df_means1['std_runtime'], marker='x',  color='blue', label=no_filename1)
for i, txt in enumerate(df1_count):
    ax.annotate(txt, (df_means1["t"].loc[i], df_means1['mean_runtime'].loc[i]))

plt.ylabel('Mean Runtime in Seconds')
plt.xlabel('Number Indices t')
plt.xticks(df2["t"].unique().tolist())
plt.margins(0.05, 0.1)
titel='Number of Indices to Mean Runtime'
if threads != "-1":
	titel='Number of Indices to Mean Runtime - threads: '+threads
plt.title(titel)



plt.scatter(df2["t"], df2['runtime'], marker='x',  color='red', label=no_filename2)
plt.plot(df_means2["t"], df_means2['mean_runtime'], linestyle='-',  color='red', label=no_filename2)
plt.errorbar(df_means2["t"], df_means2['mean_runtime'], yerr=df_means2['std_runtime'], marker='x',  color='red', label=no_filename2)

plt.legend()
plt.savefig(img_path+strftime("%Y-%m-%d_%H:%M:%S", gmtime())+"_mean-runtime-vs-t_ compare_"+meas_type1+"_"+meas_type2)
plt.show()