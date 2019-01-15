CC ?= gcc
CPP ?= cpp
AR ?= ar
OBLIVC_PATH=../../obliv-c
OBLIVCC = $(OBLIVC_PATH)/bin/oblivcc
OBLIVCH = $(OBLIVC_PATH)/src/ext/oblivc
OBLIVCA = $(OBLIVC_PATH)/_build/libobliv.a

ACKLIB_FFSL=../../absentminded-crypto-kit/src/oram_fssl/
CFLAGS+= -O3 -march=native -std=c99 -fopenmp 
CFLAGS+=-g -Wall -D _Float128=double -DDEBUG -D_POSIX_C_SOURCE=199309L 
CFLAGS+=-I$(SRC_PATH) -I$(ACKLIB_FFSL)		
ACKLIB_LIB=../../absentminded-crypto-kit/build/lib/liback.a 
LDFLAGS += -lm -lgomp -lgcrypt $(ACKLIB_LIB)

SRC_PATH=include
ACKLIB=a.out
#INCLUDE_DEPS= util.o binary_search_tree.o linked_list.o list_utils.o\
#			mpfss_naive.oo #mpfss_naive.oo
#MPB_DEPS = $(INCLUDE_DEPS:%=/%) 
#OBJS=$(MPB_DEPS)

SOURCES := $(shell find include/ -type f -name '*.c')
OBJS := $(patsubst %.c, %.o, $(SOURCES))
SOURCES2 := $(shell find include/ -type f -name '*.oc')
OBJS += $(patsubst %.oc, %.oo, $(SOURCES2))


DEPS:= $(include/*.h)
DEPS+= $(src/*.h)


TEST_PATH=src
TEST_OUT_PATH=.
TEST_DEPS=mpfss_batch_codes.o #mpfss_batch_codes.oo
TEST_BINS = mpfss_batch_codes

default: $(ACKLIB) tests clean

tests: $(TEST_BINS:%=$(TEST_OUT_PATH)/%)

$(TEST_BINS:%=$(TEST_OUT_PATH)/%): $(TEST_OUT_PATH)/%: $(TEST_PATH)/%.oo $(TEST_DEPS:%=$(TEST_PATH)/%) $(ACKLIB)
	mkdir -p $(TEST_OUT_PATH)
	$(OBLIVCC) -o $@ $(OBLIVCA) $^ $(LDFLAGS)

$(ACKLIB): $(OBJS:%=%)
	$(AR) rcs $@ $^

-include $($(patsubst %.oo,%.od,$(OBJS:.o=.d)):%=%) $(TEST_BINS:%=$(TEST_PATH)/%.od)

%.o: %.c $(DEPS)
	$(OBLIVCC) -c $(CFLAGS) $*.c -o $*.o $< $(ACKLIB_LIB) 
	$(CPP) -MM $(CFLAGS) $*.c -I $(OBLIVCH)  > $*.d

%.oo: %.oc $(DEPS)
	$(OBLIVCC) -c $(CFLAGS) $*.oc -o $*.oo -I $(OBLIVCH) $(ACKLIB_LIB)
	$(CPP) -MM $(CFLAGS) $*.oc -MT $*.oo > $*.od


clean:
	rm -f $(OBJS) $(shell find include/ -type f -name '*.d') $(shell find include/ -type f -name '*.od')
	rm -f $(OBJS) $(shell find src/ -type f -name '*.d') $(shell find src/ -type f -name '*.od')
	rm -f $(OBJS) $(shell find src/ -type f -name '*.oo') $(shell find src/ -type f -name '*.o')
	rm -f $(ACKLIB)
