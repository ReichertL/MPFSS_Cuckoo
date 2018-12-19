//ensures header file is only included once
#pragma once


bool TEST_get_mpfss_vectors(int t, size_t size);
bool TEST_new_mpfss_naive( int t, size_t size);
bool TEST_dpf(size_t size, int index);
void TEST_ALL(void* args)
