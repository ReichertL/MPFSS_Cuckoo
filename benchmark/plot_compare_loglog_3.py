import os
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
from time import gmtime, strftime
import sys
import numpy as np
import os

if (not(len(sys.argv)==4 or len(sys.argv)==5)):
	print("Please provide path to csv table")
	sys.exit(1);
path1=sys.argv[1]
path2=sys.argv[2]
path3=sys.argv[3]
caption="1"
try:
	caption=sys.argv[4]
except:
	caption="1"
	print(str(caption))

threads=8
lim=0
no_filename1=path1.split(".")[0]
splits1=no_filename1.split("_")
meas_type1=splits1[1]
img_path="comp/plots/"
if not os.path.exists(img_path):
    os.makedirs(img_path)
   
df1_org = pd.read_csv(path1)
df1=df1_org.loc[df1_org['threads']==threads]
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
   
df2_org = pd.read_csv(path2)

df2=df2_org.loc[df2_org['threads']==threads]
df2.sort_values(by=['t'])
df2_count=df2["t"].value_counts()

df_means2=df2.groupby("t").agg({"runtime":['mean','std']})
df_means2=df_means2.reset_index()
df_means2.columns=["t","mean_runtime","std_runtime"]
#df_means2.to_csv(meas_type2+"/evaluation_t-to-runtime")

no_filename3=path3.split(".")[0]
img_path="comp/plots/"
if not os.path.exists(img_path):
    os.makedirs(img_path)
   
df3_org = pd.read_csv(path3)

df3=df3_org.loc[df3_org['threads']==threads]
df3.sort_values(by=['t'])
df3_count=df3["t"].value_counts()

df_means3=df3.groupby("t").agg({"runtime":['mean','std']})
df_means3=df_means3.reset_index()
df_means3.columns=["t","mean_runtime","std_runtime"]
#df_means2.to_csv(meas_type2+"/evaluation_t-to-runtime")

if caption=="2":
	n=1000000
	lable1=no_filename1
	lable2=no_filename2
	lable3=no_filename3

	df_means1["t"]=df_means1["t"].div(n).mul(100)	
	df_means2["t"]=df_means2["t"].div(n).mul(100)	
	df_means3["t"]=df_means3["t"].div(n).mul(100)	

	ticks=df_means2["t"]	
	titel='Precentage of Indices to n (n='+str(n)+') \ncompared to Mean Runtime '
	if threads != "-1":
		titel=titel+" - Threads: "+str(threads)
if caption=="3":
	lable1="MPFSS Naive P1"
	lable2="MPFSS Naive P2"

	lable3="MPFSS Cuckoo"
	ticks=df_means2["t"]
	titel='Number of Indices to Mean Runtime'
	if threads != "-1":
		titel=titel+" - Threads: "+str(threads)
else:
	lable1=no_filename1
	lable2=no_filename2
	lable3=no_filename2

	ticks=df_means1["t"]
	titel='Number of Indices to Mean Runtime'
	if threads != "-1":
		titel=titel+" - Threads: "+str(threads)




fig, ax = plt.subplots(1,1)
fig.autofmt_xdate()
ax.set_xscale("log")#, nonposx='clip')
ax.set_yscale("log", nonposy='clip')

plt.errorbar(df_means1["t"], df_means1['mean_runtime'], yerr=df_means1['std_runtime'], marker='x',  color='blue', label=lable1)
print(df1_count)
for i, txt in enumerate(df1_count):
	if(txt<=lim):
	    ax.annotate(txt, 
	    	(df_means1["t"].loc[i], df_means1['mean_runtime'].loc[i]),
			xytext=(6, 5),
	    	 textcoords='offset pixels'
	    	)

plt.ylabel('Mean Runtime in Seconds')
plt.xlabel('Number Indices t')
#plt.xticks(ticks)
ax.set_xticks(ticks)
ax.get_xaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())
plt.margins(0.05, 0.1)
plt.title(titel)



plt.errorbar(df_means2["t"], df_means2['mean_runtime'], yerr=df_means2['std_runtime'], marker='x',  color='green', label=lable2)
print(df2_count)
for i, txt in enumerate(df2_count):
	if txt <=lim:
	    ax.annotate(txt, 
	    	(df_means2["t"].loc[i], df_means2['mean_runtime'].loc[i]),
			xytext=(6, 5),
	    	 textcoords='offset pixels')

plt.errorbar(df_means3["t"], df_means3['mean_runtime'], yerr=df_means3['std_runtime'], marker='x',  color='red', label=lable3)
print(df3_count)
for i, txt in enumerate(df3_count):
	if txt <=lim:
	    ax.annotate(txt, 
	    	(df_means3["t"].loc[i], df_means3['mean_runtime'].loc[i]),
			xytext=(6, 5),
	    	 textcoords='offset pixels')


plt.legend(loc='upper left')
plt.savefig(img_path+strftime("%Y-%m-%d_%H:%M:%S", gmtime())+"_mean-runtime-vs-t_ compare_"+meas_type1+"_"+meas_type2)
plt.show()