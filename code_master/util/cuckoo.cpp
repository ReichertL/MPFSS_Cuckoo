#include <iostream>
#include <string>
#include<bits/stdc++.h> 
#include <vector> 
#include <stdio.h>

using namespace std;
using namespace std::placeholders;
using std::vector;


// upper bound on number of elements in our set 
//#define size_hash_table 11 
  
// choices for position 
//#define no_hash_tables 2 

int no_hash_functions;
int no_hash_tables;
int size_hash_table;
int max_iter;




struct hash_function {
  int no_arguments;
  vector<int> arguments;
  int (*func)(vector<int>, int);
};

vector<int *> tables; 

vector<hash_function *> hash_functions; 


int hashfuc1(int a, int b, int c){
	return a+b;

}

int hashfuc3(vector<int> params, int key){
	return (params.at(0)*key)%size_hash_table;

}

int hashfuc2(int a, int b){
	return a;
}
  
  
/* return hashed value for a key 
 * function: ID of hash function according to which 
    key has to hashed 
 * key: item to be hashed */
int hash_this(struct hash_function *h, int key) 
{ 
	int res=h->func(h->arguments, key);
	return res;    

} 


/* function to place a key in one of its possible positions 
 * tableID: table in which key has to be placed, also equal 
   to function according to which key must be hashed 
 * cnt: number of times function has already been called 
   in order to place the first input key 
 * (max_iter: maximum number of times function can be recursively 
   called before stopping and declaring presence of cycle) */
void place(int key, int tableID, int cnt) 
{ 

	hash_function *h=hash_functions.at(cnt);
	int pos=hash_this(h, key);
	//printf("key %d hashed to %d in Table %d\n", key, pos, tableID);
	int *t=tables.at(tableID);
	//warning value da köntne auch einfach 0 sein
	if (t[pos]==NULL){
		t[pos]=key;
	}else{

		int new_cnt=cnt+1;
		if(new_cnt==max_iter){
			printf("MAX_ITER reached, stopping. max_iter: %d\n", max_iter);
		}else{
			int new_key=t[pos];
			//printf("Key %d was evicted from %d in Table %d\n", new_key, pos, tableID);
			t[pos]=key;
			int new_tableID=(tableID+1)%no_hash_tables;
			place(new_key, new_tableID, new_cnt);
		}
	}
}


void print_tables(){

	for (int i = 0; i < no_hash_tables; ++i)
	{
		int *t=tables.at(i);
		for (int j = 0; j < size_hash_table; ++j)
		{
			printf("%d ", t[j]);
		}
		printf("\n");
	}

}

void initialize(int w, int no_t, int len_t, int max, int *params, int (*func_list[]) (vector<int>, int)  ){

	no_hash_functions=w;
	no_hash_tables=no_t;
	size_hash_table=len_t;
	max_iter=max;

	//initialize hash tables 
	for (int i = 0; i < no_hash_tables; ++i)
	{
		int *t=(int *) calloc(size_hash_table, sizeof(int));
		tables.push_back(t);
	}

	//initialize hash functions
	for (int i = 0; i < no_hash_functions; ++i)
	{
	
		vector<int> args; 
		args.push_back(params[i]);
		hash_function *f= (hash_function*) calloc (1,sizeof(hash_function));
		f->arguments=args;	
		int (*this_f)(vector<int>, int)=func_list[i];
		f->func=this_f;
		hash_functions.push_back(f);
	}

}

void cuckoo(int* keys, int no_keys){

	for (int i = 0; i < no_keys ; ++i)
	{
		place(keys[i], 0, 0);
	}

}

/* drino_hash_tables function */
int main() 
{ 	

	/*
		int n=size_hash_table;
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
  	*/


	int (*func_list[2]) (vector<int>, int)={hashfuc3,hashfuc3};
	int params[]={2, 6};
	initialize(2,2,10,3, params,func_list );

	//values to hash
	int keys [] = {16, 2, 77, 40, 12071};
	cuckoo(keys, 5);
	print_tables();

    return 0; 
} 