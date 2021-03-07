// parallel primality test for uniQ Library
// compile using ./build prime
#include "uniq.h"

// basic algorithm to get the first integer divisor of a number
u64 firstDivisor0(u64 n) { // O(0.5 sqrt(n))
  if ((n % 2) == 0) return 2;
  for (u64 i = 3; i <= sqrt(n); i += 2) {
    if ((n % i) == 0) return i;
  }
  return n;
}

// spiral algorithm test fewer candidates
u64 spiral(u64 n, u64 min, u64 max) { // O(0.27 sqrt(n))
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

/* paralel version using spiral,
distribute work to cores using blocks
and pushing each block into the queue
until a worker find a divisor. */
u64 paralelDivisor(u64 n) {
  u64 blockSize = 3e6;
  atomic<u64> block(7);

  // try a first block, before go wild
  u64 result = spiralDivisor(n, block += blockSize);

  for (u64 b = block; (result == n) && b < sqrt(n); b += blockSize) {
    uniq::run(
        [&](u64 n, u64 min, u64 max) {
          // this lambda runs in a worker thread
          u64 res = spiral(n, min, max);
          if (res < n) result = res;
        },
        n, b, b + blockSize);

    // run([&]() { // smaller but slower
    //   u64 res = spiral(n, b, b + blockSize);
    //   if (res < n) result = res;
    // });
  }
  return result;
}

int main() {
  u32 bigPrime32 = 2147483647;             // 2^31-1
  u64 bigPrime64 = 18446744073709551557U;  // biggest 64 bit prime
  u64 bigSquare64 = bigPrime32 * bigPrime32;
  // setlocale(LC_NUMERIC, "C");
  
  // printf("%llu/%llu: %llux\n",bigPrime64, bigSquare64, bigPrime64/bigSquare64);
  // calc top 5 primes below a range
  u64 n = bigPrime64;  // bigSquare64;
  int count = 1;
  int sumtime = 0;

  // single threaded
  printf("Calculating some big primes... Press ctrl+c to stop\n\n");
  printf("Single Threaded factoring of %llu: ", n);
  cout << std::flush;

  Timer timer;  // startTimer();
  spiralDivisor(n);
  int singleTimer = (int)timer.reset();
  printf("%d ms\n", singleTimer);

  printf("\nNow using %d worker threads \n", uniq::pool.size());
  uniq::pool.start(); // todo: remove this
  // while (count <= 8) {
    u64 divisor = paralelDivisor(n);
    if (divisor == n) {
      printf("%d. %llu: %d ms\n", count++, n, (int)timer.round());
    };
  //   n--;
  // }
  int avg = timer.roundAvg();
  printf("\nSpeedup %.1f x\n", (double)singleTimer / avg);

  uniq::pool.stop();  // todo: remove this
  return 0;
}
