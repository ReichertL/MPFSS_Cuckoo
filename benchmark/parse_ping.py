import sys
import numpy


filename =sys.argv[1] # subprocess.check_output('ping -c %s -W %s -t %s 8.8.8.8' % (self.numPings, (self.pingTimeout * 1000), self.maxWaitTime), shell=True)
f = open(filename, "r")
text=f.read()
output = text.split('\n')


all_l=list()

for line in output:
	line_arr=line.split(" ")
	if len(line_arr)>2 	:
		last=line_arr[-2]
		time=last.split("=")
		if len(time)==2:
			this_time=float(time[1])
			all_l.append(this_time)

a = numpy.array(all_l)
std = numpy.std(a)
mean = numpy.mean(a)

print("min "+str(numpy.min(a)))
print("max "+str(numpy.max(a)))
print("mean "+str(numpy.mean(a)))
print("std "+str(numpy.std(a)))

#print(output)
# -1 is a blank line, -3 & -2 contain the actual results

#xmit_stats = output[0].split(",")
#timing_stats = output[1].split("=")[1].split("/")

#packet_loss = float(xmit_stats[2].split("%")[0])

#ping_min = float(timing_stats[0])
#ping_avg = float(timing_stats[1])
#ping_max = float(timing_stats[2])