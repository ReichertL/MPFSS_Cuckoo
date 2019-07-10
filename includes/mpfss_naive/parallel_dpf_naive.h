#pragma once

typedef void (* split_fn)( void *, int ,int , void*);
void parallize_naive(split_fn fn, void ** list_pd_split, int threads, int t, void * params);
