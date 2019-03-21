#pragma once


extern "C" void print_tables();
extern "C" void *initialize(int w, int no_hash_tables, int *size_hash_tables, int *i_array, int max_loop);
extern "C" void cuckoo(int* keys, int no_keys);
extern "C" void cuckoo_hash_intervall(int lowerbound, int upperbound, int *extern_pointer);

//extern "C" void test123();