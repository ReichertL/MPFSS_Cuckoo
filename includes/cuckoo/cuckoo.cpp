#include <iostream>
#include <string>
#include <bits/stdc++.h> 
#include <vector> 
#include <stdio.h>
//#include <cstdlib>
#include <cmath>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

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

/* Cuckoo Algorithm : Code partially taken from https://www.geeksforgeeks.org/cuckoo-hashing/ and then rewritten.
*/



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
   called before stopping and declaring presence of cycle)

	Function returns:
		true - 		Key placed successfully
		false - 	Max Loop was reached
*/
int place(int key, int rand_no, int tableID, int cnt, cuckoo_hashing *c) { 		

	int (*func)( int, int)=c->hash_function;;
	int length_of_table=c->size_hash_tables[tableID];
	int rand=c->rands.at(rand_no);
	int pos=hash_this(func, key, rand, length_of_table);
	
	log_err("key %d hashed to %d in Table %d (length %d) using rand_no %d \n", key, pos, tableID, length_of_table, rand_no);

	//Use separate vector here to see if position is used. Otherwise there would be problems with zero value and unused space.
	if (!c->table_usage.at(tableID).at(pos)){
		c->tables.at(tableID).at(pos).first=key;
		c->tables.at(tableID).at(pos).second=rand_no;
		c->table_usage.at(tableID).at(pos)=true;
		printf("%d %d\n",c->tables.at(tableID).at(pos).first, c->tables.at(tableID).at(pos).second );
		return cnt;
	}else{

		int new_cnt=cnt+1;
		if(new_cnt==c->max_loop){
			log_err("MAX_ITER reached, stopping. max_loop: %d\n", c->max_loop);
			print_tables(c);
			vector<double> u= calculate_table_utilizastion(c);
			for (std::vector<double>::const_iterator it = u.begin(); it != u.end(); ++it)
	    		std::cout << *it << ' ';
			printf("\n");
			return -1;
		}else{
			int new_key=c->tables.at(tableID).at(pos).first;
			int new_rand_no=(c->tables.at(tableID).at(pos).second +1 )% c->no_hash_functions;
			log_err("Key %d was evicted from %d in Table %d\n", new_key, pos, tableID);
			c->tables.at(tableID).at(pos).first=key;
			c->tables.at(tableID).at(pos).second=rand_no;
			int new_tableID=(tableID+1)%c->no_hash_tables;
			return place(new_key, new_rand_no,new_tableID, new_cnt, c);
		}
	}
}


std::vector<double> calculate_table_utilizastion(cuckoo_hashing *c){
	std::vector<double> utilization;
	for (int i = 0; i < c->no_hash_tables; ++i){
		int this_len=c->size_hash_tables[i];
		std::vector<bool> v=c->table_usage.at(i);
		int count;
		for (int j = 0; j < this_len; ++j){
			if(v.at(j)==true){
				count++;
			}
		}
		utilization.push_back((double)count/(double)this_len);
	}
	return utilization;
}

void print_tables(cuckoo_hashing *c){

	for (int i = 0; i < c->no_hash_tables; ++i)
	{
		std::cout << "Table " << i<< ": ";
		std::vector<pair<int,int>> table= c->tables.at(i);
		for(auto &x:table){
  			cout<<x.first<<":"<<x.second<<", ";
		}
		printf("\n");
	}
}

cuckoo_hashing * initialize(int w, int no_hash_tables, int *size_hash_tables, int *rands_array, int max_loop, int (*func)( int, int) ){

	log_info("Initializing Cuckoo Hashing\n");
	
	cuckoo_hashing *c=(cuckoo_hashing *) calloc(1, sizeof(cuckoo_hashing));
	c->no_hash_functions=w;
	c->no_hash_tables=no_hash_tables;
	c->size_hash_tables= size_hash_tables;
	c->max_loop=max_loop;

	//initialize hash tables 
	vector<vector<pair<int,int>>> tables (no_hash_tables);
	std::vector<std::vector<bool>> table_usage (no_hash_tables); 
	for (int i = 0; i < no_hash_tables; ++i)
	{
		int size=size_hash_tables[i];
		std::vector<pair<int,int>> t(size);
		tables.at(i)=t;

		std::vector<bool> t_bool(size,false);
		table_usage.at(i)=t_bool;
	}
	c->tables=tables;
	c->table_usage=table_usage;

	c->hash_function= func;
	//initialize i values: Either number of function in function family or constant value used by the function
	if(rands_array){

		std::vector<int> rands(rands_array, rands_array+w );
		c->rands=rands;
	}else{
		c->rands=create_rand_vector(w);
		for (std::vector<int>::const_iterator it = c->rands.begin(); it != c->rands.end(); ++it)
	    	std::cout << *it << ' ';
		printf("\n");
	}

	return c;
}

std::vector<int> create_rand_vector( int no){
  
  srand (time(NULL));
  std::vector<int> res;
  int i=0;
  int sanity=0;
  int limit=100000;
  bool already_exists=false;
  while(i<no && sanity<limit){	
  	int r=rand();
    sanity++;
    for (int j = 0; j < i; ++j){
      if(res.at(j)==r){
        already_exists=true;
      }
    }
    if(!already_exists){
      res.push_back(r);
      i++;
      already_exists=false;
    }
  }
  if(sanity>=limit){
    debug("creation of %d distinct random numbers failed. creating vector with f(x)=x;",no );
    vector<int> linear (no);
	std::iota (std::begin(linear), std::end(linear), 0);
    return linear;
  }
  return res;
}

int cuckoo(int* keys, int no_keys, cuckoo_hashing *c){

	log_info("Running cuckoo with %d keys.\n",no_keys);

	int evictions=0;
	for (int i = 0; i < no_keys ; ++i)
	{
		int ev=place(keys[i], 0,0, 0, c);
		if(ev==-1){
			return -1;
		}
		evictions=evictions+ev;
	}
	return evictions;

}



