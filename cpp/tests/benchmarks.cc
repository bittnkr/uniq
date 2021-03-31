#include "uniq.h"
using namespace uniq;

Time overhead(0);

// a benchmark function receives a function pointer and return the run time
Time benchmark(voidfunction f, double timeout_ = 0.1) {
  Time timeout, t, best, r;
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
  return best - overhead;
}

void logbenchmark(const string name, voidfunction f, double timeout_ = 0.1) {
  log(ORA, name, " ", GRN, benchmark(f, timeout_));
}

// measure the number to clock ticks for a function call of type u64 f(u64 &counter)
// where counter is decremented for each function call, until zero;
// i64 c=1000; log(cycles(f,c)); f receives c and should return c-1
inline i64 decr(i64& i) {
  return --i;
}

inline i64 pause(i64 count) {
  while (--count > 0)
    __asm__("pause;");
  return 0;
}

Time logbenchmark(const string name, i64 (*f)(i64&), i64 counter = 1e5) {
  Time t;
  // for(i64 i=counter; f(i););
  i64 i = counter;
  while (f(i))
    ;
  t = t() / double(counter);
  log(ORA, name, " ", GRN, t);
  return t;
}

// template <class Func, class... Args>
// auto benchmark(string name, Func&& func, Args&&... args) {
//   auto f = bind(forward<Func>(f), forward<Args>(args)...);
//   log( ORA, name, GRN, Benchmark(f));
// }

int main() {
  while (1) {
    Benchmark("\noverhead", [] {});
    log(benchmark([] {}, 0.2));

    char c = getch();
    if (c == 27) break;
  }
}