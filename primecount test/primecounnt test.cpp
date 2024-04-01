#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <mpirxx.h>
#include <mpir.h>
#include "..\prime count\primecount.h"


int main() {
	std::cout.imbue(std::locale(""));
	int N = 100;
	mpz_class sieve;
	while (N <= 10'000'000) {
		primesieve2(sieve, N);
		printsieve(sieve, N);
		N *= 10;
	}

	// test segmentmask
	mpz_class mask;
	mpz_class test;
	int64_t start = 100'000'000'000;
	int64_t segmentsize = 10'000'000;

	segmentmask(start, start + segmentsize, mask);
	std::cout << "created segment mask, verifying... \n";

	for (int64_t i = start; i <= start + segmentsize; i++) {
		int r1 = mpz_tstbit(mask.get_mpz_t(), i - start);
		test = i;
		int r2 = mpz_probab_prime_p(test.get_mpz_t(), 5);
		if (r1 + r2 != 1) {
			std::cout << i + start << " mismatch " << r1 << "," << r2 << "\n";
		}
	}
	std::cout << "segmentmask tests complete\n";

}