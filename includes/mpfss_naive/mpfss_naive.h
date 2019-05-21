//ensures header file is only included once
#pragma once

int cp;


typedef struct {
    int t; // number of points in mpfss
    size_t size; //size of input field

} mpfss;

mpfss * new_mpfss_naive( int t, size_t size);
void run_mpfss_naive(const char *remote_host, const char *port, int cp, int t, int size);
void mpfss_naive(void* args);