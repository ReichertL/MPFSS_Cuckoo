

#include "obliv.h"
#include "create_structs.h"

/*  Creates management structure for MPFSS_cuckoo. Tested.
    Input 
    t: Number of points in mpfss
    size: Size of the field the points can be taken from 

*/



mpfss_cuckoo * new_mpfss_cuckoo( int t, size_t size, int w, int b, int max_loop, int cp, int do_benchmark){
  mpfss_cuckoo * mpfss =(mpfss_cuckoo * ) malloc(sizeof(mpfss_cuckoo));
  mpfss->t = t;
  mpfss->size = size;
  mpfss->w=w;
  mpfss->b=b;
  mpfss->max_loop=max_loop;
  mpfss->cp=cp; 
  mpfss->do_benchmark=do_benchmark;

  return mpfss;

}