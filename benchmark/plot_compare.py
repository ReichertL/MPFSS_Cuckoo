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

caption="1"
try:
	caption=sys.argv[3]
except:
	caption="1"
	print(str(caption))

threads=8


no_filename1=path1.split(".")[0]
splits1=no_filename1.split("_")
meas_type1=splits1[1]
img_path="comp/plots/"
if not os.path.exists(img_path):
    os.makedirs(img_path)
   
df1_org = pd.read_csv(path1)
df1=df1_org.loc[df1_org['threads']==threads]
df1.sort_values(by=['t'])
df1_count=df1["t"].value_counts().reset_index()
df1_count.columns=["t","count"]

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
   
df2_org = pd.read_csv(path2)
df2=df2_org.loc[df2_org['threads']==threads]
df2.sort_values(by=['t'])
df2_count=df2["t"].value_counts().reset_index()
df2_count.columns=["t","count"]

df_means2=df2.groupby("t").agg({"runtime":['mean','std']})
df_means2=df_means2.reset_index()
df_means2.columns=["t","mean_runtime","std_runtime"]
#df_means2.to_csv(meas_type2+"/evaluation_t-to-runtime")

if caption=="2":
	n=1000000
	lable1=no_filename1
	lable2=no_filename2
	df_means1["t"]=df_means1["t"].div(n).mul(100)	
	df_means2["t"]=df_means2["t"].div(n).mul(100)	
	ticks=df_means2["t"]	
	titel='Precentage of Indices to n (n='+str(n)+') \ncompared to Mean Runtime '
	if threads != "-1":
		titel=titel+" - threads: "+str(threads)
else:
	lable1=no_filename1
	lable2=no_filename2
	ticks=df_means1["t"]
	titel='Number of Indices to Mean Runtime'
	if threads != "-1":
		titel=titel+" - threads: "+str(threads)


fig, ax = plt.subplots(1,1)
fig.autofmt_xdate()
plt.errorbar(df_means1["t"], df_means1['mean_runtime'], yerr=df_means1['std_runtime'], marker='x',  color='blue', label=lable1)

print(no_filename1)
test1=df_means1.merge(df1_count, left_on='t', right_on='t')
print(test1)

for i in range(0,len(test1.index)):
    ax.annotate(test1["count"].loc[i], 
    	(test1["t"].loc[i], test1['mean_runtime'].loc[i]),
		xytext=(6, 5),
    	 textcoords='offset pixels'
    	)

plt.ylabel('Mean Runtime in Seconds')
plt.xlabel('Number Indices t')
plt.xticks(ticks)
plt.margins(0.05, 0.1)
plt.title(titel)

plt.errorbar(df_means2["t"], df_means2['mean_runtime'], yerr=df_means2['std_runtime'], marker='x',  color='red', label=lable2)

print(no_filename2)
test2=df_means2.merge(df2_count, left_on='t', right_on='t')
print(test2)

for i in range(0,len(test2.index)):
    ax.annotate(test2["count"].loc[i], 
    	(test2["t"].loc[i], test2['mean_runtime'].loc[i]),
		xytext=(6, 5),
    	 textcoords='offset pixels'
    	)

plt.legend()
plt.savefig(img_path+strftime("%Y-%m-%d_%H:%M:%S", gmtime())+"_mean-runtime-vs-t_ compare_"+meas_type1+"_"+meas_type2)
plt.show()