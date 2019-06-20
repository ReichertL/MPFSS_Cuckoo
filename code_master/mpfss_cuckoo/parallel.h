#pragma once

typedef void (* split_fn)( void *, int , void*);

void create_mpfss_vector_cuckoo(    bool *mpfss_bit_vector,uint8_t **mpfss_value_vector,bool **dpf_vectors, uint8_t ***dpf_value_vectors, int ** all_buckets_array, int *bucket_lenghts,  int size, int b, int blocksize);
void parallize(split_fn fn, void ** list_pd_split, int b, void * params);
