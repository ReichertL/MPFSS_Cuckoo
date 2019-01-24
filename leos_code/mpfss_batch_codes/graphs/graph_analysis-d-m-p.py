import math
from math import log
import matplotlib.pyplot as plt

t = [5, 50, 500, 5000]
size=[10000, 100000, 1000000]
epsilon= [0.001, 0.01, 0.1, 0.2, 0.3]  
#expansion factor
s=[1.1 , 1.2, 1.3, 1.4, 1.5, 2, 2.5, 3, 3.5 , 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5]

def plot_this(p, x_value_exact, y_values_exact, name_xaxis, name_yaxis, title_addition, lable_addition, lable, set_ticks ):

  p.plot(x_value_exact, y_values_exact, lable, label= lable_addition)
  if(p==plt):
  	p.xlabel(name_xaxis)
  p.ylabel(name_yaxis)

  p.margins(0.05, 0.1)
  p.title(name_xaxis+" vs. "+name_yaxis + "\n"+title_addition)


def main():

  d_exact=list()
  d_round=list()
  d_vals=list()

  # Extensionfactor s on Repetition d
  print("Extensionfactor s on Repetition d")
  plt.figure()
  for e in epsilon:
    d_exact_e=list()
    d_round_e=list()
    d_vals_e=list()
    for ss in s:
      val=(1+ss)*e+1;
      val2=round(val)
      vals=[val, val2, ss, e]
      d_vals_e.append(vals)
      d_exact_e.append(val)
      d_round_e.append(val2)
    print("Extensionfactor s  Repetition d  e:"+str(e))
    plot_this(plt, s, d_round_e, "Extensionfactor s", "Repetition d" , "", "e:"+str(e), 'x--',1)
    plot_this(plt, s, d_exact_e, "Extensionfactor s", "Repetition d" , "", "e:"+str(e), 'o-',0)

    d_exact.append(d_exact_e) 
    d_round.append(d_round_e) 
    d_vals.append(d_vals_e)

  
  filename="d/Graphanalysis-all"
  art = []
  lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
  art.append(lgd)
  plt.savefig(filename+".png", additional_artists=art, bbox_inches="tight")
  plt.close()


  #No. Points t on No. Batches m
  print("No. Points t on No. Batches m")
  m_exact=list()
  m_round=list()
  plt.figure()
  for e in epsilon:
    m_exact_e=list()
    m_round_e=list()
    for tt in t:
      val= pow(tt, 1+e)
      m_exact_e.append(val)
      val2=round(val)
      m_round_e.append(val2)

    plot_this(plt, t, m_round_e, "No. Points t", "No. Batches m (rounded)" , "e:"+str(e), "e:"+str(e), 'o-' ,1)
    m_exact.append(m_exact_e)
    m_round.append(m_round_e)

  filename="m/Graphanalysis-all"
  art = []
  lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
  art.append(lgd)
  plt.savefig(filename+".png", additional_artists=art, bbox_inches="tight")
  plt.close()

  #Repetition d on Probability of failure p
  print("Repetition d on Probability of failure p")
  p=list()
  p_real=list()
  plt_ticks=list()
  for tt in t:
    p_t=list()
    p_real_t=list() 
    rep=0

    plt.figure()
    for se in d_exact:
      p_t_e=list()
      p_real_t_e=list()
      for d in se:
        #val=1-pow(tt,-2*(d- 1));
        #val2=1- pow(tt,-2*(round(d)-1));
        val=1/pow(tt,d)
        val2=1/pow(tt, round(d))
        p_t_e.append(val)
        p_real_t_e.append(val2)
      p_t.append(p_t_e)
      p_real_t.append(p_real_t_e)
      #plt_ticks= plt_ticks  + p_real_t_e
      
      plot_this(plt, se, p_real_t_e, "Repetition d", "Probability of failure p" , "t: "+str(tt), "e:"+str(epsilon[rep]), 'x--', 1)
      plot_this(plt, se, p_t_e, "Repetition d", "Probability of failure p" , "t: "+str(tt), "e:"+str(epsilon[rep]), 'o-', 0)

      rep=rep+1
    #plt.yticks(plt_ticks)
    filename="p/Graphanalysis-x:d-y:p-t:"+str(tt)
    art = []
    lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
    art.append(lgd)

    plt.savefig(filename+".png", additional_artists=art, bbox_inches="tight")
    plt.close()

    p_real.append(p_real_t)
    p.append(p_t)
    


main()