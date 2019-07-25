import os
import pandas as pd
import matplotlib.pyplot as plt
from time import gmtime, strftime
import sys
import numpy as np
import os

if(len(sys.argv)!=3):
	print("Please provide path to csv table")
	sys.exit(1);
path=sys.argv[1]
path2=sys.argv[2]

no_filename=path.split(".")[0]
splits=no_filename.split("_")
meas_type=splits[1]
img_path=splits[1]+"/plots/"
if not os.path.exists(img_path):
    os.makedirs(img_path)
print(img_path)
    
df = pd.read_csv(path)
df.sort_values(by=['threads'])
n_pow=np.log2(df["size"].unique()).astype(int)
n_pow_str=list()
for n in n_pow:
	n_pow_str.append("2^"+str(n))
sizes=df["size"].unique().astype(int)
thread_no=df["threads"].unique().astype(int)

#fig1, ax = plt.figure(1)
fig1, ax = plt.subplots(1,1)
means1=list()
err1=list()
count1=list()
for theads in thread_no :
	df_means=df.loc[df['threads'] == theads]
	count1.append(len(df_means.index)) 
	df_means=df_means.groupby("size").agg({"runtime":['mean','std']})
	df_means=df_means.reset_index()
	df_means.columns=["size","mean_runtime","std_runtime"]
	df_means.to_csv(meas_type+"/evaluation_"+meas_type+"_threads-to-runtime")
	means1.append(df_means['mean_runtime'])
	err1.append(df_means['std_runtime'])
	#plt.errorbar(theads, df_means['mean_runtime'], yerr=df_means['std_runtime'], linestyle="None", marker='x', color="red")
plt.errorbar(thread_no, means1, err1,color="red",marker='x', label="Party P1")

i=0
#for txt in thread_no:
#    ax.annotate(count1[i], 
#    	(txt, means1[i]),
#		xytext=(6, 5),
#    	 textcoords='offset pixels'
#    	)
#    i=i+1

df2 = pd.read_csv(path2)
df2.sort_values(by=['threads'])
sizes2=df2["size"].unique().astype(int)
thread_no2=df2["threads"].unique().astype(int)

#fig1 = plt.figure(1)

means2=list()
err2=list()
count2=list()
for theads in thread_no2 :
	df_means2=df2.loc[df2['threads'] == theads]
	count2.append(len(df_means2.index)) 

	df_means2=df_means2.groupby("size").agg({"runtime":['mean','std']})
	df_means2=df_means2.reset_index()
	df_means2.columns=["size","mean_runtime","std_runtime"]
	means2.append(df_means2['mean_runtime'])
	err2.append(df_means2['std_runtime'])


plt.errorbar(thread_no2, means2,err2, color="blue", marker='x', label="Party P2")

#for i, txt in enumerate(thread_no2):
#    ax.annotate(count2[i], 
#    	(txt, means2[i]),
#		xytext=(6, 5),
#    	 textcoords='offset pixels'
#    	)



ax = fig1.gca()
fig1.autofmt_xdate()
plt.ylabel("Mean Runtime in Seconds")
plt.xlabel("Number of Used Threads")
ax.set_xticks(thread_no)
plt.margins(0.05, 0.1)
plt.legend()

plt.title('Number of Used Threads to Mean Runtime:\n n=10^6, t=1 ')
plt.savefig(img_path+strftime("%Y-%m-%d_%H:%M:%S", gmtime())+"_mean-runtime-vs-theads_"+meas_type)	
plt.show()


