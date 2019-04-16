#include <iostream>
#include <string>
#include <bits/stdc++.h> 
#include <vector> 
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include "absl/hash/hash.h"

#include "includes/dbg.h"

/*#include "includes/cryptopp-master/sha.h"
#include "includes/cryptopp-master/filters.h"
#include "includes/cryptopp-master/base64.h"
*/

#include "cuckoo.h"

using namespace std;
using namespace std::placeholders;
using std::vector;

/* 	
	Apparently faster than %
	Cite:  Chandler Carruth's benchmarks at CppCon 2015, and 
	https://stackoverflow.com/questions/33333363/built-in-mod-vs-custom-mod-function-improve-the-performance-of-modulus-op
*/
int fast_mod(const int input, const int ceil) {
    // apply the modulo operator only when needed
    // (i.e. when the input is greater than the ceiling)
    return input >= ceil ? input % ceil : input;
    // NB: the assumption here is that the numbers are positive
}


/*
	Probably slow because of % 
	((a*x+b) %p ) % m
	a=10; b=i; p=296741 
	Warning: p>=m, so only use for tables smaller than 296741 (or change prime number). 
*/
int hashfunc_simple(int rand, int key){

	return ((rand+10*key)% 296741);

}


int hashfunc_absl(int rand, int key){
	auto hash_pair = std::make_pair(rand, key);
	int val=absl::Hash<pair<int, int>>{}(hash_pair);
	return  val;
}

  
/* return hashed value for a key 
 * function: ID of hash function according to which 
    key has to hashed 
 * key: item to be hashed */
int hash_this(int (*func)(int,int) , int key, int rand, int length_of_table) 
{ 

	int res=abs(func( rand, key));
	return fast_mod(res, length_of_table);    

} 


/* function to place a key in one of its possible positions 
 * tableID: table in which key has to be placed, also equal 
   to function according to which key must be hashed 
 * cnt: number of times function has already been called 
   in order to place the first input key 
 * (max_loop: maximum number of times function can be recursively 
   called before stopping and declaring presence of cycle) */
void place(int key, int tableID, int cnt, cuckoo_hashing *c) 
{ 		


	int (*func)( int, int)=c->hash_function;;
	int length_of_table=c->size_hash_tables[tableID];
	int rand=c->rands.at(cnt % c->no_hash_functions);
	int pos=hash_this(func, rand, key, length_of_table);
	
	debug("key %d hashed to %d in Table %d\n", key, pos, tableID);
	

	std::vector<int> t=c->tables.at(tableID);

	//Use separate vector here to see if position is used. Otherwise there would be problems with zero value and unused space.
	if (!c->table_usage.at(tableID).at(pos)){
		c->tables.at(tableID).at(pos)=key;
		c->table_usage.at(tableID).at(pos)=true;

	}else{

		int new_cnt=cnt+1;
		if(new_cnt==c->max_loop){
			printf("MAX_ITER reached, stopping. max_loop: %d\n", c->max_loop);
		}else{
			int new_key=c->tables.at(tableID).at(pos);
			debug("Key %d was evicted from %d in Table %d\n", new_key, pos, tableID);
			c->tables.at(tableID).at(pos)=key;
			int new_tableID=(tableID+1)%c->no_hash_tables;
			place(new_key, new_tableID, new_cnt, c);

		}
	}
}


void print_tables(cuckoo_hashing *c){

	for (int i = 0; i < c->no_hash_tables; ++i)
	{
		std::cout << "Table " << i<< ": ";
		std::vector<int> table= c->tables.at(i);
		for (std::vector<int>::const_iterator it = table.begin(); it != table.end(); ++it)
    	std::cout << *it << ' ';
		printf("\n");
	}
}

cuckoo_hashing * initialize(int w, int no_hash_tables, int *size_hash_tables, int *rands_array, int max_loop, int (*func)( int, int) ){

	cuckoo_hashing *c=(cuckoo_hashing *) calloc(1, sizeof(cuckoo_hashing));
	c->no_hash_functions=w;
	c->no_hash_tables=no_hash_tables;
	c->size_hash_tables= size_hash_tables;
	c->max_loop=max_loop;

	//initialize hash tables 
	vector<vector<int>> tables (no_hash_tables);
	std::vector<std::vector<bool>> table_usage (no_hash_tables); 
	for (int i = 0; i < no_hash_tables; ++i)
	{
		int size=size_hash_tables[i];
		std::vector<int> t(size);
		tables.push_back(t);

		std::vector<bool> t_bool(size);
		table_usage.push_back(t_bool);
	}
	c->tables=tables;
	c->table_usage=table_usage;

	c->hash_function= func;
	//initialize i values: Either number of function in function family or constant value used by the function
	if(rands_array){
		std::vector<int> rands(rands_array, rands_array + sizeof rands_array / sizeof rands_array[0]);
		c->rands=rands;
	}else{
		vector<int> rands (w);
		for (int j = 0; j < w ; ++j){
			rands.push_back(j);
		}
		c->rands=rands;
	}

	return c;
}

void cuckoo(int* keys, int no_keys, cuckoo_hashing *c){

	for (int i = 0; i < no_keys ; ++i)
	{
		place(keys[i], 0, 0, c);
	}

}



