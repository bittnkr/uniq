#include "uniq.h"
using namespace uniq;

#include <immintrin.h>  // Needed for __get_cpuid and __get_cpuid_max

bool is_avx2_supported() {
  // Check if the AVX2 instruction set is supported
  int a, b, c, d;
  if (__get_cpuid(1, &a, &b, &c, &d) == 0) {
    return false;
  }
  return (d & (1 << 5)) != 0;
}

// spiral algorithm test fewer candidates. www.primesdemystified.com
u64 spiral_simd(u64 n, u64 min, u64 max) {
  if (!is_avx2_supported()) {
    printf("AVX2 is not supported on this machine\n");
    return 1;
  }

  // Create a vector with the values [i, i+4]
  __m128i divisors = _mm_set_epi64x(min + 4, min);

  // Set the values of the third and fourth elements of the vector
  divisors = _mm_insert_epi64(divisors, min + 6, 2);
  divisors = _mm_insert_epi64(divisors, min + 8, 3);

  for (u64 i = min; i <= max; i += 30) {
    // Perform a vectorized division of n by the divisors vector
    __m128i quotients = _mm_div_epu64(n, divisors);

    // Check if any of the quotients is equal to 0
    u64 mask = _mm_movemask_epi8(quotients);
    if (mask != 0) {
      // One of the divisors divides n evenly, so return the corresponding divisor
      return i + _tzcnt_u64(mask);
    }

    // Increment the divisors vector by 30
    divisors = _mm_add_epi64(divisors, _mm_set1_epi64x(30));
  }

  // If no divisor is found, return n
  return n;
}
// Note that this code assumes that the CPU supports the AVX2 instruction set, which includes the _mm_div_epu64 and _mm_movemask_epi8 functions. If the CPU does not support AVX2, you may need to use a different set of SIMD instructions or a different approach altogether.
// It is also worth noting that this optimization may not necessarily lead to a significant improvement in performance, as the overhead of using SIMD instructions and the vectorized division may outweigh the benefits of the increased parallelism. You may want to benchmark the optimized version of the code to see if it performs better than the original version on your specific system.
