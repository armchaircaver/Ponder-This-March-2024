#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <mpirxx.h>
#include <mpir.h>
#include "..\prime count\primecount.h"
#include "Timer.h"

int triang(int n) {
    return (n * (n + 1))/2;
}

int solve(int n, int segmentsize = 100'000'000, int startprime = 0) {

    mpz_class one = 1;
    mpz_class primes;

    while (1) {
        segmentmask(startprime, static_cast<int64_t>(startprime) + segmentsize + triang(n), primes);

        mpz_class solutions = (one << (segmentsize + 2)) -1;

        for (int i = 0; i < n; i++) {
            solutions &= primes;
            primes >>= (static_cast<mpir_ui>(i) + 1);
        }
 
        if (solutions > 0) {
            for (int i = 0; i < segmentsize; i++)
                if (mpz_tstbit(solutions.get_mpz_t(), i))
                    return i + startprime;
        }
        startprime += segmentsize;
    }
}



int main() {
    Timer tim;
 
    for (int i : {4, 5, 6, 1000, 2024}) {
        std::cout << "solving for "<< i << "\n";
        tim.start();
        int s = solve(i, 10'000'000, 0);
        tim.end();
        std::cout << i << ", " << s <<", "<< tim.ms() <<"ms \n";
    }
}
