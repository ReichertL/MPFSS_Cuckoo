testName=mpfss_batch_codes

CC=../../obliv-c/bin/oblivcc
RUNTIME=../../obliv-c/_build/libobliv.a

ACKLIB_FFSL=../../absentminded-crypto-kit/src/oram_fssl/
Mpfss_path=../mpfss/
Util_path=../util/

INCLUDE=-I$(ACKLIB_FFSL) -I$(Mpfss_path) -I$(Util_path) 
CFLAGS=-g -Wall -D _Float128=double -DDEBUG  -I. $(INCLUDE)


ACKLIB_PATH=../../absentminded-crypto-kit/build/lib/liback.a 
#-lgomp flag is very important. Otherwise linking will fail. Does some kind of parallelization.
LGFLAGS=-fopenmp $(ACKLIB_PATH) -lm


./a.out: $(testName).c $(RUNTIME) 
	$(CC) -o $(CFLAGS) $(testName).c $(testName).oc $(LIB) $(LGFLAGS) 




