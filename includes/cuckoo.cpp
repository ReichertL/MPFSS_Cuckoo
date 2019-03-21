#include <iostream>
#include <string>
#include<bits/stdc++.h> 
#include <vector> 
#include <stdio.h>
#include <cstdlib>

#include "dbg.h"
//#include "absl/hash/hash.h"

#include "cuckoo.h"

using namespace std;
using namespace std::placeholders;
using std::vector;




typedef struct 
{
	int no_hash_functions;
	int no_hash_tables;
	int *size_hash_tables;
	int max_loop;
	vector<int *> tables; 
	int (* hash_function)( int, int, int); 
	vector<int> i; 


}cuckoo_hashing;


int hashfucSHA256(int i, int key, int length_of_table){
	return 0;
}

int hashfuc_simple(int i, int key, int length_of_table){
	return (i*key)%length_of_table;

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
	
	int *t=c->tables.at(tableID);

	//warning value da köntne auch einfach 0 sein
	if (!t[pos]){
		t[pos]=key;
	}else{

		int new_cnt=cnt+1;
		if(new_cnt==c->max_loop){
			printf("MAX_ITER reached, stopping. max_loop: %d\n", c->max_loop);
		}else{
			int new_key=t[pos];
			debug("Key %d was evicted from %d in Table %d\n", new_key, pos, tableID);
			t[pos]=key;
			int new_tableID=(tableID+1)%c->no_hash_tables;
			place(new_key, new_tableID, new_cnt, c);
		}
	}
}


void print_tables(int * extern_pointer){
	cuckoo_hashing *c=(cuckoo_hashing *) extern_pointer;

	for (int i = 0; i < c->no_hash_tables; ++i)
	{
		int *t=c->tables.at(i);
		int size_table=c->size_hash_tables[i];
		for (int j = 0; j < size_table ; ++j)
		{
			printf("%d ", t[j]);
		}
		printf("\n");
	}

}

//actually returns a cuckoo_hashing *, made int* so function can be called from c code. Casting is essential
void * initialize(int w, int no_hash_tables, int *size_hash_tables, int *i_array, int max_loop){

	cuckoo_hashing *c=(cuckoo_hashing *) calloc(1, sizeof(cuckoo_hashing));
	c->no_hash_functions=w;
	c->no_hash_tables=no_hash_tables;
	c->size_hash_tables= size_hash_tables;
	c->max_loop=max_loop;

	//initialize hash tables 
	vector<int *> tables;
	for (int i = 0; i < no_hash_tables; ++i)
	{
		int *t=(int *) calloc(size_hash_tables[i], sizeof(int));
		tables.push_back(t);
	}
	c->tables=tables;

	c->hash_function= hashfucSHA256;

	//initialize i values: Either number of function in function family or constant value used by the function
	if(i_array){

	}else{
		vector<int> i;
		for (int j = 0; j < w ; ++j)
		{
				//TODO keine doppelten
			int r=rand();
			i.push_back(r);

		}
		c->i=i;
	}

	return (void *)c;
}

void cuckoo(int* keys, int no_keys, int *extern_pointer){
	cuckoo_hashing *c=(cuckoo_hashing *) extern_pointer;

	for (int i = 0; i < no_keys ; ++i)
	{
		place(keys[i], 0, 0, c);
	}

}


void cuckoo_hash_intervall(int lowerbound, int upperbound, int *extern_pointer){
	cuckoo_hashing *c=(cuckoo_hashing *) extern_pointer;

	for (int i = lowerbound; i < upperbound ; ++i)
	{
		place(i, 0, 0, c);
	}

}



/* drino_hash_tables function */
/*int main() 
{ 	

	
		int n=size_hash_tables;
		auto h1 = std::bind(hashfuc1, _1 ,_2, n);
		auto h2 = std::bind(hashfuc2, _1 , n);
		auto h3 = std::bind(hashfuc3);

		int r1=h1(2,5);
		cout<<r1<<"\n";
		int r2=h2(2 );
		cout<<r2<<"\n";
		int r3=h3();
	  	cout<<r3<<"\n";

	  	int (*h3_ptr)()=hashfuc3;
		int (*func_list[2]) (vector<int>, int)={hashfuc3,hashfuc3};
		int params[]={2, 6};
		initialize(2,2,10,3, params,func_list );

		//values to hash
		int keys [] = {16, 2, 77, 40, 12071};
		cuckoo(keys, 5);
		print_tables();

	    return 0; 
} 

*/