import subprocess
import os
import sys
from math import pow

t_vals= [74,192,382,741,1422,5205]
n_vals_pow=[11,14,16,18,20,24] 


for i in range(0,6):
	
	t=t_vals[i]
	n=int(pow(2,n_vals_pow[i]))
			
	for i in range(0,30):
		command1="./bazel-bin/code_master/mpfss_cuckoo/mpfss_cuckoo localhost:99999 1 "+str(t)+" "+str(n)
		command2="./bazel-bin/code_master/mpfss_cuckoo/mpfss_cuckoo localhost:99999 2 "+str(t)+" "+str(n)
		print(command1)
		print(command2)
						

		p1=subprocess.Popen("exec "+ command1, close_fds=True, shell=True)
		p2=subprocess.Popen("exec " +command2, close_fds=True, shell=True)

		while p2.poll()==None:
			if p1.poll()!=None:
				print("ERROR in Party 1")
				os.system("pkill -f './bazel-bin/code_master/mpfss_cuckoo/mpfss_cuckoo'")
				#sys.exit(1)		
		p1.kill()
