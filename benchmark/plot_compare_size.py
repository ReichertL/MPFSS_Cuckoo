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


lim=0;


no_filename1=path1.split(".")[0]
splits1=no_filename1.split("_")
meas_type1=splits1[1]
img_path="comp/plots/"
if not os.path.exists(img_path):
    os.makedirs(img_path)
   
df1_org = pd.read_csv(path1)
sizes1=df1_org["size"].unique()

#fig, ax = plt.subplots(1,1)
fig = plt.figure(1)
ax = fig.add_subplot(111)
fig.autofmt_xdate()
cm = plt.get_cmap('winter')
NUM_COLORS=len(sizes1)
ax.set_color_cycle([cm(1.*i/NUM_COLORS) for i in range(NUM_COLORS)])

for size in sizes1:
	df1=df1_org.loc[df1_org['size']==size]
	df1.sort_values(by=['t'])
	df1_count=df1["t"].value_counts().reset_index()
	df1_count.columns=["t","count"]

	df_means1=df1.groupby("t").agg({"runtime":['mean','std']})
	df_means1=df_means1.reset_index()
	df_means1.columns=["t","mean_runtime","std_runtime"]
	plt.errorbar(df_means1["t"], df_means1['mean_runtime'], yerr=df_means1['std_runtime'], marker='x', label="MPFSS Naive - Party P1 (n:"+str(size)+")")


no_filename2=path2.split(".")[0]
splits2=no_filename2.split("_")
meas_type2=splits2[1]
img_path="comp/plots/"
if not os.path.exists(img_path):
    os.makedirs(img_path)
   
df2_org = pd.read_csv(path2)
sizes2=df2_org["size"].unique()
cm = plt.get_cmap('spring')
NUM_COLORS=len(sizes2)
ax.set_color_cycle([cm(1.*i/NUM_COLORS) for i in range(NUM_COLORS)])

for size in sizes2:
	df2=df2_org.loc[df2_org['size']==size]
	df2.sort_values(by=['t'])
	df2_count=df2["t"].value_counts().reset_index()
	df2_count.columns=["t","count"]

	df_means2=df2.groupby("t").agg({"runtime":['mean','std']})
	df_means2=df_means2.reset_index()
	df_means2.columns=["t","mean_runtime","std_runtime"]
	plt.errorbar(df_means2["t"], df_means2['mean_runtime'], yerr=df_means2['std_runtime'], marker='x', label="MPFSS Cuckoo (n:"+str(size)+")")


if caption=="2":
	n=1000000
	lable1=no_filename1
	lable2=no_filename2
	df_means1["t"]=df_means1["t"].div(n).mul(100)	
	df_means2["t"]=df_means2["t"].div(n).mul(100)	
	ticks=df_means2["t"]	
	titel='Precentage of Indices to n (n='+str(n)+') \ncompared to Mean Runtime '

else:
	lable1=no_filename1
	lable2=no_filename2
	ticks=df_means1["t"]
	titel='Number of Indices to Mean Runtime'





print(no_filename1)
test1=df_means1.merge(df1_count, left_on='t', right_on='t')
print(test1)

#for i in range(0,len(test1.index)):
 #   if test1["count"].loc[i]<lim:
 #   	ax.annotate(test1["count"].loc[i], 
  #  	(test1["t"].loc[i], test1['mean_runtime'].loc[i]),
#		xytext=(6, 5),
#    	 textcoords='offset pixels'
#    	)

plt.ylabel('Mean Runtime in Seconds')
plt.xlabel('Number Indices t')
plt.xticks(ticks)
plt.margins(0.05, 0.1)
plt.title(titel)


print(no_filename2)
test2=df_means2.merge(df2_count, left_on='t', right_on='t')
print(test2)

#for i in range(0,len(test2.index)):
#	if test2["count"].loc[i]<lim:
#	    ax.annotate(test2["count"].loc[i], 
#	    	(test2["t"].loc[i], test2['mean_runtime'].loc[i]),
#			xytext=(6, 5),
#	    	 textcoords='offset pixels'
#	    	)
box = ax.get_position()

handles, labels = ax.get_legend_handles_labels()
lgd = ax.legend(handles, labels, loc='upper left', bbox_to_anchor=(0,-0.2))
text = ax.text(-0.2,1.05, "", transform=ax.transAxes)
#ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
#fig.subplots_adjust(right=0.4) 
#plt.legend(loc='center left', bbox_to_anchor=(1, 0.5))
#plt.savefig()
name=img_path+strftime("%Y-%m-%d_%H:%M:%S", gmtime())+"_mean-runtime-vs-t_ compare_"+meas_type1+"_"+meas_type2
fig.savefig(name, bbox_extra_artists=(lgd,text), bbox_inches='tight')
plt.show()