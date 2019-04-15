#pragma once


typedef struct 
{
	int no_hash_functions;
	int no_hash_tables;
	int *size_hash_tables;
	int max_loop;
	std::vector<std::vector<int>> tables; 
	int (* hash_function)( int, int, int); 
	std::vector<int> i; 
}cuckoo_hashing;




int hashfunc_simple(int i, int key, int length_of_table);
int hashfunc_absl(int i, int key, int length_of_table);

void print_tables(cuckoo_hashing *c);
cuckoo_hashing *initialize(int w, int no_hash_tables, int *size_hash_tables, int *i_array, int max_loop, int (*func)( int, int, int));
void cuckoo(int* keys, int no_keys,  cuckoo_hashing *c);

//extern "C" void test223();