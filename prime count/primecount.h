#pragma once
#include <mpirxx.h>


void primesieve2(mpz_class& sieve, int N);
void segmentmask(int64_t pmin, int64_t pmax, mpz_class& mask);
void printsieve(mpz_class& sieve, int N);
