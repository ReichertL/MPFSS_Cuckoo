#pragma once


typedef struct 
{
	int no_hash_functions;
	int no_hash_tables;
	int *size_hash_tables;
	int max_loop;
	std::vector<std::vector<int>> tables; 
	std::vector<std::vector<bool>> table_usage; 
	int (* hash_function)( int, int); 
	std::vector<int> rands; 
}cuckoo_hashing;



int fast_mod(const int input, const int ceil);
int hashfunc_simple(int rand, int key);
int hashfunc_absl(int rand, int key);
int hash_this(int (*func)(int,int) , int key, int rand, int length_of_table);
std::vector<double> calculate_table_utilizastion(cuckoo_hashing *c);
void print_tables(cuckoo_hashing *c);
cuckoo_hashing *initialize(int w, int no_hash_tables, int *size_hash_tables, int *rands_array, int max_loop, int (*func)( int, int));
std::vector<int> create_rand_vector( int no);
int cuckoo(int* keys, int no_keys,  cuckoo_hashing *c);

//extern "C" void test223();