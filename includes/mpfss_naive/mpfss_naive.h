//ensures header file is only included once
#pragma once

double lap;
int cp;


typedef struct {
    int t; // number of points in mpfss
    size_t size; //size of input field

} mpfss;

mpfss * new_mpfss_naive( int t, size_t size);
void mpfss_naive(void* args);