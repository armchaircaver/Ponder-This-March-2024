#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <mpirxx.h>
#include <mpir.h>

mpz_class one = 1;
mpz_class two = 2;

// performance is terrible due to the division step
/*
mpz_class  primesieve(int N) {
	mpz_class compositemask;
	mpz_class P;
	mpz_class sieve = 3; // bit 'c' is set if c is composite

	for (int p = 2; p * p <= N; p++) {
		// compositemask = P*P*(P^(N/p)-1)/(P-1) where P = 2^p
		if (!mpz_tstbit(sieve.get_mpz_t(), p)) {
			//std::cout << "p =" << p << "\n";
			mpz_pow_ui(P.get_mpz_t(), two.get_mpz_t(), p);
			mpz_pow_ui(compositemask.get_mpz_t(), P.get_mpz_t(), N / p);
			compositemask -= 1;
			//compositemask *= P*P;
			mpz_mul_2exp(compositemask.get_mpz_t(), compositemask.get_mpz_t(), 2 * p);
			compositemask /= P - 1;

			//std::cout << "compositemask " << compositemask << "\n";

			sieve |= compositemask;
		}
	}
	sieve &= (one << static_cast<mp_bitcnt_t>(N)) - 1;

	return sieve;
}
*/

void   primesieve2(mpz_class &sieve, int N) {
	sieve = (one << static_cast<mp_bitcnt_t>(N)) + 3; // bit 'c' is set if c is composite

	for (int p = 2; p * p <= N; p++) {
		if (!mpz_tstbit(sieve.get_mpz_t(), p)) {
			for (int c = p * p; c <= N; c += p) {
				mpz_setbit(sieve.get_mpz_t(), c);
			}
		}
	}
	sieve &= (one << static_cast<mp_bitcnt_t>(N)) - 1;
}

// construct mask where mask[i]=1 iff i+pmin is composite
__declspec(noinline) void  segmentmask(int64_t pmin, int64_t pmax, mpz_class& mask) {
	// create a mask of bits of size pmax - pmin + 1
	// representing primes in that range

	if (pmin == 0) {
		primesieve2(mask, (int)pmax);
		return;
	}

	mask = one << (pmax-pmin + 2);
	mpz_class sieve; // sieve up to sqrt(pmax)
	primesieve2(sieve, int(sqrt(pmax)) + 1);

	std::cout << "segmentmask: " << pmin << " to " << pmax << "\n";
	for (int64_t p = 2; p * p <= pmax; p++) {
		if (mpz_tstbit(sieve.get_mpz_t(), p))
			continue;
		//std::cout << "primesieve2, p=" << p << "\n";
		int64_t y = (pmin / p)*p;
		while (y < pmin)
			y += p;
		//std::cout << "primesieve2, start y=" << y << "\n";
		for (; y <= pmax; y += p)
			mpz_setbit(mask.get_mpz_t(), y-pmin);
	}
	mask &= (one << (pmax - pmin + 2)) - 1;

}

void printsieve(mpz_class & sieve, int N){
	if (N <= 1000) {
		for (int i = 0; i < N; i++) {
			if (!mpz_tstbit(sieve.get_mpz_t(), i))
				std::cout << i << ",";
			if (i > 200)
				break;
		}
		std::cout << "\n";
	}

	int primecount = N - mpz_popcount(sieve.get_mpz_t());
	std::cout << primecount << " primes up to " << N << "\n";

}
