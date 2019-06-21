#ifndef fss_CPRG_H
#define fss_CPRG_H
#include "oram_fssl/floram.h"
#include "oram_fssl/fss.h"

typedef struct fss_cprg_offline fss_cprg_offline;

typedef void (* fss_cprg_traverser_fn)(void*, void *, void *, void*, void*);

void fss_cprg_offline_start(uint8_t * local_output, bool * local_bit_output, uint64_t * accumulator_L, uint64_t * accumulator_R, fss_cprg_offline * fsso);
void fss_cprg_offline_process_round(uint8_t * accumulator_L, uint8_t * accumulator_R, uint8_t * z, bool advicebit_l, bool advicebit_r, fss_cprg_offline * fsso);
void fss_cprg_offline_finalize(uint8_t * accumulator, uint8_t * z, bool advicebit_l, bool advicebit_r, fss_cprg_offline * fsso);

void fss_cprg_offline_parallelizer(void* fss, void* indexp, void* blockdelta, void * local_output, void * local_bit_output, void* pd, fss_cprg_traverser_fn fn, facb_fn cbfn, void* cbpass);

fss_cprg_offline * fss_cprg_offline_new(size_t size, size_t blockmultiple, uint8_t * keyL, uint8_t * keyR);
void fss_cprg_offline_free(fss_cprg_offline * fsso);

#endif