#pragma once

void print_vector(std::vector<int> v);
void debug_print_buckets(std::vector<std::vector<int> > all_buckets , int b );
std::vector<std::vector<int>> generate_buckets_cuckoo(int size, int w, int b, int (*func)( int, int));
std::vector<std::vector<int>> preparations(	mpfss_cuckoo *mpfss, 
											int *indices_notobliv, 
											match **matches, 
											int *bucket_lenghts,
											int (*func)( int, int));

