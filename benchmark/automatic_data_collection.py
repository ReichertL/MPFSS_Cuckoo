import subprocess

repeat= 20
t=[2,10,20,100, 200]
size=(1000, 10000, 100000, 1000000 )

location= "./../bazel-bin/semesterprojekt/code_projekt/mpfss_naive/mpfss_naive"
while(repeat>0):
	for this_t in t:
		for this_size in size:
			c1=location+" localhost:55555 1 "+str(this_t)+" "+str(this_size)+" > /dev/null"
			c2=location+" localhost:55555 2 "+str(this_t)+" "+str(this_size)+" > /dev/null"
			
			print(c1)
			p1 = subprocess.Popen(c1, shell=True )
			
			print(c2)
			p2 = subprocess.Popen(c2, shell=True )
			p2.wait();

			try:
			    while p2.poll() is None:
			        time.sleep(0.1)

			except KeyboardInterrupt:
			    p2.terminate()
			    p1.terminate()
			    raise

#		subprocess.check_call(p1, shell=True)
	repeat=repeat-1;		


