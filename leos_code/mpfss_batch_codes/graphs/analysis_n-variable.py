from math import log
import matplotlib.pyplot as plt
from decimal import Decimal

t=5000
p_inverse=pow(10.,-25.)
print(p_inverse)
p=Decimal(1)-Decimal(p_inverse)
print(p)

def plot_this( x_value_exact, y_values_exact, name_xaxis, name_yaxis, title_addition, lable_addition, lable ):

  plt.plot(x_value_exact, y_values_exact, lable, label= lable_addition)

  plt.xlabel(name_xaxis)
  plt.ylabel(name_yaxis)
  plt.grid(True)
  plt.margins(0.05, 0.1)
  plt.title(name_xaxis+" vs. "+name_yaxis + "\n"+title_addition)

def plot_this_semilogy( x_value_exact, y_values_exact, name_xaxis, name_yaxis, title_addition, lable_addition, lable ):

  plt.semilogy(x_value_exact, y_values_exact, lable, label= lable_addition)

  plt.xlabel(name_xaxis)
  plt.ylabel(name_yaxis)
  plt.grid(True)
  plt.margins(0.05, 0.1)
  plt.title(name_xaxis+" vs. "+name_yaxis + "\n"+title_addition)


def plot_this_loglog( x_value_exact, y_values_exact, name_xaxis, name_yaxis, title_addition, lable_addition, lable ):

  plt.loglog(x_value_exact, y_values_exact, lable, label= lable_addition)
  plt.xlabel(name_xaxis)
  plt.ylabel(name_yaxis)
  plt.grid(True)
  plt.margins(0.05, 0.1)
  plt.title(name_xaxis+" vs. "+name_yaxis + "\n"+title_addition)


def main():
	n_list = [10000. ,50000., 100000, 500000,1000000, 2000000, 3000000, 4000000,  5000000]
	s_list=list()
	d_list=list()
	e_list=list()
	m_list=list()
	rt_list=list()
	rt_list_bst=list()
	rt_list_ll=list()
	rt_list_naive=[2*n*t for n in n_list]
	t_to_n_list=[t/n for n in n_list]

	for n in n_list:
		s=log(n)/log(t)
		s_list.append(s)

		d=log(Decimal(1)-Decimal(p))/(-2*log(t))+1
		d_list.append(d)

		e=(d-1)/(s+1)
		e_list.append(e)

		m=pow(t, 1+e)
		m_list.append(m)

		rt= t*n*d/m+n*d+t*d
		rt_list.append(rt)

		rt_bst= t*n*d/m+n*d+t*m/2*((n*d)/m)
		rt_list_bst.append(rt_bst)

		rt_ll= t*n*d/m+n*d+t*n*m/2
		rt_list_ll.append(rt_ll)


	plt.figure()
	plot_this( t_to_n_list, rt_list, "Quotient t/n", "Runtime rt ", " t: "+str(t)+" p: 1-"+str(p_inverse), "Batch Codes", "o-" )
	plot_this( t_to_n_list, rt_list_naive, "Quotient t/n", "Runtime rt ", " t: "+str(t)+" p: 1-"+str(p_inverse), "Naive", "x--" )
	filename="Analysis-x:t_to_n-y:rt-p:1-"+str(p_inverse)+"-t:"+str(t)
	art = []
	lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
	art.append(lgd)
	plt.savefig("runtime/n_variable/"+filename+".png", additional_artists=art, bbox_inches="tight")
	plt.close()

	plt.figure()
	plot_this( n_list, rt_list, "Size of Inputfield n ", "Runtime rt ", " t: "+str(t)+" p: 1-"+str(p_inverse), "Batch Codes", "o-" )
	plot_this( n_list, rt_list_naive, "Size of Inputfield ", "Runtime rt ", " t: "+str(t)+" p: 1-"+str(p_inverse), "Naive", "x--" )
	filename="Analysis-x:n-y:rt-p:1-"+str(p_inverse)+"-t:"+str(t)
	art = []
	lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
	art.append(lgd)
	plt.savefig("runtime/n_variable/"+filename+".png", additional_artists=art, bbox_inches="tight")
	plt.close()

	plt.figure()
	plot_this( n_list, m_list, "Size of Inputfield n ", "Number of Batches m ", " t: "+str(t)+" p: 1-"+str(p_inverse), "", "o-" )
	filename="Analysis-x:n-y:m-p:1-"+str(p_inverse)+"-t:"+str(t)
	art = []
	lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
	art.append(lgd)
	plt.savefig("m/"+filename+".png", additional_artists=art, bbox_inches="tight")
	plt.close()

	plt.figure()
	plot_this( n_list, d_list, "Size of Inputfield n ", "Repetition Factor d ", " t: "+str(t)+" p: 1-"+str(p_inverse), "", "o-" )
	filename="Analysis-x:n-y:d-p:1-"+str(p_inverse)+"-t:"+str(t)
	art = []
	lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
	art.append(lgd)
	plt.savefig("d/"+filename+".png", additional_artists=art, bbox_inches="tight")
	plt.close()


	#plt.figure()
	#plot_this( t_to_n_list, rt_list, "Quotient t/n", "Runtime rt", " t: "+str(t)+" p: 1-"+str(p_inverse), "Batch Codes", "bo-" )
	#plot_this( t_to_n_list, rt_list_bst, "Quotient t/n", "Runtime rt", " t: "+str(t)+" p: 1-"+str(p_inverse), "Batch Codes(BST)", "ro-" )
	#filename="Analysis-x:t_to_n-y:rt-p:1-"+str(p_inverse)+"-n:"+str(n)+"-comparison-to-bst"
	#art = []
	#lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
	#art.append(lgd)
	#plt.savefig("runtime/n_variable/"+filename+".png", additional_artists=art, bbox_inches="tight")
	#plt.close()

main()