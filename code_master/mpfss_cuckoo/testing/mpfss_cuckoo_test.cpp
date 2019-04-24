#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>

#include "gtest/gtest.h"
#include "includes/dbg.h"
#include "../mpfss_cuckoo.h"

using namespace std;

TEST(run_mpfss_cuckoo, small){


    const char* s_host = "localhost";
    char* remote_host = new char[15];
    strncpy(remote_host, s_host, 15);

    const char* s_port = "55555";
    char* port = new char[15];
    strncpy(port, s_port, 15);

    int t= 2;
    int size=20;


	thread t1(run_mpfss_cuckoo, t,size,1,remote_host, port,false );
	thread t2(run_mpfss_cuckoo, t,size,2,remote_host, port,false );
	t1.join();
	t2.join();

    //Testing indices-----------------------------------------------------------
    string filename="results_debug_mpfss_cuckoo_indices.txt";
    ifstream infile(filename); 
    EXPECT_TRUE(infile.good())<<"File with indices does not exist.";
    EXPECT_TRUE(infile.is_open());
    
    std::vector<int> indices;
    int index;
    if (infile.is_open()) {
        while(infile>>index){
            indices.push_back(index);
        }
    }
    infile.close();
    for (int i = 0; i < t; ++i){
        int this_index=indices.at(i);
        EXPECT_LT(this_index,size);
        EXPECT_GE(this_index,0);
        for (int j = t-i+1; j < t ; ++j)
        {
            int other_index=indices.at(j);
            EXPECT_NE(this_index,other_index);
        }
    }

    //Testing mpfss-----------------------------------------------------------
   
    filename="results_debug_mpfss_cuckoo_result.txt";
    ifstream infile2(filename); 
    EXPECT_TRUE(infile2.good())<<"File with result mpfss does not exist.";
    EXPECT_TRUE(infile2.is_open());
    
    int count=0;
    std::vector<int> mpfss;
    if (infile2.is_open()) {
        while(infile2>>index){
            mpfss.push_back(index);
            count++;
        }
    }
    infile2.close();

    EXPECT_EQ(size,count);
   
    std::vector<char> expected(size,0);
    for (int i = 0; i < t; ++i){
        expected.at(indices.at(i))=1;
    }
    for (int i = 0; i < size; ++i){
        EXPECT_EQ(mpfss.at(i), expected.at(i));
    }
}


TEST(run_mpfss_cuckoo, middle){


    const char* s_host = "localhost";
    char* remote_host = new char[15];
    strncpy(remote_host, s_host, 15);

    const char* s_port = "55555";
    char* port = new char[15];
    strncpy(port, s_port, 15);

    int t= 20;
    int size=20000;


    thread t1(run_mpfss_cuckoo, t,size,1,remote_host, port,false );
    thread t2(run_mpfss_cuckoo, t,size,2,remote_host, port,false );
    t1.join();
    t2.join();

    //Testing indices-----------------------------------------------------------
    string filename="results_debug_mpfss_cuckoo_indices.txt";
    ifstream infile(filename); 
    EXPECT_TRUE(infile.good())<<"File with indices does not exist.";
    EXPECT_TRUE(infile.is_open());
    
    std::vector<int> indices;
    int index;
    if (infile.is_open()) {
        while(infile>>index){
            indices.push_back(index);
        }
    }
    infile.close();
    for (int i = 0; i < t; ++i){
        int this_index=indices.at(i);
        EXPECT_LT(this_index,size);
        EXPECT_GE(this_index,0);
        for (int j = t-i+1; j < t ; ++j)
        {
            int other_index=indices.at(j);
            EXPECT_NE(this_index,other_index);
        }
    }

    //Testing mpfss-----------------------------------------------------------
   
    filename="results_debug_mpfss_cuckoo_result.txt";
    ifstream infile2(filename); 
    EXPECT_TRUE(infile2.good())<<"File with result mpfss does not exist.";
    EXPECT_TRUE(infile2.is_open());
    
    int count=0;
    std::vector<int> mpfss;
    if (infile2.is_open()) {
        while(infile2>>index){
            mpfss.push_back(index);
            count++;
        }
    }
    infile2.close();

    EXPECT_EQ(size,count);

    std::vector<char> expected(size,0);
    for (int i = 0; i < t; ++i){
        expected.at(indices.at(i))=1;
    }
    for (int i = 0; i < size; ++i){
        EXPECT_EQ(mpfss.at(i), expected.at(i));
    }
}