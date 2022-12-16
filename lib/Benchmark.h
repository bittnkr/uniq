//==============================================================================
// Benchmark • Receives a function pointer and return the execution time
//==============================================================================

#pragma once
// #include "uniq.h"
namespace uniq {

struct Benchmark : public Name {
  inline static Time timeout = 0.1;
  static Time overhead;

  Time result;

  Benchmark(string name, voidfunction f, bool print = 1)
      : Name(name) {
    Time t(CpuTime());
    i64 count = 1;

    while (double(CpuTime(t)) < timeout) {
      Time r = CpuTime();

      i64 c = count;

      while (c--)
        f();

      r = CpuTime(r) / double(count);
      if (r < result)
        result = r;

      count *= 2;
      // log(log2(count), ":", r);
    };

    if (name != "overhead")
      result -= overhead;

    if (print)
      log(*this);
  }

  Benchmark(string name, i64 (*f)(i64&), i64 counter = 1e6, bool print = 1)
      : Name(name) {
    Time r;
    // for(i64 i=counter; f(i););
    i64 i = counter;
    while (f(i)) {
    };
    result = r() / double(counter);
    if (print)
      log(*this);
  }
};

Time Benchmark::overhead = Benchmark(
                               "overhead",
                               [] {},
                               0)
                               .result;

ostream& operator<<(ostream& os, Benchmark& b) {
  return os << sstr(ORA, b.name, " ", GRN, b.result, YEL, DIM, " (", integer(1 / (b.result * 1e6)), " M op/s)");
};

// template <class Func, class... Args>
// auto benchmark(string name, Func&& func, Args&&... args) {
//   auto f = bind(forward<Func>(f), forward<Args>(args)...);
//   log( ORA, name, GRN, Benchmark(f));
// }

//========================================================================= test
inline i64 test_decr(i64& i) {
  return --i;
}

TEST(Benchmark) {
  CHECK(double(Benchmark("decr(n)", test_decr, 100'000, 0).result) > 0);
}

}// uniq • Released under GPL 3.0