testName=mpfss_batch_codes

OBLIVCC=../../obliv-c/bin/oblivcc
RUNTIME=../../obliv-c/_build/libobliv.a


#CILPATH=../../obliv-c/
#DNDEBUG sets the debug level. 
CFLAGS=-g -Wall -D _Float128=double -DDEBUG

ACKLIB_FFSL=../../absentminded-crypto-kit/src/oram_fssl/
Mpfss_path=../mpfss/
Util_path=../util/
bcrandom_paht=../../obliv-c/src/ext/oblivc/
INCLUDE=-I$(ACKLIB_FFSL) -I$(Mpfss_path) -I$(Util_path) -I$(bcrandom_paht)


LOADFILES= ../util/util.c 

ACKLIB_PATH=../../absentminded-crypto-kit/build/lib/liback.a 
#-lgomp flag is very important. Otherwise linking will fail. Does some kind of parallelization.
LOADLIBES=-fopenmp $(ACKLIB_PATH) -lm


./a.out: $(testName).c $(RUNTIME)
	$(OBLIVCC) $(CFLAGS) $(INCLUDE) $(testName).c $(LOADFILES) $(LOADLIBES) 

#./a.out: $(testName).oc $(testName).c $(RUNTIME)
#	$(OBLIVCC) $(CFLAGS) $(INCLUDE) $(testName).oc $(testName).c $(IncludeFiles) $(LOADLIBES) 

clean:
	rm -f a.out 

