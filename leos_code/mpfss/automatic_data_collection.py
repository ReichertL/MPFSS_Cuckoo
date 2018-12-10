import os


#t= (5, 10, 20,50)
#n=(100, 500, 1000)
t=2
size=20

p1="./a.out localhost:88888 1 "+t+" "+size+" > /dev/null &"
p2="./a.out localhost:88888 2 "+t+" "+size+" > /dev/null &"
print(p1)
os.system(p1)
print(p2)
os.system(p2)

print("is ist blocking?")
#for this_t in t:
#	for this_n in n:
#		os.system(p1)
#		os.system(p2)


