from math import log
import matplotlib.pyplot as plt

n=1000000.
p=pow(10.,-5.)

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
	t_list = [ 50., 500., 1000., 2000., 3000., 4000., 5000., 10000., 20000., 30000.,40000., 50000., 100000., 200000., 300000.,500000.]
	s_list=list()
	d_list=list()
	e_list=list()
	m_list=list()
	rt_list=list()
	rt_list_bst=list()
	rt_list_ll=list()
	rt_list_naive=[n*n*t for t in t_list]
	t_to_n_list=[t/n for t in t_list]

	for t in t_list:
		s=log(n)/log(t)
		s_list.append(s)

		d=log(-p+1)/(-2*log(t))
		d_list.append(d+1)

		e=(d)/(s+1)
		e_list.append(e)

		m=pow(t, 1+e)
		m_list.append(m)

		rt= t*n*(d+1)/m+n*d+t*d
		rt_list.append(rt)

		rt_bst= t*n*(d+1)/m+n*d+t*m/2*((n*d)/m)
		rt_list_bst.append(rt_bst)

		rt_ll= t*n*(d+1)/m+n*d+t*n*m/2
		rt_list_ll.append(rt_ll)


	plt.figure()
	plot_this( t_to_n_list, e_list, "Quotient t/n", "Epsilon e ", " n: "+str(n)+" p: "+str(p), "", "o-" )
	filename="Analysis-x:t_to_n-y:e-p:"+str(p)+"-n:"+str(n)
	art = []
	lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
	art.append(lgd)
	plt.savefig("e/"+filename+".png", additional_artists=art, bbox_inches="tight")
	plt.close()

	plt.figure()
	plot_this( t_to_n_list, d_list, "Quotient t/n", "Repetition Factor d ", " n: "+str(n)+" p: "+str(p), "", "o-" )
	filename="Analysis-x:t_to_n-y:d-p:"+str(p)+"-n:"+str(n)
	art = []
	lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
	art.append(lgd)
	plt.savefig("d/"+filename+".png", additional_artists=art, bbox_inches="tight")
	plt.close()


	plt.figure()
	plot_this( t_to_n_list, rt_list, "Quotient t/n", "Runtime rt ", " n: "+str(n)+" p: "+str(p), "Batch Codes", "o-" )
	plot_this( t_to_n_list, rt_list_naive, "Quotient t/n", "Runtime rt ", " n: "+str(n)+" p: "+str(p), "Naive", "x--" )
	filename="Analysis-x:t_to_n-y:rt-p:"+str(p)+"-n:"+str(n)
	art = []
	lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
	art.append(lgd)
	plt.savefig("runtime/"+filename+".png", additional_artists=art, bbox_inches="tight")
	plt.close()


	plt.figure()
	plot_this( t_to_n_list, rt_list, "Quotient t/n", "Runtime rt", " n: "+str(n)+" p: "+str(p), "Batch Codes", "bo-" )
	plot_this( t_to_n_list, rt_list_bst, "Quotient t/n", "Runtime rt", " n: "+str(n)+" p: "+str(p), "Batch Codes(BST)", "ro-" )


	filename="Analysis-x:t_to_n-y:rt-p:"+str(p)+"-n:"+str(n)+"-comparison-to-bst"
	art = []
	lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
	art.append(lgd)
	plt.savefig("runtime/"+filename+".png", additional_artists=art, bbox_inches="tight")
	plt.close()

main()