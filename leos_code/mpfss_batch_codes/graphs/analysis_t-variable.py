from math import log
import matplotlib.pyplot as plt
from decimal import Decimal


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
	n_list=[1000000.]
	for n in n_list:
		t_list = [ 100., 200., 400., 800., 1600.,5000.]
		s_list=list()
		d_list=list()
		e_list=list()
		m_list=list()
		rt_list=list()
		rt_list_bst=list()
		rt_list_ll=list()
		rt_list_naive=[2*n*t for t in t_list]
		t_to_n_list=[t/n for t in t_list]

		for t in t_list:
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

			#print("s:"+str(s)+" d:"+str(d)+" e:"+str(e)+" m:"+str(m)+" rt:"+str(rt))
			print(" m:"+str(m/ pow(10,6)))
#			print(" rt:"+str(rt / pow(10, 7)))
			rt_bst= t*n*d/m+n*d+t*m/2*((n*d)/m)
			rt_list_bst.append(rt_bst)

			rt_ll= t*n*d/m+n*d+t*n*m/2
			rt_list_ll.append(rt_ll)


		plt.figure()
		plot_this( t_to_n_list, e_list, "Quotient t/n", "Epsilon e ", " n: "+str(n)+" p: 1-"+str(p_inverse), "", "o-" )
		filename="Analysis-x:t_to_n-y:e-p:1-"+str(p_inverse)+"-n:"+str(n)
		art = []
		lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
		art.append(lgd)
		plt.savefig("e/"+filename+".png", additional_artists=art, bbox_inches="tight")
		plt.close()

		plt.figure()
		plot_this( t_list, e_list, "Number Points of MPF t ", "Epsilon e ", " n: "+str(n)+" p: 1-"+str(p_inverse), "", "o-" )
		filename="Analysis-x:t-y:e-p:1-"+str(p_inverse)+"-n:"+str(n)
		art = []
		lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
		art.append(lgd)
		plt.savefig("e/"+filename+".png", additional_artists=art, bbox_inches="tight")
		plt.close()

		plt.figure()
		plot_this( t_to_n_list, d_list, "Quotient t/n", "Repetition Factor d ", " n: "+str(n)+" p: 1-"+str(p_inverse), "", "o-" )
		filename="Analysis-x:t_to_n-y:d-p:1-"+str(p_inverse)+"-n:"+str(n)
		art = []
		lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
		art.append(lgd)
		plt.savefig("d/"+filename+".png", additional_artists=art, bbox_inches="tight")
		plt.close()


		plt.figure()
		plot_this( t_list, d_list, "Number Points of MPF t", "Repetition Factor d ", " n: "+str(n)+" p: 1-"+str(p_inverse), "", "o-" )
		filename="Analysis-x:t-y:d-p:1-"+str(p_inverse)+"-n:"+str(n)
		art = []
		lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
		art.append(lgd)
		plt.savefig("d/"+filename+".png", additional_artists=art, bbox_inches="tight")
		plt.close()

		plt.figure()
		plot_this( t_list, m_list, "Number Points of MPF t", "Number of Batches m ", " n: "+str(n)+" p: 1-"+str(p_inverse), "", "o-" )
		filename="Analysis-x:t-y:m-p:1-"+str(p_inverse)+"-n:"+str(n)
		art = []
		lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
		art.append(lgd)
		plt.savefig("m/"+filename+".png", additional_artists=art, bbox_inches="tight")
		plt.close()

		plt.figure()
		plot_this( t_to_n_list, rt_list, "Quotient t/n", "Runtime rt ", " n: "+str(n)+" p: 1-"+str(p_inverse), "Batch Codes", "o-" )
		plot_this( t_to_n_list, rt_list_naive, "Quotient t/n", "Runtime rt ", " n: "+str(n)+" p: 1-"+str(p_inverse), "Naive", "x--" )
		filename="Analysis-x:t_to_n-y:rt-p:1-"+str(p_inverse)+"-n:"+str(n)
		art = []
		lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
		art.append(lgd)
		plt.savefig("runtime/t_variable/"+filename+".png", additional_artists=art, bbox_inches="tight")
		plt.close()


		plt.figure()
		plot_this( t_list, rt_list, "Number Points of MPF t ", "Runtime rt ", " n: "+str(n)+" p: 1-"+str(p_inverse), "Batch Codes", "o-" )
		plot_this( t_list, rt_list_naive, "Number Points of MPF t ", "Runtime rt ", " n: "+str(n)+" p: 1-"+str(p_inverse), "Naive", "x--" )
		filename="Analysis-x:t-y:rt-p:1-"+str(p_inverse)+"-n:"+str(n)
		art = []
		lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
		art.append(lgd)
		plt.savefig("runtime/t_variable/"+filename+".png", additional_artists=art, bbox_inches="tight")
		plt.close()

		#plt.figure()
		#plot_this( t_to_n_list, rt_list, "Quotient t/n", "Runtime rt", " n: "+str(n)+" p: 1-"+str(p_inverse), "Batch Codes", "bo-" )
		#plot_this( t_to_n_list, rt_list_bst, "Quotient t/n", "Runtime rt", " n: "+str(n)+" p: 1-"+str(p_inverse), "Batch Codes(BST)", "ro-" )
		#filename="Analysis-x:t_to_n-y:rt-p:1-"+str(p_inverse)+"-n:"+str(n)+"-comparison-to-bst"
		#art = []
		#lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
		#art.append(lgd)
		#plt.savefig("runtime/t_variable/"+filename+".png", additional_artists=art, bbox_inches="tight")
		#plt.close()

		plt.figure()
		plot_this( t_to_n_list, rt_list, "Quotient t/n", "Runtime rt", " n: "+str(n)+" p: 1-"+str(p_inverse), "Batch Codes", "bo-" )
		filename="Analysis-x:t_to_n-y:rt-p:1-"+str(p_inverse)+"-n:"+str(n)+"-single"
		art = []
		lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
		art.append(lgd)
		plt.savefig("runtime/t_variable/"+filename+".png", additional_artists=art, bbox_inches="tight")
		plt.close()


		plt.figure()
		plot_this( t_list, rt_list, "Number Points of MPF t ", "Runtime rt", " n: "+str(n)+" p: 1-"+str(p_inverse), "Batch Codes", "bo-" )
		filename="Analysis-x:t-y:rt-p:1-"+str(p_inverse)+"-n:"+str(n)+"-single"
		art = []
		lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
		art.append(lgd)
		plt.savefig("runtime/t_variable/"+filename+".png", additional_artists=art, bbox_inches="tight")
		plt.close()

main()