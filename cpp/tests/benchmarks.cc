/* Plano para o timer da UNIQ
O worker salva inicio e fim 
*/
#include "uniq.h"
using namespace uniq;

// a benchmark function receives a function pointer and return the run time
Time overhead(0);

Time benchmark(voidfunction f, double timeout_ = 0.1){
  Time timeout, t, best, r; 
  int count = 1;

  while(double(timeout(-timeout_))<0){
    Time r = procTime();

    int c = count;
    while(c--) f();

    r = r(procTime()) / Time(count);
    if(r < best) best = r;

    count *= 2; // log(log2(count), ":", best);
  };
  return best - Time(overhead);
}

int main()
{
  overhead = benchmark([]{ },0.2);

  log("pause() ", benchmark([]{ pause(); }));
  log("Time() ", benchmark([]{ Time(); }));
  log("ticks() ", benchmark([]{ ticks(); }));
  log("procTime() ", benchmark([]{ procTime(); }));
 
}