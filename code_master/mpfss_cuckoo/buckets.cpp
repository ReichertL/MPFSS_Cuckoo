#include <vector> 
#include <string>
#include <algorithm>
#include <iostream>

#include "includes/dbg.h"

//for match struct
extern "C"{
	#include "create_structs.h"
}
#include "includes/cuckoo/cuckoo.h"
#include "buckets.h"

using namespace std;

void print_vector(std::vector<int> v){

	for (std::vector<int>::const_iterator it = v.begin(); it != v.end(); ++it)
    std::cout << *it << ' ';
	printf("\n");
}


void debug_print_buckets(vector<vector<int> > all_buckets , int b ){
	int count=0;
	for(auto i: all_buckets){
		string out="Bucket ";
		out+= to_string(count);
		out+=": ";
		for (auto j:i)
		{
			out.append(to_string(j));
			out+=" ";
		}
	  	out+="\n";
	  	debug("%s", out.c_str());
	  	count++;
	}
}

vector<vector<int>> generate_buckets_cuckoo(int size, int w, int b, int (*func)( int, int)){

	vector<vector<int>> all_buckets(b);

	for (int key = 0; key < size; ++key){

		for (int rand = 0; rand < w; ++rand){
			int bucket_number=hash_this(func,key,rand, b);
			all_buckets.at(bucket_number).push_back(key);
		}
	}
	return all_buckets;
}

void create_assignement(mpfss_cuckoo *mpfss, int *indices_notobliv, match **matches, int (*func)( int, int), vector<vector<int>> all_buckets){
	
	int w=mpfss->w;
	int b=mpfss->b;
	int max_loop=mpfss->max_loop;
	int t=mpfss->t;

	if(mpfss->cp==1){   
		//array of len 1 holding value b
		int size_of_hashtable[1]={b};
		int no_hash_tables=1;
		cuckoo_hashing *c=initialize( w, no_hash_tables, size_of_hashtable, NULL, max_loop, func);
		
		cuckoo(indices_notobliv, t, c);
		print_tables(c);
		std::vector<int> table=c->tables.at(0);
		std::vector<bool> this_usage=c->table_usage.at(0);
		for (int i = 0; i < b; ++i){

			int val=table.at(i);
			bool bucket_used=this_usage.at(i); //Allows checking if bucket was used
			match *p=(match *) calloc(1, sizeof(match)); 
			p->batch=i;
			if(!bucket_used){
				p->val=-1;
				p->index_in_batch=-1;
				debug("unused batch: index = %d \n", p->index_in_batch );
			}else{
				p->val=val;
				//Search takes O(log(n))
				std::vector<int> v =all_buckets.at(i);
 	    		std::vector<int>::iterator upper1, upper2; 
			    upper1 = std::lower_bound(v.begin(), v.end(), val );
			    p->index_in_batch= upper1 - v.begin();
      			debug("key %d -> index %d\n",val, p->index_in_batch );
			}
			matches[i]=p;
		}
	}

}


std::vector<std::vector<int>> preparations(mpfss_cuckoo *mpfss, int *indices_notobliv, match **matches, int *bucket_lenghts, int (*func)( int, int)){


	int size=mpfss->size;
	int w=mpfss->w;
	int b=mpfss->b;


	//--------------------Create Buckets----------------------------------------------------------------------
	vector<vector<int>> all_buckets=generate_buckets_cuckoo( size,  w,  b, func);

	for (int i = 0; i < b; ++i){
		vector<int> v=all_buckets.at(i);
		bucket_lenghts[i]= static_cast<int>(v.size());
	}
	
	debug_print_buckets(all_buckets,b);
	
	//--------------------Create Assignment----------------------------------------------------------------------
	create_assignement(mpfss, indices_notobliv, matches, func, all_buckets);
	return all_buckets;
}

