#include <vector> 
#include <string>
#include <algorithm>
#include <iostream>

#include "includes/dbg.h"
#include "includes/benchmark.h"

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

vector<vector<int>> generate_buckets_cuckoo(int size, int w, int b, int (*func)( int, int), std::vector<int> rands){

	vector<vector<int>> all_buckets(b);

	for (int key = 0; key < size; ++key){

		for (int j = 0; j < w; ++j){
			int bucket_number=hash_this(func,key,rands.at(j), b);
			all_buckets.at(bucket_number).push_back(key);
		}
	}
	return all_buckets;
}

bool create_assignement(mpfss_cuckoo *mpfss, int *indices_notobliv, match **matches, int (*func)( int, int), vector<vector<int>> all_buckets , int *evictions_logging, std::vector<int> rands){
	
	int w=mpfss->w;
	int b=mpfss->b;
	int max_loop=mpfss->max_loop;
	int t=mpfss->t;

	if(mpfss->cp==1){   
		//array of len 1 holding value b
		int size_of_hashtable[1]={b};
		int no_hash_tables=1;
		cuckoo_hashing *c=initialize( w, no_hash_tables, size_of_hashtable, rands.data(), max_loop, func);
		
		int evictions=cuckoo(indices_notobliv, t, c);
		*evictions_logging=evictions;
		if(evictions==-1){
			if(mpfss->do_benchmark==1){
				std::vector<string> list_of_names={"runtime","t","size","no_buckets b", "no_hashfunctions w", "max_loop", "max_loop_reached", "evictions"};
				std::vector<string> list_of_values={"-1",to_string(t),to_string(mpfss->size),to_string(b),to_string(w),to_string(max_loop), "yes", "-1" };
                benchmark_list("cuckoo", 8, list_of_names, list_of_values);
			}
			free(c);
			return false;
		}
		
		#ifdef DEBUG
			print_tables(c);
		#endif

		std::vector<pair<int,int>> table=c->tables.at(0);
		std::vector<bool> this_usage=c->table_usage.at(0);
		for (int i = 0; i < b; ++i){

			int val=table.at(i).first;
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
	free(c);
	}
	return true;

}


std::vector<std::vector<int>> preparations(mpfss_cuckoo *mpfss,  int *bucket_lenghts, int (*func)( int, int), std::vector<int> rands){


	int size=mpfss->size;
	int w=mpfss->w;
	int b=mpfss->b;


	//--------------------Create Buckets----------------------------------------------------------------------
	log_info("Creating Buckets with size %d ,b %d and w %d\n", size,b, w);
	vector<vector<int>> all_buckets=generate_buckets_cuckoo( size,  w,  b, func, rands);

	for (int i = 0; i < b; ++i){
		vector<int> v=all_buckets.at(i);
		bucket_lenghts[i]= static_cast<int>(v.size());
	}
	
	#ifdef DEBUG
		debug_print_buckets(all_buckets,b);
	#endif

	return all_buckets;
}

