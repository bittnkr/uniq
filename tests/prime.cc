// parallel primality test
// compile using ./build prime
#include "uniq.h"
using namespace uniq;

// basic algorithm to get the first integer divisor of a number
u64 firstDivisor0(u64 n) { // O(0.5 sqrt(n))
  if ((n % 2) == 0) return 2;
  for (u64 i = 3; i <= sqrt(n); i += 2) {
    if ((n % i) == 0) return i;
  }
  return n; // is prime
}

// spiral algorithm test fewer candidates. www.primesdemystified.com
u64 spiral(u64 n, u64 min, u64 max) { // O(0.27 sqrt(n))
  // log("block: ",min," ",max);
  for (u64 i = min; i <= max; i += 30) { // todo: use SIMD
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
  // if(is_avx2_supported()) return spiral_simd(n, 7, limit);
  return spiral(n, 7, limit);
}

/* paralel version using spiral, distribute work to cores using blocks
and pushing each block into the queue until a worker find a divisor. */
u64 paralelDivisor(u64 n) {
  u64 blockSize = 3e6;
  atomic<u64> block(7);

  // try a first block, before go wild
  u64 result = spiralDivisor(n, block += blockSize);

  for (u64 b = block; (result == n) && b < sqrt(n); b += blockSize) {

    run([&]{ // this lambda runs in a worker thread
      u64 res = spiral(n, b, b + blockSize);
      if (res < n) result = res;
    });

  }
  // emit('prime',n);
  return result;
}

int main() {
  u64 bigPrime64 = 18446744073709551557U;  // biggest 64 bit prime

  u64 d, n = bigPrime64;
  int count = 1;
  int sumtime = 0;

  out("Single threaded brute force primality test of: \n", n);
  Time t;
  d = spiralDivisor(n);
  log(" ", t=t());

  pool().showstats = true;
  pool().start();

  log("\nNow using ", pool().workers.size(), " worker threads");

  Time tp;
  d = paralelDivisor(n);
  pool().stop();

  // WAIT( run( [&d,n]{ d = paralelDivisor(n); }) );

  if (d == n) log(count++,". ",n, " ", setprecision(2),  tp=tp());
  
  log("\nSpeedup ", setprecision(2), double(t/tp), "x");
  sleep(100); // allow std out printing
  log("");
  quick_exit(0); // return 0;
}