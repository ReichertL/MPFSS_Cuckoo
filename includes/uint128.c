#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <math.h>

#include "uint128.h"

//taken from https://www.codeproject.com/Tips/618570/UInt-Multiplication-Squaring

uint128 create_uint128(uint64_t  u, uint64_t  v){
    uint128 a;
     a.Lo=u;
     a.Hi=v;
    return a;

}


void Increment(uint128 *N){
    uint64_t  T = (N->Lo + 1);
    N->Hi += ((N->Lo ^ T) & N->Lo) >> 63;
    N->Lo = T;
}

void mult64to128(uint64_t  u, uint64_t  v, uint64_t  *h, uint64_t  *l)
{
    uint64_t  u1 = (u & 0xffffffff);
    uint64_t  v1 = (v & 0xffffffff);
    uint64_t  t = (u1 * v1);
    uint64_t  w3 = (t & 0xffffffff);
    uint64_t  k = (t >> 32);
    
    u >>= 32;
    t = (u * v1) + k;
    k = (t & 0xffffffff);
    uint64_t  w1 = (t >> 32);
    
    v >>= 32;
    t = (u1 * v) + k;
    k = (t >> 32);
    
    *h = (u * v) + w1 + k;
    *l = (t << 32) + w3;
}

void mult128(uint128 N, uint128 M, uint128 *Ans){

    mult64to128(N.Lo, M.Lo, &Ans->Hi, &Ans->Lo);
    Ans->Hi += (N.Hi * M.Lo) + (N.Lo * M.Hi);
}


/* int main(int argc, char const *argv[])
{
    
    uint128 i;
    i.Lo=3;
    i.Hi=0;

    uint128 j;
    j.Lo=3;
    j.Hi=1;

    uint128 k;
    k.Lo=0;
    k.Hi=0;


    mult128(i, j, &k);

    printf(" %"PRIu64"\n", k.Lo );
    printf(" %"PRIu64"\n", k.Hi);

    return 0;
}*/