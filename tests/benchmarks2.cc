#include "Benchmark.h"
#include "uniq.h"
using namespace uniq;

// measure the number to clock ticks for a function call of type u64 f(u64 &counter)
// where counter is decremented for each function call, until zero;
// i64 c=1000; cycles(f,c)); f receives c and should return c1
inline i64 decr(i64& i) { return --i; }
inline i64 pause(i64& count) { while (--count) __asm__("pause;"); return 0; }

inline void pause1() {__asm__( "pause;" );}

int main() {
  Benchmark("ticks()", []{ ticks(); });
  Benchmark("Time()", []{ Time(); });
  Benchmark("ticks(0)", []{ ticks(0); });
  Benchmark("CpuTime()", []{ CpuTime(); });
  Benchmark("pause()", []{ uniq::pause(); });

  // i64 f(i64 N)
  Benchmark("\ndecr(n)", decr);
  Benchmark("pause1", pause1);

  Benchmark("\nif(N)", [](i64& N) -> i64 { if(N) N--; return N; });
  Benchmark("if(!N)else", [](i64& N) -> i64 { if(!N) N=0; else N--; return N; });

  Benchmark("\nvector(n)", [](i64& N) -> i64{
    vector<u64> v;
    for (auto i = 0; i < N; ++i)
      v.push_back(i);
    for (auto i = 0; i < N; ++i)
      v.pop_back();
    return 0;
  });

  Benchmark("deque(n)", [](i64& N) -> i64{
    deque<u64> v;
    for (auto i = 0; i < N; ++i)
      v.push_back(i);
    for (auto i = 0; i < N; ++i)
      v.pop_back();
    return 0;
  });

  Benchmark("list(n)", [](i64& N) -> i64{
    list<u64> v;
    for (auto i = 0; i < N; ++i)
      v.push_back(i);
    for (auto i = 0; i < N; ++i)
      v.pop_back();
    return 0;
  });

  // Benchmark("\nset(n)", [](i64& N) -> i64{
  //   set<u64> v;
  //   for (auto i = 0; i < N; ++i)
  //     v.insert(i);
  //   for (auto i = 0; i < N; ++i)
  //     v.erase(i);
  //   return 0;
  // });

  // Benchmark("unordered_set(n)", [](i64& N) -> i64{
  //   unordered_set<u64> v;
  //   v.reserve(N);
  //   for (auto i = 0; i < N; ++i)
  //     v.insert(i);
  //   for (auto i = 0; i < N; ++i)
  //     v.erase(i);
  //   return 0;
  // });

  Benchmark("queue(1)", [](i64& N) -> i64{
    queue<u64> v;
    while (--N > 0){
      v.push(N);
      assert(v.back() == N);
      v.pop();
    }
    return N;
  });

 Benchmark("Queue(1)", [](i64& N) -> i64{
    Queue<int> Q;
    int v;
    for (auto i = 0; i < N; ++i) {
      Q.push(i);
      Q.pop(v);
      assert(i == v);
    }
    return 0;
  });
}