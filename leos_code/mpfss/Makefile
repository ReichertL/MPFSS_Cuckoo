OBLIVCC=../../obliv-c/bin/oblivcc
RUNTIME=../../obliv-c/_build/libobliv.a


CILPATH=../../obliv-c/
CFLAGS=-g -Wall
LOADLIBES=-lm
testName=mpfss_naive
ACKLIB_PATH=../../absentminded-crypto-kit/build/lib/liback.a
ACKLIB_FFSL=../../absentminded-crypto-kit/src/oram_fssl
ACKLIB_SRC=../../absentminded-crypto-kit/src/
INCLUDE=-I . -I$(ACKLIB_SRC) -I$(ACKLIB_FFSL)
./a.out: $(testName).oc $(testName).c $(RUNTIME)
	$(OBLIVCC) $(CFLAGS) $(INCLUDE) $(testName).oc $(testName).c $(LOADLIBES) -L$(ACKLIB_PATH)

clean:
	rm -f a.out $(testName).oc.cil.i $(testName).oc.i $(testName).oc.cil.c


