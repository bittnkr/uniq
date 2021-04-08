#pragma once
#include "std.h"

clock_t ticks(){ return clock(); }

double timer(double prev=0) { 
  double t = ((double)ticks()) / (CLOCKS_PER_SEC); 
  return (prev>0) ? t-prev : t; 
}

double startTimer(string msg="") {
  if (msg != "") cout << msg;
  // Clock = clock();
  return (double)ticks() / (CLOCKS_PER_SEC);
}

double printTimer(string msg = "") {
  double r = timer();
  printf("%s: %.3fs\n", msg.c_str(), r);
  return r;
}

double roundsPerSec(double done) {
  return round(done / timer());
}

// void timeTicker() {
//   // clock_gettime(CLOCK_REALTIME) struct timespec {
//   //   time_t tv_sec; /* seconds */
//   //   long tv_nsec;  /* nanoseconds */
//   // };
//   run(timeTicker);
// };
  
// void test_utils() {
//   printf("clockSpeed():%lu", CLOCKS_PER_SEC);
// }

typedef std::chrono::_V2::system_clock::time_point Time;
typedef std::chrono::high_resolution_clock Clock;

// inline u64 ticks() { return __rdtsc(); }

class Timer {
 public:
  Time begin = Clock::now();
  int rounds = 0;
  Time firstRound = Clock::now();
  Time lastRound = Clock::now();

  Timer() { start(); };

  Time start() {
    begin = Clock::now();
    firstRound = begin;
    lastRound = begin;
    int rounds = 0;
    return begin;
  };

  i64 elapsed() {
    Time now = Clock::now();
    return chrono::duration_cast<chrono::milliseconds>(now - begin).count();
  };

  i64 reset() {
    Time now = Clock::now();
    i64 r = elapsed();
    start();
    return r;
  };

  i64 round() {
    Time r = lastRound;
    lastRound = Clock::now();
    rounds++;
    return chrono::duration_cast<chrono::milliseconds>(lastRound - r).count();
  };

  i64 roundAvg() {
    Time r = firstRound;
    return chrono::duration_cast<chrono::milliseconds>(Clock::now() - firstRound).count() / rounds;
  };
};

// void alrm_handler(int) {
//     alarmed = true;
// }

// int main() {
//     signal(SIGALRM, alrm_handler);

// void signal_handler (int x)
// {
//     printf ("Signal with the following number: %d\n", x);
//     // /* Go back to where setjmp was called. Return the value "1" from
//     //    setjmp. */
//     // longjmp (buf,1);
// }

// template <int ms, typename Func, typename... Args>
// inline void timeout(Func&& f, Args&&... args) {
//   struct sigaction act;
//   act.sa_handler = signal_handler;
//   sigaction(SIGALRM, &act, 0);

//   // Job job = bind(forward<Func>(f), forward<Args>(args)...);
//   // Q.push(job);
// }