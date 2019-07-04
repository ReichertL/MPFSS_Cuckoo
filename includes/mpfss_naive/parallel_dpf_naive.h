#pragma once

typedef void (* split_fn)( void *, int , void*);
void parallize_naive(split_fn fn, void ** list_pd_split, int b, void * params);
