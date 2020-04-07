#include <stdio.h> /* printf */
#include <unistd.h>

#include <chrono>
using namespace std;

// #include <bits/stdc++.h>

typedef std::chrono::_V2::system_clock::time_point Time;
typedef std::chrono::high_resolution_clock Clock;

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

  int64_t elapsed() {
    Time now = Clock::now();
    return chrono::duration_cast<chrono::milliseconds>(now - begin).count();
  };

  int64_t reset() {
    Time now = Clock::now();
    int64_t r = elapsed();
    start();
    return r;
  };

  int64_t round() {
    Time r = lastRound;
    lastRound = Clock::now();
    rounds++;
    return chrono::duration_cast<chrono::milliseconds>(lastRound - r).count();
  };

  int64_t roundAvg() {
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