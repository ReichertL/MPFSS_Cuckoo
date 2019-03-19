//ensures header file is only included once
#pragma once

double lap;
int cp;


typedef struct {
    int t; // number of points in mpfss
    size_t size; //size of input field
    int max_loop;
    int w; //number of hashfunctions
    int b; //number of buckets

} mpfss_cuckoo;

mpfss_cuckoo * new_mpfss_cuckoo( int t, size_t size, int w, int b, int max_loop);
void mpfss_batch_cuckoo(void* args);