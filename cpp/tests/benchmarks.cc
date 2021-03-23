#include "uniq.h"
using namespace uniq;

Time overhead(0);

// a benchmark function receives a function pointer and return the run time
Time benchmark(voidfunction f, double timeout_ = 0.1){
  Time timeout, t, best, r; 
  i64 count = 1;

  while(double(timeout(-timeout_))<0){
    Time t = CpuTime();

    double c = count;
    while(c--) f();

    t = t(CpuTime())/double(count);
    if(t < best) best = t;

    count *= 2; // log(log2(count), ":", best);
  };
  return best - overhead;
}

void logbenchmark(const string name, voidfunction f, double timeout_ = 0.1){
  log( ORA, name, " ", GRN, benchmark(f, timeout_));
}

// measure the number to clock ticks for a function call of type u64 f(u64 &counter) 
// where counter is decremented for each function call, until zero;
// i64 c=1000; log(cycles(f,c)); f receives c and should return c-1
inline i64 decr(i64 &i) { return --i; }

inline i64 pause(i64 count) {  while(--count > 0) __asm__( "pause;" ); return 0;}

Time logbenchmark(const string name, i64 (*f)(i64&), i64 counter=1e5){
  Time t; 
  // for(i64 i=counter; f(i););
  i64 i=counter; while(f(i));
  t = t()/double(counter);
  log( ORA, name," ", GRN, t);
  return t;
}

// template <class Func, class... Args>
// auto benchmark(string name, Func&& func, Args&&... args) {
//   auto f = bind(forward<Func>(f), forward<Args>(args)...);
//   log( ORA, name, GRN, Benchmark(f));
// }

int main()
{
  overhead = benchmark([]{ },0.2);
  logbenchmark("pause[]()", []{ uniq::pause(); });
  logbenchmark("Time()",    []{ Time(); });
  logbenchmark("ticks()",   []{ ticks(); });
  logbenchmark("ticks(0)",  []{ ticks(0); });
  logbenchmark("CpuTime()", []{ CpuTime(); });
  
  logbenchmark("decr",    decr, 1e6 );

  auto v = [](i64 &i)->i64 { 
    vector<u64> v;
    while(i) v.push_back(i--);
    return i;
  };
  logbenchmark("vector.push_back()", v, 1000);

  logbenchmark("vector.push_back(reserved)",[](i64 &N)->i64 { 
    vector<u64> v; v.reserve(N);
    for(auto i = 0; i < N; ++i) v.push_back(i);    
    return 0;
  });

  logbenchmark("deque.push_back() ",[](i64 &N)->i64 { 
    deque<u64> v;
    for(auto i = 0; i < N; ++i) v.push_back(i);    
    return 0;
  });

  logbenchmark("list.push_back() ",[](i64 &N)->i64 { 
    list<u64> v;
    for(auto i = 0; i < N; ++i) v.push_back(i);    
    return 0;
  });
 
  logbenchmark("set.insert() ",[](i64 &N)->i64 { 
    set<u64> v;
    for(auto i = 0; i < N; ++i) v.insert(i);    
    return 0;
  });

  logbenchmark("unordered_set.insert() ",[](i64 &N)->i64 { 
    unordered_set<u64> v; v.reserve(N);
    for(auto i = 0; i < N; ++i) v.insert(i);    
    return 0;
  });

  logbenchmark("Queue push&pop",[](i64 &N)->i64 { 
    Queue<int> Q; int v;
    for(auto i = 0; i < N; ++i){
       Q.push(i);    
       Q.pop(v);
       assert(i==v);
    }
    return 0;
  });
}