#include <vector> 
#include <string>
#include <algorithm>
#include <iostream>

#include "includes/dbg.h"

//for match struct
#include "mpfss_cuckoo.h"
#include "includes/cuckoo/cuckoo.h"
#include "buckets.h"

using namespace std;

void print_vector(std::vector<int> v){

	for (std::vector<int>::const_iterator it = v.begin(); it != v.end(); ++it)
    std::cout << *it << ' ';
	printf("\n");
}

void print_vector_utf8(std::vector<uint8_t> v){

	for (std::vector<uint8_t>::const_iterator it = v.begin(); it != v.end(); ++it)
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

vector<vector<int>> generate_buckets_cuckoo(int size, int w, int b, int (*func)( int, int, int)){

	vector<vector<int>> all_buckets(b);

	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			//not hashing j but j+1, useful later
			int bucket_number=func(j+1,i,b);
			all_buckets.at(bucket_number).push_back(i);
		}
	
	}
	return all_buckets;
}


std::vector<std::vector<int>> preparations(mpfss_cuckoo *mpfss, int *indices_notobliv, int ocCurrentParty, match **matches, int *bucket_lenghts, int (*func)( int, int, int)){


	int size=mpfss->size;
	int w=mpfss->w;
	int b=mpfss->b;
	int max_loop=mpfss->max_loop;
	int t=mpfss->t;


	//--------------------Create Buckets----------------------------------------------------------------------
	vector<vector<int>> all_buckets=generate_buckets_cuckoo( size,  w,  b, func);

	for (int i = 0; i < b; ++i){
		vector<int> v=all_buckets.at(i);
		bucket_lenghts[i]= static_cast<int>(v.size());
	}
	
	debug_print_buckets(all_buckets,b);
	
	//--------------------Create Assignment----------------------------------------------------------------------
	if(ocCurrentParty==1){   
		//array of len 1 holding value b
		int siz_of_hashtable[1]={b};
		cuckoo_hashing *c=initialize( w, 1, siz_of_hashtable, NULL, max_loop, func);
	
		int *indices_notobliv_plus_1=(int *)calloc(t, sizeof(int));
		for (int i = 0; i < t; ++i){
			indices_notobliv_plus_1[i]=indices_notobliv[i]+1;
		}
		
		cuckoo(indices_notobliv_plus_1, t, c);
		print_tables(c);
		std::vector<int> table=c->tables.at(0);
		for (int i = 0; i < b; ++i){

			int val=table.at(i)-1; //unassigned slots will have value of -1
			match *p=(match *) calloc(1, sizeof(match)); 
			p->val=val;
			p->batch=i;
			if(val==-1){
				p->index_in_batch=-1;
				debug("unused batch: index = %d \n", p->index_in_batch );
			}else{
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
	return all_buckets;
}

