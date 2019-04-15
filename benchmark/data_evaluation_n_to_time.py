#!/usr/bin/env python3



import os
import pandas as pd
import matplotlib.pyplot as plt

x_t=list()
x_size=list()
directory="Naive"
target_size=1000
target_t= 10

size_fix=False
t_fix=not size_fix

for filename in os.listdir(directory):
	parts=filename.split("_")
	if(len(parts)>2):
		part_t=parts[1]
		part_size=parts[2]
		t=int(part_t.split(":")[1])
		size=int(part_size.split(":")[1])
		if((size_fix and size==target_size) or (t_fix and t==target_t)):
			if(t not in x_t):
				x_t.append(t)
				x_t.sort()

			if(size not in x_size):
				x_size.append(size)
				x_size.sort()

y_t_temp=list()
y_size_temp=list()

for i in range(0,len(x_t)):
	y_t_temp.append([])
	
for i in range(0,len(x_size)):
	y_size_temp.append([])

for filename in os.listdir(directory):
	path=directory+"/"+filename
	parts=filename.split("_")
	if(len(parts)>1):
		part_t=parts[1]
		part_size=parts[2]
		t=int(part_t.split(":")[1])
		size=int(part_size.split(":")[1])

		if((size_fix and size==target_size) or (t_fix and t==target_t)):
			
			df = pd.read_csv(path)
			
			this_datapoints=df[' Runtime in Seconds'].get_values()
			#print(df.get_values())

			n=len(df.index)
			mean=df.mean(0)[1]
			std=df.std(0)[1]

			index_size=x_size.index(size);
			index_t=x_t.index(t);

			y_t_temp[index_t]=y_t_temp[index_t]+this_datapoints.tolist()
			y_size_temp[index_size]=y_size_temp[index_size]+this_datapoints.tolist()
			#print(y_size_temp[0])
			#print("added to y_size_temp["+str(index_size)+"]")

df_res_t=pd.DataFrame(y_t_temp)
df_res_size=pd.DataFrame(y_size_temp)


mean_t=df_res_t.mean(axis=1).get_values()
std_t=df_res_t.std(axis=1)

mean_size=df_res_size.mean(axis=1).get_values()
std_size=df_res_size.std(axis=1)

plt.figure(0) 

plt.errorbar(x_t, mean_t, std_t, marker='^')
plt.xlabel('Anzahl "t" der Punkte der MPFSS ')
plt.ylabel('Dauer in Sekunden')
plt.xticks(x_t)
plt.margins(0.05, 0.1)
plt.title(' t zu Kommunikationsdauer(MPFSS_naive)\n')
plt.savefig("MPFSS_naive-x:t-y:time.png")

plt.figure(1) 
plt.errorbar(x_size, mean_size, std_size, marker='^')
#fig, ax = plt.subplots()
#plt.xscale('log')
plt.xlabel('Größes des Inputfeldes "size" ')
plt.ylabel('Dauer in Sekunden')
plt.xticks(x_size)
plt.margins(0.05, 0.1)
plt.title(' "size" zu Kommunikationsdauer (MPFSS_naive) \n ')
plt.savefig("MPFSS_naive-x:size-y:time.png")

