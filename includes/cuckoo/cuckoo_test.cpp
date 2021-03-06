#include <string>
#include <cmath>
#include <stdlib.h>     /* srand, rand */
#include <time.h>  

#include "gtest/gtest.h"
#include "absl/hash/hash_testing.h"

#include "cuckoo.h"



TEST(fast_mod, Works){
	//TODO input zufällig generieren
	int input=21;
	int ceil=20;
	int target=input%ceil;

	EXPECT_EQ(
		target,
		fast_mod(input,ceil)	
	);

	input++;
	EXPECT_NE(
		target,
		fast_mod(input,ceil)	
	);

	input++;
	ceil++;
	EXPECT_NE(
		target,
		fast_mod(input,ceil)	
	);
}

TEST(hashfunc_absl, Works){
	for (int j = 0; j< 20; ++j){
		
		srand (time(NULL));
		int key = rand();
		int i=rand();
		int offset=rand()%10+1;
		EXPECT_NE(
			hashfunc_absl(i+offset,key),
			hashfunc_absl(i,key)	
		);

		EXPECT_NE(
			hashfunc_absl(i,key+offset),
			hashfunc_absl(i,key)	
		);

	}
}

TEST(hash_this, Works){
	for (int j = 0; j< 20; ++j){
		
		srand (time(NULL));
		int key = rand();
		int i=rand();
		int ceil=rand();

		EXPECT_EQ(
			abs(hashfunc_absl(i,key)%ceil),
			hash_this(hashfunc_absl,key,i,ceil)	
		);

		EXPECT_EQ(
			abs(hashfunc_simple(i,key)%ceil),
			hash_this(hashfunc_simple,key,i,ceil)	
		);

		EXPECT_LE(
			0,
			hash_this(hashfunc_simple,key,-i,ceil)	
		);

		EXPECT_LE(
			0,
			hash_this(hashfunc_simple,-key,i,ceil)	
		);
	}
}

/*TEST(initialize, rands_array_predefined){
	
		
	srand (time(NULL));
	int w = rand()%100000;
	int no_hash_tables=rand()%1000;
	int *size_hash_tables=(int *)calloc(no_hash_tables,sizeof(int));
	for (int i = 0; i < no_hash_tables; ++i)
	{
		size_hash_tables[i]=rand()%100000;
	}
	int *i_array=(int *)calloc(w,sizeof(int));
	for (int i = 0; i < w; ++i)
	{
		i_array[i]=rand();
	}
	int max_loop=rand();
	cuckoo_hashing *c=initialize(w, no_hash_tables, size_hash_tables, i_array, max_loop, hashfunc_absl );

	EXPECT_EQ(w,c->no_hash_functions);
	EXPECT_EQ(no_hash_tables,c->no_hash_tables);
	EXPECT_EQ(size_hash_tables,c->size_hash_tables);
	EXPECT_EQ(max_loop,c->max_loop);

	//casting does not work
	
	ASSERT_EQ(no_hash_tables*2,static_cast<int>(c->tables.size()));
	for (int i = 0; i < no_hash_tables; ++i)
	{
		EXPECT_EQ(0,c->tables.at(i).size());
		EXPECT_EQ(0,c->table_usage.at(i).size());

	}
	EXPECT_EQ(hashfunc_absl,c->hash_function);

	for (int i = 0; i < w; ++i)
	{
		EXPECT_EQ(i_array[i],c->rands.at(i));
	}
}

TEST(initialize, no_rands_array){
	
		
	srand (time(NULL));
	int w = rand()%100000;
	int no_hash_tables=rand()%1000;
	int *size_hash_tables=(int *)calloc(no_hash_tables,sizeof(int));
	for (int i = 0; i < no_hash_tables; ++i)
	{
		size_hash_tables[i]=rand()%100000;
	}

	int max_loop=rand();
	cuckoo_hashing *c=initialize(w, no_hash_tables, size_hash_tables, NULL, max_loop, hashfunc_absl );

	EXPECT_EQ(w,c->no_hash_functions);
	EXPECT_EQ(no_hash_tables,c->no_hash_tables);
	EXPECT_EQ(size_hash_tables,c->size_hash_tables);
	EXPECT_EQ(max_loop,c->max_loop);

	//casting does not work
	ASSERT_EQ(no_hash_tables*2,static_cast<int>(c->tables.size()));
	for (int i = 0; i < no_hash_tables; ++i)
	{
		EXPECT_EQ(0,c->tables.at(i).size());
		EXPECT_EQ(0,c->table_usage.at(i).size());

	}
	EXPECT_EQ(hashfunc_absl,c->hash_function);

	for (int i = 0; i < w; ++i)
	{
		EXPECT_EQ(i,c->rands.at(i));
	}
}*/

TEST(cuckoo, Works){

	srand (time(NULL));
	int w = rand()%100000;
	int no_hash_tables=3;//rand()%1000;
	int *size_hash_tables=(int *)calloc(no_hash_tables,sizeof(int));
	int ceil=0;
	for (int i = 0; i < no_hash_tables; ++i)
	{
		size_hash_tables[i]=rand()%100000;
		ceil=ceil+size_hash_tables[i];
	}
	int max_loop=rand();
	cuckoo_hashing *c=initialize(w, no_hash_tables, size_hash_tables, NULL, max_loop, hashfunc_absl );

	ceil=0.01*ceil;
	int no_keys=rand()%ceil;
	int *keys= (int *) calloc(no_keys, sizeof(int));
	for (int i = 0; i < no_keys; ++i)
	{
		keys[i]=rand();		
	}

	cuckoo(keys, no_keys, c);

	for (int i = 0; i < no_hash_tables; ++i)
	{
		for (int j = 0; j <size_hash_tables[i] ; ++j)
		{
			bool used =c->table_usage.at(i).at(j);
			int set=c->tables.at(i).at(j);
			if(set>0){
				EXPECT_TRUE(used);
			}
			if(!used){
				EXPECT_TRUE(set==0);
			}else{
				bool found=false;
				for (int k = 0; k < no_keys; ++k)
				{
					if(keys[k]==set){
						found=true;
					}
				}
				EXPECT_TRUE(found);
			}
		}
	}
}