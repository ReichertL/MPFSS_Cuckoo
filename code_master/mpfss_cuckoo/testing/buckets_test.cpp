#include <string>
#include <cmath>
#include <stdlib.h>     /* srand, rand */
#include <time.h>  

#include "gtest/gtest.h"
#include "includes/cuckoo/cuckoo.h"


extern "C" {
    #include "includes/util.h"
    #include "../create_structs.h"

}
#include "../buckets.h"

TEST(generate_buckets_cuckoo, Works){		
	srand (time(NULL));
	int size = rand()%1000000;
	int w=rand()%1000;
	int t=0.01*size;
	int b=1.5*t;
	int (*func)( int, int)= hashfunc_absl; 
	std::vector<std::vector<int>>  result = generate_buckets_cuckoo( size,  w,  b, func);
	EXPECT_EQ(result.size(),b);

	int test_val=rand()%size;
	for (int i = 0; i < w; ++i)
	{
		int bucket_number=hash_this(func,test_val,i, b);
		std::vector<int> this_bucket=result.at(bucket_number);
		bool contained=std::find(this_bucket.begin(), this_bucket.end(), test_val) != this_bucket.end();
		EXPECT_TRUE(contained)<<"Test value "<<test_val<<" was not contained in  bucket "<<bucket_number<<" (w="<<i<<").";

	}
	/*if(::testing::Test::HasFatalFailure() and b<=10 and size<=100){
		debug_print_buckets(result,b);
	}*/
}

TEST(preparations, cp1){
	int size =rand()%1000000;
	int w=rand()%100;
	int t=0.01*size;
	int b=1.5*t;
	int max_loop=size;
	int cp=1;

	mpfss_cuckoo *m=new_mpfss_cuckoo(t, (size_t)size, w, b, max_loop,cp);
	int (*func)( int, int)= hashfunc_absl; 
	int *indices_notobliv = (int *)calloc(t, sizeof(int ));

	int count=0;
	bool created=false;
    while(!created and count<t*size*100000){ 
        created=create_indices(indices_notobliv, t , size);
        count++;
    }
    ASSERT_TRUE(created)<<"indices_notobliv could not be created, even after "<<t*size*100000<<" attempts.";
	int *bucket_lenghts=(int *) calloc(b, sizeof(int));            
    match **matches = (match **) calloc(b, sizeof(match*));
    std::vector<std::vector<int>> all_buckets = preparations(m,indices_notobliv, matches, bucket_lenghts,func);

    for (int i = 0; i < b; ++i)
    {
    	std::vector<int> this_bucket =all_buckets.at(i);
    	EXPECT_EQ(this_bucket.size(), bucket_lenghts[i]); 
    	match *this_match=matches[i];
		
		bool contained=false;
		for (int j = 0; j < bucket_lenghts[i]; ++j)
		{
			if(this_bucket.at(j)==this_match->val){
				contained=true;
				break;
			}
		}
		if(this_match->val!=-1) {
			EXPECT_TRUE(contained)<<"The match value "<<this_match->val<<" was not contained in  bucket "<<i;
			EXPECT_EQ(this_bucket.at(this_match->index_in_batch), this_match->val);
		}else{
			EXPECT_EQ(-1,this_match->index_in_batch);
		}
		EXPECT_EQ(i, this_match->batch);	
    }

    for (int j = 0; j < t; ++j){
    	int index=indices_notobliv[j];
    	bool found=false;
    	for (int i = 0; i < b; ++i){
    		int test_value=matches[i]->val;
    		if(index==test_value){
    			found=true;
    		}
    	}
    	EXPECT_TRUE(found)<<"Index "<<index<<"was not found in any match.";
    }
}


TEST(preparations, cp2){
	int size= rand()%1000000;
	int w=rand()%100;
	int t=0.01*size;
	int b=1.5*t;
	int max_loop=size;
	int cp=2;

	mpfss_cuckoo *m=new_mpfss_cuckoo(t, (size_t)size, w, b, max_loop,cp);
	int (*func)( int, int)= hashfunc_absl; 
	int *indices_notobliv = (int *)calloc(t, sizeof(int ));

	int *bucket_lenghts=(int *) calloc(b, sizeof(int));            
    match **matches = (match **) calloc(b, sizeof(match*));
    std::vector<std::vector<int>> all_buckets = preparations(m,indices_notobliv, matches, bucket_lenghts,func);

	EXPECT_EQ(all_buckets.size(),b);

	int test_val=rand()%size;
	for (int i = 0; i < w; ++i)
	{
		int bucket_number=hash_this(func,test_val,i, b);
		std::vector<int> this_bucket=all_buckets.at(bucket_number);
		bool contained=std::find(this_bucket.begin(), this_bucket.end(), test_val) != this_bucket.end();
		EXPECT_TRUE(contained)<<"Test value "<<test_val<<" was not contained in  bucket "<<bucket_number<<" (w="<<i<<").";

	}

	for (int i = 0; i < b; ++i)
	{
		std::vector<int> this_bucket=all_buckets.at(i);
		EXPECT_EQ(this_bucket.size(), bucket_lenghts[i]);

	}
	for (int i = 0; i < b; ++i)
	{
		match *this_match=matches[i];
		EXPECT_TRUE(!this_match);
	}

	


}
