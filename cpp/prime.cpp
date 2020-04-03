// parallel primality test for uniQ Library
// compile using ./build prime

#include <bits/stdc++.h>

#include <algorithm>
#include <iostream>

// #include "./utils.h"
#include "./color.h"
#include "./pool.h"
using namespace std;

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
u64 spiralDivisor(u64 n, u64 limit = 0) {
  if (!(n % 2)) return 2;
  if (!(n % 3)) return 3;
  if (!(n % 5)) return 5;
  if (limit == 0) limit = n;
  limit = std::min(limit, (u64)sqrt(n));
  return spiral(n, 7, limit);
}

/* paralel version using spiral */
u64 result;

inline u64 mineBlock(u64 n, u64 min, u64 max) {
  result = spiral(n, min, max);
  return result;
}

u64 paralelDivisor(u64 n) {
  u64 blockSize = 3e6;
  atomic<u64> block(7);
  atomic<int> freeWorkers(4);

  // try a first block, before go wild
  u64 result = spiralDivisor(n, block += blockSize);

  for (u64 b = block; (result == n) && b < sqrt(n); b += blockSize) {
    run(mineBlock, n, b, b + blockSize);
    // call(spiral, n, b, b + blockSize);
    // call(spiral, n, b, b + blockSize, [&](u64 res) {
    //   if (res < n) result = res;
    // });
  }
  // printf("done.");
  // pool.wait(result, a, b, c, d);
  return result;
}

int main() {
  u32 bigPrime32 = 2147483647;             // 2^31-1
  u64 bigPrime64 = 18446744073709551557U;  // biggest 64 bit prime

  u64 maxU32 = 0xFFFFFFFF;
  u64 maxU64 = 0xFFFFFFFFFFFFFFFF;
  u64 bigSquare64 = bigPrime32 * bigPrime32;  // biggest 32 bit prime

  // calc top 5 primes below a range
  printf("Calculating some big primes... \nPress Ctrl+C to stop\n");

  u64 candidate = bigSquare64;
  int count = 1;
  while (true) {
    u64 divisor = paralelDivisor(candidate);
    bool isPrime = (divisor == candidate);
    if (isPrime) {
      printf("%d. %llu\n", count, candidate);
      if (count++ == 8) break;
    };
    candidate--;
  }
  pool.stop();  // todo: remove this
  return 0;
}
