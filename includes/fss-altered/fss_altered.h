#ifndef fss_H
#define fss_H
#include "floram.h"

typedef struct fss_offline fss_offline;

typedef void (* fss_traverser_fn)(void *, void *);
typedef void (* fss_pusher_fn)(void *, void *, void *);
typedef void (* facb_fn)(void *, void*);

void fss_offline_start(fss_offline * fsso, uint8_t * blocks);
void fss_offline_push_Z(fss_offline * fsso, uint8_t * Z, bool advicebit_l, bool advicebit_r, size_t level);
void fss_offline_readblockvector(uint8_t * local_output, bool* local_bit_output, fss_offline * fsso);

void fss_offline_parallelizer(void* fss, fss_offline * fsso, void* indexp, void * local_output, void* local_bit_output, void* pd, fss_traverser_fn fn, fss_pusher_fn fn2, facb_fn cbfn, void* cbpass);

fss_offline * fss_offline_new(size_t size, size_t blockmultiple, size_t startlevel, uint8_t * keyL, uint8_t * keyR);
void fss_offline_free(fss_offline * fsso);

#endif