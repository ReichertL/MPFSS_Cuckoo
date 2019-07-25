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

ts=df["t"].unique().astype(int)
thread_no=df["threads"].unique().astype(int)

fig1, ax = plt.subplots(1,1)

for t in ts:	
	df_t=df.loc[df['t'] == t]

	means=list()
	err=list()
	count=list()

	for theads in thread_no :
		df_means=df_t.loc[df['threads'] == theads]
		count.append(len(df_means.index))
		df_means=df_means.groupby("t").agg({"runtime":['mean','std']})
		df_means=df_means.reset_index()
		df_means.columns=["t","mean_runtime","std_runtime"]
		df_means.to_csv(meas_type+"/evaluation_"+meas_type+"_threads-to-runtime")
		means.append(df_means["mean_runtime"])
		err.append(df_means['std_runtime'])
		
	plt.errorbar(thread_no, means, err, marker='x', label="t: "+str(t))

	#for i, txt in enumerate(thread_no):
	 #   ax.annotate(count[i], 
	  #  	(txt, means[i]),
	#		xytext=(6, 5),
	 #   	 textcoords='offset pixels'
	 #   	)

	print(t)
	print(thread_no)
	print(count)

ax = fig1.gca()
fig1.autofmt_xdate()
plt.ylabel('Mean Runtime in Seconds')
plt.xlabel('Number of Threads')
ax.set_xticks(thread_no)
plt.margins(0.05, 0.1)
plt.legend()
plt.title('Number of Threads to Mean Runtime:\n n=10^6')
print(img_path)
plt.savefig(img_path+strftime("%Y-%m-%d_%H:%M:%S", gmtime())+"_mean-runtime-vs-theads_"+meas_type)	
plt.show()


