CC ?= gcc
CPP ?= cpp
AR ?= ar
OBLIVC_PATH=../../obliv-c
OBLIVCC = $(OBLIVC_PATH)/bin/oblivcc
OBLIVCH = $(OBLIVC_PATH)/src/ext/oblivc
OBLIVCA = $(OBLIVC_PATH)/_build/libobliv.a

ACKLIB_FFSL=../../absentminded-crypto-kit/src/oram_fssl/
CFLAGS+= -O3 -march=native -std=c99 -fopenmp 
CFLAGS+=-g -Wall -D_Float128=double  -D_POSIX_C_SOURCE=199309L -DTESTING
CFLAGS+=-I$(INCLUDE_Path) -I$(ACKLIB_FFSL) -I$(mpfss_PATH) -I$(mpfss_bc_PATH) 
ACKLIB=../../absentminded-crypto-kit/build/lib/liback.a 
LDFLAGS += -lm -lgomp -lgcrypt $(ACKLIB)

mpfss_PATH=../mpfss
mpfss_bc_PATH=../mpfss_batch_codes/src
INCLUDE_Path=../mpfss_batch_codes/include
SRC_PATH=.
#Util_Path=../util
LIB=a.out


SOURCES := $(shell find $(INCLUDE_Path)/ -type f -name '*.c')
SOURCES += $(shell find $(mpfss_PATH)/ -type f -name '*.c')
SOURCES += $(shell find $(mpfss_bc_PATH)/ -type f -name '*.c')
SOURCES += $(shell find ./ -type f -name '*.c')
OBJS := $(patsubst %.c, %.o, $(SOURCES))

SOURCES2 := $(shell find $(INCLUDE_Path)/ -type f -name '*.oc')
SOURCES2 += $(shell find $(mpfss_PATH)/ -type f -name '*.oc')
SOURCES2 += $(shell find $(mpfss_bc_PATH)/ -type f -name '*.oc')
OBJS += $(patsubst %.oc, %.oo, $(SOURCES2))




DEPS:= $(shell find $(INCLUDE_Path)/ -type f -name '*.h')
DEPS+= $(shell find $(SRC_PATH)/ -type f -name '*.h')
DEPS+= $(shell find $(mpfss_PATH)/ -type f -name '*.h')
DEPS+= $(shell find $(mpfss_PATH)/ -type f -name '*.oh')
DEPS+= $(shell find ./ -type f -name '*.h')


TEST_OUT_PATH=.
TEST_DEPS=test_main.o #mpfss_batch_codes_unittest.o mpfss_naive_unittest.o
TEST_BINS = test_main


default: tests clean

all: $(LIB) tests clean 


tests: $(TEST_BINS:%=$(TEST_OUT_PATH)/%)

$(TEST_BINS:%=$(TEST_OUT_PATH)/%): $(TEST_OUT_PATH)/%:  $(TEST_DEPS:%=$(SRC_PATH)/%) $(LIB)
	mkdir -p $(TEST_OUT_PATH)
	$(OBLIVCC) -o $@ $(OBLIVCA) $^ $(LDFLAGS)

$(LIB): $(OBJS:%=%)
	$(AR) rcs $@ $^

-include $($(patsubst %.oo,%.od,$(OBJS:.o=.d)):%=%) $(TEST_BINS:%=$(SRC_PATH)/%.od)


%.o: %.c $(DEPS)
	$(OBLIVCC) -c $(CFLAGS) $*.c -o $*.o $< $(ACKLIB)
	$(CPP) -MM $(CFLAGS) $*.c -I $(OBLIVCH)  > $*.d 

%.oo: %.oc $(DEPS)
	$(OBLIVCC) -c $(CFLAGS) $*.oc -o $*.oo $< -I $(OBLIVCH) $(ACKLIB) 
	$(CPP) -MM $(CFLAGS) $*.oc -MT $*.oo > $*.od 


clean:
	rm -f $(OBJS) $(shell find $(INCLUDE_Path)/ -type f -name '*.d') $(shell find $(INCLUDE_Path) -type f -name '*.od')
	rm -f $(OBJS) $(shell find $(mpfss_bc_PATH)/ -type f -name '*.d') $(shell find $(mpfss_bc_PATH)/ -type f -name '*.od')
	rm -f $(OBJS) $(shell find $(SRC_PATH)/ -type f -name '*.d') $(shell find $(SRC_PATH)/ -type f -name '*.od')
	rm -f $(OBJS) $(shell find $(mpfss_PATH)/ -type f -name '*.d') $(shell find $(mpfss_PATH)/ -type f -name '*.od')
	rm -f $(OBJS) $(shell find $(SRC_PATH)/ -type f -name '*.oo') $(shell find $(SRC_PATH)/ -type f -name '*.o')
	rm -f $(LIB)

