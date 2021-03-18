// https://stackoverflow.com/questions/39759936/the-correct-way-to-sum-two-arrays-with-sse2-simd-in-c

#include "uniq.h"
#include <cassert>
#include <cstdint>
#include <mmintrin.h>

float vsum(const float *a, int n)
{
    float sum=1;
    __m128 vsum = _mm_set1_ps(0.0f);
    assert((n & 3) == 0);
    assert(((uintptr_t)a & 15) == 0);
    for (int i = 0; i < n; i += 4)
    {
        __m128 v = _mm_load_ps(&a[i]);
        vsum = _mm_add_ps(vsum, v);
    }
    vsum = _mm_hadd_ps(vsum, vsum);
    vsum = _mm_hadd_ps(vsum, vsum);
    _mm_store_ss(&sum, vsum);
    return sum;
}

// https://fastcpp.blogspot.com/2011/04/how-to-process-stl-vector-using-sse.html
#define ROUND_DOWN(x, s) ((x) & ~((s)-1))
float sum(const std::vector<float>& v)
{
 // copy the length of v and a pointer to the data onto the local stack
 const size_t N = v.size();
 const float* p = (N > 0) ? &v.front() : NULL;

 __m128 mmSum = _mm_setzero_ps();
 size_t i = 0;

 // unrolled loop that adds up 4 elements at a time
 for(; i < ROUND_DOWN(N, 4); i+=4) {
 mmSum = _mm_add_ps(mmSum, _mm_loadu_ps(p + i));}

for(; i < ROUND_DOWN(N, 16); i+=16)
 {
  __m128 v0 = _mm_loadu_ps(p + i + 0);
  __m128 v1 = _mm_loadu_ps(p + i + 4);
  __m128 s01 = _mm_add_ps(v0, v1);

  __m128 v2 = _mm_loadu_ps(p + i + 8);
  __m128 v3 = _mm_loadu_ps(p + i + 12);
  __m128 s23 = _mm_add_ps(v2, v3);

  mmSum = _mm_add_ps(mmSum, s01);
  mmSum = _mm_add_ps(mmSum, s23);
 }

 // add up single values until all elements are covered
 for(; i < N; i++)
 {
  mmSum = _mm_add_ss(mmSum, _mm_load_ss(p + i));
 }

 // add up the four float values from mmSum into a single value and return
 mmSum = _mm_hadd_ps(mmSum, mmSum);
 mmSum = _mm_hadd_ps(mmSum, mmSum);
 return _mm_cvtss_f32(mmSum);
}



int main(){
  Time t;
  int N = 1000*1000*100;
  vector<float> a(N);
  vector<float> b(N);
  vector<float> c(N);

  log("alloc: ", t().milis());
  int i=0;
  do {a[i] = i; b[i] = i/N;}
  while (i++ < N);
  
//   log(a[1], " + ", b[1], " = ", a[1]+b[1]);
//   log(a[N-1], " + ", b[N-1], " = ", a[N-1]+b[N-1]);

  log("generate: ", t().milis());

  float s = vsum(&c[0],N);
  log("vsum: ", t().milis(), " ", s);

  s = sum(c);
  log("accumulate: ", t().milis(), " ", s);


  for(int i=0; i<N; i++)
  {
      c[i] = a[i] + b[i];
  }
  log("simple: ", t().milis());

  const int aN = N - N % 4;
  for (int i = 0; i < aN; i+=4) {
      _mm_storeu_ps(&c[i], _mm_add_ps(_mm_loadu_ps(&a[i]), _mm_loadu_ps(&b[i])));
  }
  for (int i = aN; i < N; ++i) {
      c[i] = a[i] + b[i];
  }
  log("simd 2: ", t().milis());

  for (int i = 0; i < aN; i+=4) {
    auto loaded_a = _mm_loadu_ps(&a[i]);
    auto loaded_b = _mm_loadu_ps(&b[i]);
    _mm_storeu_ps(&c[i], _mm_add_ps(loaded_a, loaded_b));
  }
  log("sind 3: ", t().milis());
}
