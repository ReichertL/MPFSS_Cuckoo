import math
import matplotlib.pyplot as plt

t = [5, 50, 500]
size=[10000, 100000, 1000000]
epsilon= [0.001, 0.01, 0.1, 0.2, 0.3]
  #expansion factor
s=[1.1 , 1.2, 1.3, 1.4, 1.5, 2, 2.5, 3, 3.5 , 4]

def plot_this(x_value_exact, y_values_exact, name_xaxis, name_yaxis, title_addition, lable_addition, lable ):

  p=plt.plot(x_value_exact, y_values_exact, lable, label= lable_addition)
  plt.xlabel(name_xaxis)
  plt.ylabel(name_yaxis)
  #plt.xticks(x_value_exact)
  plt.margins(0.05, 0.1)
  plt.title(name_xaxis+" vs. "+name_yaxis + "\n"+title_addition)
  return p


def main():

  d_exact=list()
  d_round=list()
  d_vals=list()

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
    
    plot_this(s, d_exact_e, "Extenstionfactor s", "Repetiotion d" , "e:"+str(e), "e:"+str(e), 'o-')
    plot_this(s, d_round_e, "Extenstionfactor s", "Repetiotion d" , "e:"+str(e), "e:"+str(e), 'x--')

    d_exact.append(d_exact_e) 
    d_round.append(d_round_e) 
    d_vals.append(d_vals_e)
  
  filename="d/Graphanalysis-all"
  art = []
  lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
  art.append(lgd)
  plt.savefig("Graphanalysis/"+filename+".png", additional_artists=art, bbox_inches="tight")
  plt.close()



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

    plot_this(t, m_exact_e, "No. Points t", "No. Batches m" , "e:"+str(e), "e:"+str(e), 'o-')
    m_exact.append(m_exact_e)
    m_round.append(m_round_e)

  filename="m/Graphanalysis-all"
  art = []
  lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
  art.append(lgd)
  plt.savefig("Graphanalysis/"+filename+".png", additional_artists=art, bbox_inches="tight")
  plt.close()


  p=list()
  p_real=list()
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
      
      
      plot_this(se, p_t_e, "Repetiotion d", "Probability of failure p" , "t: "+str(tt), "e:"+str(epsilon[rep]), 'o-')
      plot_this(se, p_real_t_e, "Repetiotion d", "Probability of failure p" , "t: "+str(tt), "e:"+str(epsilon[rep]), 'x--')

      rep=rep+1
    filename="p/Graphanalysis-x:d-y:p-t:"+str(tt)
    art = []
    lgd = plt.legend(loc=9, bbox_to_anchor=(0.5, -0.1), ncol=2)
    art.append(lgd)
    plt.savefig("Graphanalysis/"+filename+".png", additional_artists=art, bbox_inches="tight")
    plt.close()

    p_real.append(p_real_t)
    p.append(p_t)
    

main()