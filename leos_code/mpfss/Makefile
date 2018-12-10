testName=mpfss_naive

OBLIVCC=../../obliv-c/bin/oblivcc
RUNTIME=../../obliv-c/_build/libobliv.a


#CILPATH=../../obliv-c/
CFLAGS=-g -Wall #-DNDEBUG

ACKLIB_FFSL=../../absentminded-crypto-kit/src/oram_fssl/

INCLUDE=-I$(ACKLIB_FFSL)  


LOADFILES= util.c 

ACKLIB_PATH=../../absentminded-crypto-kit/build/lib/liback.a 
#-lgomp flag is very important. Otherwise linking will fail. Does some kind of parallelization.
LOADLIBES=-fopenmp $(ACKLIB_PATH) 


./a.out: $(testName).oc $(testName).c $(RUNTIME)
	$(OBLIVCC) $(CFLAGS) $(INCLUDE) $(testName).oc $(testName).c $(LOADFILES) $(LOADLIBES) 

#./a.out: $(testName).oc $(testName).c $(RUNTIME)
#	$(OBLIVCC) $(CFLAGS) $(INCLUDE) $(testName).oc $(testName).c $(IncludeFiles) $(LOADLIBES) 

clean:
	rm -f a.out $(testName).oc.cil.i $(testName).oc.i $(testName).oc.cil.c


