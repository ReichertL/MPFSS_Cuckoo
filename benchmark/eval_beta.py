import os
import pandas as pd
import matplotlib.pyplot as plt
from time import gmtime, strftime
import sys
import numpy as np
import os

if len(sys.argv)!=2 :
	print("Please provide path to csv table")
	sys.exit(1);
path1=sys.argv[1]



threads=8


no_filename1=path1.split(".")[0]
splits1=no_filename1.split("_")
meas_type1=splits1[1]

df1_org = pd.read_csv(path1)
df1=df1_org.loc[df1_org['threads']==threads]
df1.sort_values(by=['t'])
df1_count=df1["t"].value_counts().reset_index()
df1_count.columns=["t","count"]

df_means1=df1.groupby("t").agg({"create_beta_vals":['mean','std']})
df_means1=df_means1.reset_index()
df_means1.columns=["t","create_beta_vals mean","create_beta_vals std"]
#df_means1.to_csv(meas_type1+"/evaluation_t-to-runtime")



print(no_filename1)
test1=df_means1.merge(df1_count, left_on='t', right_on='t')
print(test1)

