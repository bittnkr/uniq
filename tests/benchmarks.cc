#include "uniq.h"
#include "Profiler.h" // execution time recording
#include "Benchmark.h" // speed tests

// a benchmark function receives a function pointer and return the run time
Time benchmark(voidfunction f, double timeout_ = 0.1) {
  Time timeout, best, r;
  i64 count = 1;

  while (double(timeout(-timeout_)) < 0) {
    Time t = CpuTime();

    double c = count;
    while (c--)
      f();

    t = t(CpuTime()) / double(count);
    if (t < best)
      best = t;

    count *= 2;  // log(log2(count), ":", best);
  };
  return best;
}

int main() {
  for (auto i = 0; i < 5; i++)
  {
    log(benchmark([] {}, 0.1), " ", Benchmark("overhead", [] {}, 0));
  }
}