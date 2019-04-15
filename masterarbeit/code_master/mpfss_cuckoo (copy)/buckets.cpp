#include <vector> 
#include <string>
#include <algorithm>
#include <iostream>

#include "includes/dbg.h"
#include "includes/list_utils.h"

extern "C" {
#include "mpfss_naive_wrapper.h"
}

#include "cuckoo.h"
#include "mpfss_cuckoo.h"
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

vector<vector<int>> generate_batches_cuckoo(int size, int w, int b){

	vector<vector<int>> all_buckets(b); 

	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			//not hashing j but j+1, useful later
			int bucket_number=hashfuc_simple(j+1,i,b);
			all_buckets.at(bucket_number).push_back(i);
		}
	
	}
	return all_buckets;
}


void create_mpfss_vector_cuckoo(bool **dpf_vectors, vector<vector<int>> all_buckets, int size){
	
	std::vector<uint8_t> mpfss_vector(size);
	int b_count=0;

	for(auto bucket: all_buckets){

		int p_count=0;
		for (auto position:bucket){

			bool value=dpf_vectors[b_count][p_count];
			printf("b_count %d, p_count %d, position %d, value %d \n",b_count, p_count, position, value);

			mpfss_vector[position]=mpfss_vector[position]^value;
			p_count++;
		}
	  	b_count++;
	}
	//#ifdef DEBUG
		print_vector_utf8(mpfss_vector);
		uint8_t *vector=mpfss_vector.data();
		reveal_vector(vector, size, 1);		
	//#endif

}



void mpfss_cuckoo_in_cpp(mpfss_cuckoo *mpfss, int *indices_notobliv, int ocCurrentParty){


	int size=mpfss->size;
	int w=mpfss->w;
	int b=mpfss->b;
	int max_loop=mpfss->max_loop;
	int t=mpfss->t;

	pointinfo_t **matches = (pointinfo_t **) calloc(b, sizeof(pointinfo_t*));

	vector<vector<int>> all_buckets=generate_batches_cuckoo( size,  w,  b);
	int *bucket_lenghts=(int *) calloc(b, sizeof(int));
	for (int i = 0; i < b; ++i)
	{
		vector<int> v=all_buckets.at(i);
		bucket_lenghts[i]= static_cast<int>(v.size());
	}
	
	debug_print_buckets(all_buckets,b);
	
	if(ocCurrentParty==1){   
		//array of len 1 holding value b
		int siz_of_hashtable[1]={b};
		cuckoo_hashing *c=initialize( w, 1, siz_of_hashtable, NULL, max_loop);
	
		int *indices_notobliv_plus_1=(int *)calloc(t, sizeof(int));
		for (int i = 0; i < t; ++i){
			indices_notobliv_plus_1[i]=indices_notobliv[i]+1;
		}
		
		cuckoo(indices_notobliv_plus_1, t, c);
		print_tables(c);
		std::vector<int> table=c->tables.at(0);
		for (int i = 0; i < b; ++i){

			int val=table.at(i)-1; //unassigned slots will have value of -1
			pointinfo_t *p=(pointinfo_t *) calloc(1, sizeof(pointinfo_t)); 
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

	bool **dpf_vectors= (bool **)calloc(b, sizeof( bool *));
	call_dpf_from_matches(matches,dpf_vectors, b, bucket_lenghts);
	create_mpfss_vector_cuckoo(dpf_vectors, all_buckets, size);

}

