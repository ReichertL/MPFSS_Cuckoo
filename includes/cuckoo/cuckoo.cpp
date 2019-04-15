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
	Probably slow because of % 
	((a*x+b) %p ) % m
	a=10; b=i; p=296741 
	Warning: p>=m, so only use for tables smaller than 296741 (or change prime number). 
*/
int hashfunc_simple(int i, int key, int length_of_table){

	return ((i+10*key)% 296741)%length_of_table;

}

int hashfunc_absl(int i, int key, int length_of_table){
	auto hash_pair = std::make_pair(i, key);
	int val=absl::Hash<pair<int, int>>{}(hash_pair);
	return abs(val) % length_of_table; // TODO: Das ist nicht gut/langsam!
}

  
/* return hashed value for a key 
 * function: ID of hash function according to which 
    key has to hashed 
 * key: item to be hashed */
int hash_this(int (*func)(int,int,int) , int key, int i, int length_of_table) 
{ 

	int res=func(i, key, length_of_table);
	return res;    

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


	int (*func)( int, int, int)=c->hash_function;;
	int length_of_table=c->size_hash_tables[tableID];
	int i=c->i.at(cnt % c->no_hash_functions);
	int pos=hash_this(func, i, key, length_of_table);
	
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

cuckoo_hashing * initialize(int w, int no_hash_tables, int *size_hash_tables, int *i_array, int max_loop, int (*func)( int, int, int) ){

	cuckoo_hashing *c=(cuckoo_hashing *) calloc(1, sizeof(cuckoo_hashing));
	c->no_hash_functions=w;
	c->no_hash_tables=no_hash_tables;
	c->size_hash_tables= size_hash_tables;
	c->max_loop=max_loop;

	//initialize hash tables 
	vector<vector<int>> tables;
	std::vector<std::vector<bool>> table_usage; 
	for (int i = 0; i < no_hash_tables; ++i)
	{
		std::vector<int> t(size_hash_tables[i]);
		tables.push_back(t);

		std::vector<bool> t_bool(size_hash_tables[i]);
		table_usage.push_back(t_bool);
	}
	c->tables=tables;
	c->table_usage=table_usage;

	c->hash_function= func;
	//initialize i values: Either number of function in function family or constant value used by the function
	if(i_array){
		std::vector<int> i(i_array, i_array + sizeof i_array / sizeof i_array[0]);
		c->i=i;
	}else{
		vector<int> i;
		for (int j = 0; j < w ; ++j){
			i.push_back(j);
		}
		c->i=i;
	}

	return c;
}

void cuckoo(int* keys, int no_keys, cuckoo_hashing *c){

	for (int i = 0; i < no_keys ; ++i)
	{
		place(keys[i], 0, 0, c);
	}

}



