// parallel primality test for uniQ Library
// compile using ./build prime

#include <bits/stdc++.h>

#include <algorithm>
#include <iostream>

#include "math.h"
using namespace std;

typedef unsigned long long u64;

// basic algorithm 
u64 firstDivisor0(u64 n) {
  if ((n % 2) == 0) return 2;
  for (u64 i = 3; i <= sqrt(n); i += 2) {
    if ((n % i) == 0) return i;
  }
  return n;
}

// spiral algorithm test fewer candidates
u64 spiral(u64 n, u64 min, u64 max) {
  for (u64 i = min; i <= max; i += 30) {
    u64 j = i;
    // if (j >= 6000000) asm("int3");
    if (!(n % j)) return j;
    if (!(n % (j += 4))) return j;
    if (!(n % (j += 2))) return j;
    if (!(n % (j += 4))) return j;
    if (!(n % (j += 2))) return j;
    if (!(n % (j += 4))) return j;
    if (!(n % (j += 6))) return j;
    if (!(n % (j += 2))) return j;
  }
  return n;
}

// firstDivisor using spiral
u64 firstDivisor1(u64 n, u64 limit = 0) {
  if (!(n % 2)) return 2;
  if (!(n % 3)) return 3;
  if (!(n % 5)) return 5;
  if (limit == 0) limit = n;
  limit = std::min(limit, (u64)sqrt(n));
  return spiral(n, 7, limit);
}

/* paralel version using spiral * /
#include "pool.h"
u64 paralelDivisor(u64 n) {
  u64 i = 3e6;
  u64 result = firstDivisor1(n, i);

  if (result < n) return result;

  // // a batch restrict the amout of jobs to the number of cores
  // pool.on(spiral, [&](u64 res) {
  //   if (res < n) result = res;
  // });

  while (i < sqrt(n) && !result) {  //
    run(spiral, n, i, i += 3e6);
    // pool.wait(result, a, b, c, d);
  }
  return result;
}
/**/

int main() {
  u64 bigPrime = 18446744073709551557U;  // biggest 64 bit prime
  cout << "Calculating primality of: " << bigPrime << "\n";
  cout << (firstDivisor1(bigPrime) == bigPrime ? "is prime" : "is not prime\n");
  // pool.stop();
  return 0;
}
