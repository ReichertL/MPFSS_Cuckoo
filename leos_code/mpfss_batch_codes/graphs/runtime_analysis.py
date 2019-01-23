import math
from math import log
import matplotlib.pyplot as plt

epsilon= [0.00001, 0.0001, 0.001, 0.01]  
n=1000000
t_fix=100
#s=list(range(1,t_fix, round(t_fix/10)))
d_max=t_fix//5
s=list(range(1,d_max, d_max//10))

#expansion factor
#s=[1.1 , 1.2, 1.3, 1.4, 1.5, 2, 2.5, 3, 3.5 , 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5]


def plot_this( x_value_exact, y_values_exact, name_xaxis, name_yaxis, title_addition, lable_addition, lable ):

  plt.plot(x_value_exact, y_values_exact, lable, label= lable_addition)

  plt.xlabel(name_xaxis)
  plt.ylabel(name_yaxis)
  plt.margins(0.05, 0.1)
  plt.title(name_xaxis+" vs. "+name_yaxis + "\n"+title_addition)


def main():

  #Runtime
  print("Runtime")

  rt_bc=list()
  rt_bc_worstcase=list()
  rt_naive=list()
  rt_bc_ll=list()
  d=list()
  m=list()

  plt.figure()
  for e in epsilon:
  	rt_bc_e=list()
  	rt_bc_worstcase_e=list()
  	rt_naive_e=list()
  	rt_bc_ll_e=list()
  	d_round_e=list()

  	for ss in s:
  		d_round=round((1+ss)*e+1)
  		m_round= round(pow(t_fix, 1+e))
  		d.append(d_round)
  		m.append(m_round)
  		runtime_bc=t_fix+m_round+n*d_round*log(n*d_round/m_round)+t_fix*m_round/2*log(n*d_round/m_round)+t_fix*(n*d_round)/m_round+pow(m_round,2)
  		runtime_bc_wc=t_fix+m_round+n*d_round*n*d_round/m_round+t_fix*m_round/2*n*d_round/m_round+t_fix*(n*d_round)/m_round+pow(m_round,2)
  		runtime_bc_ll=t_fix+m_round+n*d_round+t_fix*m_round/2*n+t_fix*n*d_round/m_round+m_round*m_round
  		runtime_naive=n*t_fix
  		rt_bc_worstcase.append(runtime_bc_wc)
  		rt_bc_ll.append(runtime_bc_ll)
  		rt_bc.append(runtime_bc)
  		rt_naive.append(runtime_naive)

  		d_round_e.append(d_round)
  		rt_bc_worstcase_e.append(runtime_bc_wc)
  		rt_bc_ll_e.append(runtime_bc_ll)
  		rt_bc_e.append(runtime_bc)
  		rt_naive_e.append(runtime_naive)

  	plot_this( d_round_e, rt_bc_e, "Repetition d", "Runtime" , "n:"+str(n)+" t: "+str(t_fix), "Batch Codes e:"+str(e), 'o')


  filename="Runtimeanalysis-x:d-y:runtime-t:"+str(t_fix)+"-n:"+str(n)+"-colored-batch_codes_avarage_case"
  art = []
  lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
  art.append(lgd)
  plt.savefig("runtime/"+filename+".png", additional_artists=art, bbox_inches="tight")
  plt.close()  		

  plt.figure()
  plot_this( d, rt_bc, "Repetition d", "Runtime" , "n:"+str(n)+"t: "+str(t_fix), "Batch Codes", 'o')
  plot_this( d, rt_bc_worstcase, "Repetition d", "Runtime" , "n:"+str(n)+"t: "+str(t_fix), "Batch Codes Worts case", 'rx')
  plot_this( d, rt_bc_ll, "Repetition d", "Runtime" , "n:"+str(n)+"t: "+str(t_fix), "Batch Codes Linked List", 'yo')
  plot_this( d, rt_naive, "Repetition d", "Runtime" , "n:"+str(n)+"t: "+str(t_fix), "Naive", 'x')
  filename="Runtimeanalysis-x:d-y:runtime-t:"+str(t_fix)+"-n:"+str(n)+"-comparison"
  art = []
  lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
  art.append(lgd)
  plt.savefig("runtime/"+filename+".png", additional_artists=art, bbox_inches="tight")
  plt.close()
  plt.figure()
  plot_this( m, rt_bc, "No Batches m", "Runtime" , "n:"+str(n)+"t: "+str(t_fix), "Batch Codes", 'o-')
  plot_this( m, rt_bc_worstcase, "Repetition d", "Runtime" , "n:"+str(n)+"t: "+str(t_fix), "Batch Codes Worts case", 'rx')
  plot_this( m, rt_bc_ll, "Repetition d", "Runtime" , "n:"+str(n)+"t: "+str(t_fix), "Batch Codes Linked List", 'go-')
  plot_this( m, rt_naive, "No Batches m", "Runtime" , "n:"+str(n)+"t: "+str(t_fix), "Naive", 'x--')
  filename="Runtimeanalysis-x:m-y:runtime-t:"+str(t_fix)+"-n:"+str(n)+"-comparison"
  art = []
  lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
  art.append(lgd)
  plt.savefig("runtime/"+filename+".png", additional_artists=art, bbox_inches="tight")
  plt.close()

main()