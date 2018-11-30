OBLIVCC=../../obliv-c/bin/oblivcc
RUNTIME=../../obliv-c/_build/libobliv.a


CILPATH=../../obliv-c/
CFLAGS=-g -Wall
LOADLIBES=-lm
testName=mpfss_naive


./a.out: $(testName).oc $(testName).c ../common/util.c $(RUNTIME)
	$(OBLIVCC) $(CFLAGS) -I . $(testName).oc $(testName).c ../common/util.c $(LOADLIBES)

clean:
	rm -f a.out $(testName).oc.cil.i $(testName).oc.i $(testName).oc.cil.c


