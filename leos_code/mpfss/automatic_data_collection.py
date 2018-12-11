import subprocess

repeat= 20
t= (5, 10, 20,50)
size=(100, 1000, 10000 )


while(repeat>0):
	for this_t in t:
		for this_size in size:
			c1="./a.out localhost:55555 1 "+str(this_t)+" "+str(this_size)+" > /dev/null"
			c2="./a.out localhost:55555 2 "+str(this_t)+" "+str(this_size)+" > /dev/null"
			
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


